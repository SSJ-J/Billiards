#pragma once

#include "Vector3.h"

/* Confugration of table */
#define WIDTH 3.0f
#define LENGTH (2 * WIDTH)
#define HEIGHT 1.0f
#define THICKNESS  0.2f

/* Confugration of ball */
#define RADIUS 0.08f

/* Ball on the table */
class Ball {
public:
	unsigned char num;		// number
	float rad;				// radius
	Point pos;				// position on the table
	Point vel;				// velocity

public:
	Ball();
	Ball(unsigned char number, Point position, float radius = RADIUS);
	virtual void move();
	void collisionCheck(Ball * const ball);

protected:
	void frictionFreeMove();
	void boundCheck();
};

/* Balls that can walk slowly on table */
class WalkBall : public Ball {
public:
	WalkBall();
	WalkBall(unsigned char number, Point position, float radius = RADIUS);
	virtual void move();
};

/* Golden Snitch -- which can fly above the table */
class FlyBall : public Ball {
public:
	FlyBall();
	FlyBall(unsigned char number, Point position, float radius = RADIUS);
	virtual void move();
	void landing();
};

/* All balls on a table */
/* 1 cue ball + 8 normal balls + 6 walkballs + 1 flyballs = 16 balls*/
/* 1 cue stick */
class Billard {
private:
	Ball *balls[16];

public:
	Billard();
	~Billard();
	Ball *getBall(unsigned char num) const;
	void shoot(Point accDir);
	void updateBalls();
};

// CueStick *Billard::cueStick;
// CueStick *Billard::getStick() const;
// void Billard::updateStick(float stickAngle, float stickPower);

/* Confugration of cue stick */
//#define MIN_POWER 1.0f
//#define MAX_POWER 10.0f

//struct CueStick {
//	float power;		// power used to push stick
//	float direction[3];		// direction of stick
//};