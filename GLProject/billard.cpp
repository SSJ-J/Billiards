#include "entity.h"
#include <cmath>
#include <time.h>
#include <iostream>

const float FIRST_BALL_Z = -1.3f;

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
	Point positions[16] = {
		{ 0.0f, r, FIRST_BALL_Z },
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

	//cueStick = new CueStick();	// cue stick
	//cueStick->power = 0;
	//cueStick->direction[0] = 0.0f;
	//cueStick->direction[1] = 0.0f;
	//cueStick->direction[2] = -1.0f;		// front
}

Billard::~Billard() {
	for (int i = 0; i < 16; i++)
		delete balls[i];
	// delete cueStick;
}

Ball *Billard::getBall(unsigned char num) const {
	if (num < 0 || num >= 16)
		return NULL;
	return balls[num];
}

void Billard::shoot(Point accDir) {
	// 100 is the conversion fraction
	float tmp = 200 * accDir.norm();
	Point acc = accDir / tmp;
	if (balls[0]->vel.norm() < 0.1f)
		balls[0]->vel += acc;
}

std::vector<Point> Billard::updateBalls() {
	int walknum[6] = { 5, 6, 7, 13, 14, 15 };
	WalkBall *wb = NULL;
	FlyBall *fb = (FlyBall *)balls[8];
	Ball *ball = NULL;
	std::vector<Point> result = crashPoints;

	for (int i = 0; i < 16; i++) {
		ball = balls[i];
		ball->move();
	}

	for (int j = 1; j < 16; j++) {
		if (balls[0]->collisionCheck(balls[j])) {
			Point p = (balls[0]->pos + balls[j]->pos) / 2.0f;
			crashPoints.push_back(p);
		}
	}

	for (int i = 1; i < 16; i++) {
		for (int j = i + 1; j < 16; j++)
			balls[i]->collisionCheck(balls[j]);
	}

	if(crashPoints.size() > 0 )
		crashPoints.clear();		// make the vector empty
	return result;
}

//CueStick *Billard::getStick() const {
//	return cueStick;
//}

//void Billard::updateStick(float stickAngle, float stickPower) {
//	float pi = 4 * atan(1);
//	cueStick->direction[0] = -sin(stickAngle / 180 * pi);
//	cueStick->direction[1] = 0.0f;
//	cueStick->direction[2] = -cos(stickAngle / 180 * pi);
//	cueStick->power = stickPower;
//}