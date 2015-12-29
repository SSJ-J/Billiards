#pragma once

/* Confugration of table */
#define WIDTH 3.0f
#define LENGTH (2 * WIDTH)
#define HEIGHT 1.0f
#define THICKNESS  0.2f

/* Confugration of ball */
#define RADIUS 0.08f

/* Confugration of cue stick */
#define MIN_POWER 1.0f
#define MAX_POWER 10.0f

#define X	(0)
#define Y	(1)
#define Z	(2)

/* Ball on the table */
class Ball {
public:
	unsigned char num;		// number
	float rad;				// radius
	float pos[3];			// position on the table
	float vel[3];			// velocity

public:
	Ball();
	Ball(unsigned char number, float position[3], float radius = RADIUS);
	virtual void move();

protected:
	void frictionFreeMove();
	void boundCheck(int dir);
};

/* Balls that can walk slowly on table */
class WalkBall : public Ball {
public:
	WalkBall();
	WalkBall(unsigned char number, float position[3], float radius = RADIUS);
	virtual void move();
};

/* Golden Snitch -- which can fly above the table */
class FlyBall : public Ball {
public:
	FlyBall();
	FlyBall(unsigned char number, float position[3], float radius = RADIUS);
	virtual void move();
	void landing();
};

struct CueStick {
	float power;		// power used to push stick
	float direction[3];		// direction of stick
};

/* All balls on a table */
/* 1 cue ball + 8 normal balls + 6 walkballs + 1 flyballs = 16 balls*/
/* 1 cue stick */
class Billard {
private:
	Ball *balls[16];
	CueStick *cueStick;

public:
	Billard();
	~Billard();
	Ball *getBall(unsigned char num) const;
	CueStick *getStick() const;
	void shoot(int interval);
	void updateStick(float stickAngle, float stickPower);
	void updateBalls();
};