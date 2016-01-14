#include "sphere.h"
#include <cmath>

#define PI 3.14159265358979323846

Point getPoint(float u, float v) {
	float x = sin(PI*v)*cos(2 * PI * u);
	float y = sin(PI*v)*sin(2 * PI * u);
	float z = cos(PI*v);
	return Point(x, y, z);
}

void drawSphere(float r, GLuint texNum, GLuint uStepsNum, GLuint vStepNum) {
	float ustep = 1.0f / uStepsNum, vstep = 1.0f / vStepNum;
	float u = 0, v = 0;

	glPushMatrix();
	glScalef(r, r, r);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, texNum);

	// Bottom triangles
	for (int i = 0; i<uStepsNum; i++) {
		glBegin(GL_TRIANGLES);
		Point a = getPoint(0.0f, 0.0f);
		Point b = getPoint(u, vstep);
		Point c = getPoint(u + ustep, vstep);
		Point n = (b - a) * (c - a);
		n /= n.norm();
		glNormal3f(n.x, n.y, n.z);
		glTexCoord3f((1 + a.x) / 2, (1 + a.y) / 2, (1 + a.z) / 2); glVertex3f(a.x, a.y, a.z);
		glTexCoord3f((1 + b.x) / 2, (1 + b.y) / 2, (1 + b.z) / 2); glVertex3f(b.x, b.y, b.z);
		glTexCoord3f((1 + c.x) / 2, (1 + c.y) / 2, (1 + c.z) / 2); glVertex3f(c.x, c.y, c.z);
		u += ustep;
		glEnd();
	}

	// Middle quads
	u = 0, v = vstep;
	for (int i = 1; i<vStepNum - 1; i++) {
		for (int j = 0; j<uStepsNum; j++) {
			glBegin(GL_QUADS);
			Point a = getPoint(u, v);
			Point b = getPoint(u, v + vstep);
			Point c = getPoint(u + ustep, v + vstep);
			Point d = getPoint(u + ustep, v);
			Point n = (b - a) * (d - a);
			n /= n.norm();
			glNormal3f(n.x, n.y, n.z);
			glTexCoord3f((1 + a.x) / 2, (1 + a.y) / 2, (1 + a.z) / 2); glVertex3f(a.x, a.y, a.z);
			glTexCoord3f((1 + b.x) / 2, (1 + b.y) / 2, (1 + b.z) / 2); glVertex3f(b.x, b.y, b.z);
			glTexCoord3f((1 + c.x) / 2, (1 + c.y) / 2, (1 + c.z) / 2); glVertex3f(c.x, c.y, c.z);
			glTexCoord3f((1 + d.x) / 2, (1 + d.y) / 2, (1 + d.z) / 2); glVertex3f(d.x, d.y, d.z);
			u += ustep;
			glEnd();
		}
		v += vstep;
	}

	// Top triangles
	u = 0;
	for (int i = 0; i<uStepsNum; i++) {
		glBegin(GL_TRIANGLES);
		Point a = getPoint(0, 1);
		Point b = getPoint(u, 1 - vstep);
		Point c = getPoint(u + ustep, 1 - vstep);
		Point n = (b - a) * (c - a);
		n /= n.norm();
		glNormal3f(n.x, n.y, n.z);
		glTexCoord3f((1 + a.x) / 2, (1 + a.y) / 2, (1 + a.z) / 2); glVertex3f(a.x, a.y, a.z);
		glTexCoord3f((1 + b.x) / 2, (1 + b.y) / 2, (1 + b.z) / 2); glVertex3f(b.x, b.y, b.z);
		glTexCoord3f((1 + c.x) / 2, (1 + c.y) / 2, (1 + c.z) / 2); glVertex3f(c.x, c.y, c.z);
		u += ustep;
		glEnd();
	}

	glDisable(GL_TEXTURE_3D);
	glPopMatrix();
}

