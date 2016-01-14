#pragma once

#include "background.h"
#include "Vector3.h"

// Particles Structure
struct Particle {
	bool	active;					// Active (Yes/No)
	float	life;					// Particle Life
	float	fade;					// Fade Speed
	Point	color;					// R, G, B
	Point	position;				
	Point	velocity;				// velocity direction		
	Point	accelarate;				// accelarate direction
};							

class Particles {
public:
	Particles();
	~Particles();
	void draw(Point pos, Point vel, GLuint texture);
	void crashInit(Point pos);
	void crash(GLuint texture);

private:
	Particle *particles;
	Particle *crashParts;
	Point crashPos;
	int maxNum;
	void init();
	
};
