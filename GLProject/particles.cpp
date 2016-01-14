#include "particles.h"

#define MAX_PART 1000

static Point colors[12] = {
	{ 1.0f,0.5f,0.5f },{ 1.0f,0.75f,0.5f },{ 1.0f,1.0f,0.5f },{ 0.75f,1.0f,0.5f },
	{ 0.5f,1.0f,0.5f },{ 0.5f,1.0f,0.75f },{ 0.5f,1.0f,1.0f },{ 0.5f,0.75f,1.0f },
	{ 0.5f,0.5f,1.0f },{ 0.75f,0.5f,1.0f },{ 1.0f,0.5f,1.0f },{ 1.0f,0.5f,0.75f }
};

Particles::Particles()
	: maxNum(MAX_PART), particles(new Particle[MAX_PART]), crashParts(new Particle[MAX_PART])
{ 
	init();
	crashInit({0.0f, 0.0f, 0.0f});
}

Particles::~Particles() {
	delete[] particles;
	delete[] crashParts;
}

void Particles::init() {
	for (int i = 0; i < maxNum; i++)	{				// Initials All The Textures
		particles[i].active = true;								// Make All The Particles Active
		particles[i].life = 1.0f;								// Give All The Particles Full Life
		particles[i].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
		particles[i].color = colors[i*(12 / maxNum)];
		crashParts[i].position = { 0.0f, 0.0f, 0.0f };
		particles[i].velocity = {
			float((rand() % 50) - 25.0f)*10.0f,
			float((rand() % 50) - 25.0f)*10.0f,
			float((rand() % 50) - 25.0f)*10.0f
		};
		particles[i].accelarate = { 0.0f, -0.8f, 0.0f };
	}
}

void Particles::crashInit(Point pos) {
	crashPos = pos;
	for (int i = 0; i < maxNum; i++) {				// Initials All The Textures
		crashParts[i].active = true;								// Make All The Particles Active
		crashParts[i].life = 1.0f;								// Give All The Particles Full Life
		crashParts[i].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Speed
		crashParts[i].color = colors[i*(12 / maxNum)];
		crashParts[i].position = pos;
		crashParts[i].velocity = {
			float((rand() % 50) - 25.0f)*10.0f,
			float((rand() % 50) - 25.0f)*10.0f,
			float((rand() % 50) - 25.0f)*10.0f
		};
		crashParts[i].accelarate = { 0.0f, -0.8f, 0.0f };
	}
}


void Particles::draw(Point pos, Point vel, GLuint texture) {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);									// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);					// Type Of Blending To Perform
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(0.1f, 0.1f, 0.1f);

	for (int i = 0; i < maxNum; i++) {
		if (particles[i].active) {
			float x = particles[i].position.x;
			float y = particles[i].position.y;
			float z = particles[i].position.z;

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glBindTexture(GL_TEXTURE_2D, texture);			// Select Our Texture

			glColor4f(particles[i].color.x,
				particles[i].color.y,
				particles[i].color.z,
				particles[i].life);

			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + 0.5f, y + 0.5f, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - 0.5f, y + 0.5f, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + 0.5f, y - 0.5f, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - 0.5f, y - 0.5f, z); // Bottom Left
			glEnd();										// Done Building Triangle Strip

			/* move and update*/
			particles[i].position += (particles[i].velocity / 2000.f);
			particles[i].velocity += particles[i].accelarate;
			particles[i].life -= particles[i].fade;		// Reduce Particles Life By 'Fade'

			/* "relive" */
			if (particles[i].life < 0.0f) {
				particles[i].life = 1.0f;					// Give It New Life
				particles[i].fade = float(rand() % 100) / 1000.0f + 0.003f;	// Random Fade Value
				particles[i].position = { 0.0f, 0.0f, 0.0f };
				particles[i].velocity = { float((rand() % 60) - 32.0f),
										  float((rand() % 60) - 30.0f),
										  float((rand() % 60) - 30.0f) };
				particles[i].color = colors[i % 12];
			}
		}
		particles[i].accelarate = vel * (-200.0f);
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPopMatrix();
}

void Particles::crash(GLuint texture) {
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);									// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);					// Type Of Blending To Perform
	glTranslatef(crashPos.x, crashPos.y, crashPos.z);
	glScalef(0.1f, 0.1f, 0.1f);

	for (int i = 0; i < maxNum; i++) {
		if (crashParts[i].active) {
			float x = crashParts[i].position.x;
			float y = crashParts[i].position.y;
			float z = crashParts[i].position.z;

			// Draw The Particle Using Our RGB Values, Fade The Particle Based On It's Life
			glBindTexture(GL_TEXTURE_2D, texture);			// Select Our Texture

			glColor4f(crashParts[i].color.x,
				crashParts[i].color.y,
				crashParts[i].color.z,
				crashParts[i].life);

			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
			glTexCoord2d(1, 1); glVertex3f(x + 0.5f, y + 0.5f, z); // Top Right
			glTexCoord2d(0, 1); glVertex3f(x - 0.5f, y + 0.5f, z); // Top Left
			glTexCoord2d(1, 0); glVertex3f(x + 0.5f, y - 0.5f, z); // Bottom Right
			glTexCoord2d(0, 0); glVertex3f(x - 0.5f, y - 0.5f, z); // Bottom Left
			glEnd();										// Done Building Triangle Strip

			/* move and update*/
			crashParts[i].position += (crashParts[i].velocity / 2000.f);
			crashParts[i].velocity += crashParts[i].accelarate;
			crashParts[i].life -= crashParts[i].fade;		// Reduce Particles Life By 'Fade'
		}
	}
	glEnable(GL_LIGHTING);
	glDisable(GL_BLEND);
	glPopMatrix();
}