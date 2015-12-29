#include "entity.h"
#include <cmath>
#include <time.h>
#include <iostream>

/* Confugration of balls
	9	2	11	 4	 10
	  7	  3    12  15
		14	 8	  6
		   5   13
			 1
*/
Billard::Billard() {
	float r = RADIUS;
	float hpl = sqrt(3) * r;		// height per layer
	float first = -3.5f;	// position 1
							/* position of 16 balls */
	float positions[16][3] = {
		{ 0.0f, r, -1.3f },
		{ 0.0f, r, first },
		{ -2 * r, r, first - 4 * hpl },
		{ -r, r, first - 3 * hpl },
		{ 2 * r, r, first - 4 * hpl },
		{ -r, r, first - hpl },
		{ 2 * r, r, first - 2 * hpl },
		{ -3 * r, r, first - 3 * hpl },
		{ 0.0f, r, first - 2 * hpl },
		{ -4 * r, r, first - 4 * hpl },
		{ 4 * r, r, first - 4 * hpl },
		{ 0.0f, r, first - 4 * hpl },
		{ r, r, first - 3 * hpl },
		{ r, r, first - hpl },
		{ -2 * r, r, first - 2 * hpl },
		{ 3 * r, r, first - 3 * hpl }
	};

	balls[0] = new Ball(0, positions[0]);	// cue ball
	balls[8] = new FlyBall(8, positions[8]);	// Golden Snitch
	for (int i = 1; i <= 4; i++) {		// normal balls
		balls[i] = new Ball(i, positions[i]);
		balls[i + 8] = new Ball(i + 8, positions[i + 8]);
	}
	for (int i = 5; i <= 7; i++) {		// Ghost Ball
		balls[i] = new WalkBall(i, positions[i]);
		balls[i + 8] = new WalkBall(i + 8, positions[i + 8]);
	}

	cueStick = new CueStick();	// cue stick
	cueStick->power = 0;
	cueStick->direction[0] = 0.0f;
	cueStick->direction[1] = 0.0f;
	cueStick->direction[2] = -1.0f;		// front
}

Billard::~Billard() {
	for (int i = 0; i < 16; i++)
		delete balls[i];
	delete cueStick;
}

Ball *Billard::getBall(unsigned char num) const {
	if (num < 0 || num >= 16)
		return NULL;
	return balls[num];
}

CueStick *Billard::getStick() const {
	return cueStick;
}

void Billard::shoot(int interval) {
	if (interval == 0) {
		float power = cueStick->power;
		float dx = cueStick->direction[0],
			  dy = cueStick->direction[1],
			  dz = cueStick->direction[2];
		balls[0]->vel[X] = power*dx / 100.0f;
		balls[0]->vel[Y] = power*dy / 100.0f;
		balls[0]->vel[Z] = power*dz / 100.0f;
	}
}

void Billard::updateStick(float stickAngle, float stickPower) {
	float pi = 4 * atan(1);
	cueStick->direction[0] = -sin(stickAngle / 180 * pi);
	cueStick->direction[1] = 0.0f;
	cueStick->direction[2] = -cos(stickAngle / 180 * pi);
	cueStick->power = stickPower;
}

void Billard::updateBalls() {
	int walknum[6] = { 5, 6, 7, 13, 14, 15 };
	WalkBall *wb = NULL;
	FlyBall *fb = (FlyBall *)balls[8];

	balls[0]->move();

	/* Walk Ball*/
	for (int i = 0; i < 6; i++) {
		wb = (WalkBall *)balls[walknum[i]];
		wb->move();
	}

	/* Fly Ball */
	fb->move();
}