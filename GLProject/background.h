#pragma once

#include <windows.h>	
#include <string>
#include <GL\gl.h>			
#include <GL\glu.h>			
#include <GL\glut.h>	

extern bool keys[256];	// Array Used For The Keyboard Routine
extern bool mouse[3];		// Array Used For The Mouse Routine(0-L, 1-M, 2-R)

class Handlers {
public:
	virtual GLint initalHandler() = 0;
	virtual GLint drawHandler() = 0;
};

class Window {

public:
	Window(std::string title, int width, int height, int bits, Handlers *handlers);
	GLboolean createGLWindow();		// OpenGL Window
	GLvoid loopGLWindow(MSG &msg);	// Draw the window and loop
	GLvoid killGLWindow();			// Properly Kill The Window

private:
	std::string title;		// Title To Appear At The Top Of The Window
	GLint width;			// Width Of The GL Window Or Fullscreen Mode
	GLint height;			// Height Of The GL Window Or Fullscreen Mode
	GLint bits;				// Number Of Bits To Use For Color (8/16/24/32)
	Handlers *handlers;		// A Group Of Handlers(init, draw, texture...)

	HDC	hDC;				// Private GDI Device Context
	HGLRC hRC;				// Permanent Rendering Context
	HWND hWnd;				// Window Handle
	HINSTANCE hInstance;	// Instance Of The Application

	static LRESULT CALLBACK wndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
	static GLvoid reSizeGLScene(GLsizei width, GLsizei height);		// resize the window

};
