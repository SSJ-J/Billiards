#include <stdio.h>
#include "draw.h"
#include <GL/GLAUX.H>

#define iWidth 16
#define iHeight 16 
#define iDepth 16
GLubyte image[iDepth][iHeight][iWidth][3];

/*  Create a 16x16x16x3 array with different color values in
*  each array element [r, g, b].  Values range from 0 to 255.
*/
void makeImage(void) {
	for (int s = 0; s < iWidth; s++)
		for (int t = 0; t < iHeight; t++)
			for (int r = 0; r < iDepth; r++) {
				image[r][t][s][0] = (GLubyte)(s * 17);
				image[r][t][s][1] = (GLubyte)(t * 17);
				image[r][t][s][2] = (GLubyte)(r * 17);
			}
}


AUX_RGBImageRec *loadBMP(const char *filename) {
	if (!filename) return NULL;

	/* Test if the file exsists or not*/
	FILE *pic = NULL;
	fopen_s(&pic, filename, "r");
	if (pic) {
		fclose(pic);
		return auxDIBImageLoad(filename);
	}
	return NULL;
}

char *pictures[] = {
	"Data/ball_0.bmp",
	"Data/ball_1.bmp",
	"Data/ball_2.bmp",
	"Data/ball_3.bmp",
	"Data/ball_4.bmp",
	"Data/ball_5.bmp",
	"Data/ball_6.bmp",
	"Data/ball_7.bmp",
	"Data/ball_8.bmp",
	"Data/ball_9.bmp",
	"Data/ball_10.bmp",
	"Data/ball_11.bmp",
	"Data/ball_12.bmp",
	"Data/ball_13.bmp",
	"Data/ball_14.bmp",
	"Data/ball_15.bmp",
	"Data/table.bmp",
	"Data/cuestick.bmp",
	"Data/flag_1.bmp",
	"Data/flag_2.bmp",
	"Data/title.bmp",
	"Data/sky.bmp",
	"Data/ground.bmp",
	"Data/Particle.bmp"
};

GLint GLhandlers::loadGLTexture(GLuint *textures) {
	int flag = FALSE;
	AUX_RGBImageRec *texturePic[TEXTURES];
	for (int i = 0; i < TEXTURES; i++) {
		if (i == TEXTURE_BALL_BASE + 8) continue;
		if (texturePic[i] = loadBMP((pictures[i]))) {	// load BMP
			flag = TRUE;
			glGenTextures(1, &textures[i]);		// generate textures
			glBindTexture(GL_TEXTURE_2D, textures[i]);	// 2d
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
				texturePic[i]->sizeX, texturePic[i]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE,
				texturePic[i]->data);
			if (texturePic[i]) {	// free
				if (texturePic[i]->data)
					free(texturePic[i]->data);
				free(texturePic[i]);
			}
		}
	}

	makeImage();
	glTexImage3D = (PFNGLTEXIMAGE3DPROC)wglGetProcAddress("glTexImage3D");
	glGenTextures(1, &textures[TEXTURE_BALL_BASE + 8]);
	glBindTexture(GL_TEXTURE_3D, textures[TEXTURE_BALL_BASE + 8]);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER,
		GL_NEAREST);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER,
		GL_NEAREST);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, iWidth, iHeight,
		iDepth, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	return flag;
}