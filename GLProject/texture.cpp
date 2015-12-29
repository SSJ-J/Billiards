#include <GL/GLAUX.H>
#include <stdio.h>
#include "draw.h"

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
	"Data/title.bmp"
};

GLint GLhandlers::loadGLTexture(GLuint *textures, GLuint size) {
	int flag = FALSE;
	AUX_RGBImageRec *texturePic[TEXTURES];
	for (int i = 0; i < TEXTURES; i++) {
		if (texturePic[i] = loadBMP((pictures[i]))) {	// load BMP
			flag = TRUE;
			glGenTextures(1, &textures[i]);		// generate textures
			glBindTexture(GL_TEXTURE_2D, textures[i]);	// 2d
			glTexImage2D(GL_TEXTURE_2D, 0, 3,
				texturePic[i]->sizeX, texturePic[i]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE,
				texturePic[i]->data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			if (texturePic[i]) {	// free
				if (texturePic[i]->data)
					free(texturePic[i]->data);
				free(texturePic[i]);
			}
		}
	}
	return flag;
}