#include "entity.h"
#include <cmath>
#include <time.h>

/* --------- Ball ---------- */
Ball::Ball()
	: num(0), rad(RADIUS), pos(Point()), vel(Point()) { }

Ball::Ball(unsigned char number, Point position, float radius)
	: num(number), pos(position), rad(radius) { }

const float fraction = float(1e-3);
void Ball::move() {
	frictionFreeMove();

	if (pow(vel.x, 2) + pow(vel.z, 2) < pow(fraction, 2)) {
		vel.x = vel.y = vel.z = 0.0f;
		return;
	}

	float tmp = fraction / (abs(vel.x) + abs(vel.z));
	vel.x -= vel.x * tmp;
	vel.y = 0.0f;
	vel.z -= vel.z * tmp;
}

void Ball::frictionFreeMove() {
	pos.x += vel.x;
	pos.y += vel.y;
	pos.z += vel.z;
	boundCheck();
}

/* Is the ball over the bound? 
 * If true, change position and velocity
  */
void Ball::boundCheck() {
	if (pos.x < -WIDTH / 2 + RADIUS) {
		pos.x = -WIDTH / 2 + RADIUS;
		vel.x = -vel.x;
	}
	else if (pos.x > WIDTH / 2 - RADIUS) {
		pos.x = WIDTH / 2 - RADIUS;
		vel.x = -vel.x;
	}

	if (pos.y < RADIUS)
		pos.y = RADIUS;
	else if (pos.y > HEIGHT)
		pos.y = HEIGHT;

	if (pos.z > -RADIUS) {
		pos.z = -RADIUS;
		vel.z = -vel.z;
	}
	else if (pos.z < -(LENGTH - RADIUS)) {
		pos.z = -(LENGTH - RADIUS);
		vel.z = -vel.z;
	}
}

/*  Is the ball crashing the other ball?
 *  If true, change position and velocity
 *  (Only considering 2D collision)
 */
bool Ball::collisionCheck(Ball * const that) {
	Point p1 = pos, p2 = that->pos;
	Point v1 = vel, v2 = that->vel;
	float r = rad;			// radius is the same
	Point dir1 = p2 - p1, dir2 = p1-p2;
	if (dir1.norm() > 2 * r)		// no collision
		return false;
	if (v1.x * v2.x + v1.z * v2.z < 0) return false;

	/* Component of velocity on the direction of 'dir' */
	// (v1 ¡¤ dir) / (norm(dir))^2 * dir
	Point c1 = dir1 * ((v1.x * dir1.x + v1.z * dir1.z) / (dir1.norm() * dir1.norm()));
	Point c2 = dir2 * ((v2.x * dir2.x + v2.z * dir2.z) / (dir2.norm() * dir2.norm()));

	/* Exchange the velocity on 'dir' */
	vel = vel - c1 + c2;
	vel.y = 0.0f;
	that->vel = v2 - c2 + c1;
	that->vel.y = 0.0f;

	/* make a distance */
	pos += dir2 / (dir2.norm() * 10);
	that->pos += dir1 / (dir1.norm() * 10);

	return true;
}

/* --------- Walk Ball ---------- */
const float WALK_VELOCITY = 0.0005f;
const float FLY_VELOCITY = 0.0025f;

WalkBall::WalkBall() : Ball() { }

WalkBall::WalkBall(unsigned char number, Point position, float radius)
	: Ball(number, position, radius) { }

void WalkBall::move() {
	static int count = 0;
	if (count % 1000 == num) {
		float dirX = rand() % 50 - 25, 
			  dirZ = rand() % 100 - 50;
		float tmp = sqrt(pow(dirX, 2) + pow(dirZ, 2));
		vel.x = WALK_VELOCITY * dirX / tmp;
		vel.y = 0.0f;
		vel.z = WALK_VELOCITY * dirZ / tmp;
	}
	Ball::frictionFreeMove();
	count = (count + 1) % 5001;
}

/* --------- Fly Ball ---------- */
FlyBall::FlyBall() : Ball() { }

FlyBall::FlyBall(unsigned char number, Point position, float radius)
	: Ball(number, position, radius) { }

void FlyBall::move() {
	static int count = 0;
	if (count % 1000 == 0) {
		float dirX = rand() % 50 - 25,
			  dirY = rand() % 10 - 3,
			  dirZ = rand() % 100 - 50;
		float tmp = sqrt(pow(dirX, 2) + pow(dirY, 2) + pow(dirZ, 2));
		vel.x = FLY_VELOCITY * dirX / tmp;
		vel.y = FLY_VELOCITY * dirY / tmp;
		vel.z = FLY_VELOCITY * dirZ / tmp;
	}
	if (count >= 2000 && count <= 3000) 
		landing();
	else
		Ball::frictionFreeMove();

	count = (count + 1) % 5001;
}

void FlyBall::landing() {
	pos.y -= 0.001f;
	boundCheck(); 
}


