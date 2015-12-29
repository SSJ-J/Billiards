#include "entity.h"
#include <cmath>
#include <time.h>

const float ORIGIN[3] = { 0.0f, 0.0f, 0.0f };	// origin point

/* --------- Ball ---------- */
Ball::Ball()
	: num(0), rad(RADIUS) {
	for (int i = 0; i < 3; i++) {
		pos[i] = ORIGIN[i];
		vel[i] = ORIGIN[i];
	}
}

Ball::Ball(unsigned char number, float position[3], float radius)
	: num(number), rad(radius) {
	for (int i = 0; i < 3; i++) 
		pos[i] = position[i];
}

// check the ball in the bound or not
float boundCheck(float point, int dir) {
	switch (dir) {
	case X: {
		if (point < -WIDTH / 2 + RADIUS)
			point = -WIDTH / 2 + RADIUS;
		else if (point > WIDTH / 2 - RADIUS)
			point = WIDTH / 2 - RADIUS;
		break;
	}
	case Y: {
		if (point < RADIUS)
			point = RADIUS;
		else if (point > HEIGHT)
			point = HEIGHT;
		break;
	}
	case Z: {
		if (point > -RADIUS)
			point = -RADIUS;
		else if (point < -(LENGTH - RADIUS))
			point = -(LENGTH - RADIUS);
		break;
	}
	default:
		break;
	}
	return point;
}

const float fraction = float(1e-3);
void Ball::move() {
	frictionFreeMove();

	if (pow(vel[X], 2) + pow(vel[Z], 2) < pow(fraction, 2)) {
		vel[X] = vel[Y] = vel[Z] = 0.0f;
		return;
	}

	float tmp = fraction / (abs(vel[X]) + abs(vel[Z]));
	vel[X] -= vel[X] * tmp;
	vel[Y] = 0.0f;
	vel[Z] -= vel[Z] * tmp;
}

void Ball::frictionFreeMove() {
	for (int i = 0; i < 3; i++) {
		pos[i] += vel[i];
		boundCheck(i);
	}
}

/* Is the ball over the bound? 
 * If true, change position and velocity
  */
void Ball::boundCheck(int dir) {
	switch (dir) {
	case X: {
		if (pos[X] < -WIDTH / 2 + RADIUS) {
			pos[X] = -WIDTH / 2 + RADIUS;
			vel[X] = -vel[X];
		}
		else if (pos[X] > WIDTH / 2 - RADIUS) {
			pos[X] = WIDTH / 2 - RADIUS;
			vel[X] = -vel[X];
		}
		break;
	}
	case Y: {
		if (pos[Y] < RADIUS)
			pos[Y] = RADIUS;
		else if (pos[Y] > HEIGHT)
			pos[Y] = HEIGHT;
		break;
	}
	case Z: {
		if (pos[Z] > -RADIUS) {
			pos[Z] = -RADIUS;
			vel[Z] = -vel[Z];
		}
		else if (pos[Z] < -(LENGTH - RADIUS)) {
			pos[Z] = -(LENGTH - RADIUS);
			vel[Z] = -vel[Z];
		}
		break;
	}
	default:
		break;
	}
}

/* --------- Walk Ball ---------- */
const float WALK_VELOCITY = 0.0005f;
const float FLY_VELOCITY = 0.0025f;

WalkBall::WalkBall() : Ball() { }

WalkBall::WalkBall(unsigned char number, float position[3], float radius)
	: Ball(number, position, radius) {
}

void WalkBall::move() {
	static int count = 0;
	if (count % 1000 == num) {
		float dirX = rand() % 50 - 25, 
			  dirZ = rand() % 100 - 50;
		float tmp = sqrt(pow(dirX, 2) + pow(dirZ, 2));
		vel[X] = WALK_VELOCITY * dirX / tmp;
		vel[Y] = 0.0f;
		vel[Z] = WALK_VELOCITY * dirZ / tmp;
	}
	Ball::frictionFreeMove();
	count = (count + 1) % 5001;
}

/* --------- Fly Ball ---------- */
FlyBall::FlyBall() : Ball() { }

FlyBall::FlyBall(unsigned char number, float position[3], float radius)
	: Ball(number, position, radius) {

}

void FlyBall::move() {
	static int count = 0;
	if (count % 1000 == 0) {
		float dirX = rand() % 50 - 25,
			  dirY = rand() % 10 - 3,
			  dirZ = rand() % 100 - 50;
		float tmp = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
		vel[X] = FLY_VELOCITY * dirX / tmp;
		vel[Y] = FLY_VELOCITY * dirY / tmp;
		vel[Z] = FLY_VELOCITY * dirZ / tmp;
	}
	if (count >= 2000 && count <= 3000) 
		landing();
	else
		Ball::frictionFreeMove();

	count = (count + 1) % 5001;
}

void FlyBall::landing() {
	pos[Y] -= 0.001f;
	boundCheck(Y); 
}


