#pragma once

#include "background.h"
#include "particles.h"

#define TEXTURES				24
#define TEXTURES_2D				23
#define TEXTURES_3D				0
#define TEXTURE_BALL_BASE		0
#define TEXTURE_TABLE			16
#define TEXTURE_CUESTICK		17
#define TEXTURE_FLAG_1			18
#define TEXTURE_FLAG_2			19
#define TEXTURE_TITLE			20
#define TEXTURE_SKY				21
#define TEXTURE_GROUND			22
#define TEXTURE_PARTICLE		23

#define MAP_X 1024
#define MAP_Y 1024

class GLhandlers : public Handlers {

public:
	virtual GLint initalHandler();
	virtual GLint drawHandler();
private:
	GLuint textures[TEXTURES];		// textures on model
	Particles part;

	GLint loadGLTexture(GLuint *textures);
	GLvoid loadRawFile(LPSTR strName, GLuint nSize, BYTE *pHeightMap);

	// GLvoid renderTable();		// Render the surface of table
	GLvoid renderBall();		// Render 16 balls on table
	GLvoid renderFlag(GLfloat x, GLfloat z, GLint texture);	// Render flags 
	GLvoid renderBanner(GLfloat x, GLfloat y, GLfloat z, GLint texture);	// Render Banner
	GLvoid renderSky();
	GLvoid renderTerrain();

	// GLvoid renderCueStick();	// Render cue stick
};

