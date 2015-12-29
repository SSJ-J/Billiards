#include <windows.h>		
#include "background.h"
#include "draw.h"

char* const WIN_TITLE = "¿ýµØÆæ×ÀÇò";
const int WIN_WIDTH = 480;
const int WIN_HEIGHT = 640;
const int WIN_COL_BITS = 16;

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG	msg;						// Windows Message Structure

	GLhandlers handlers;
	Window window(WIN_TITLE, WIN_WIDTH, WIN_HEIGHT, WIN_COL_BITS, &handlers);
	if (!window.createGLWindow())
		return 0;
	window.loopGLWindow(msg);
	window.killGLWindow();		// Kill The Window
	return (msg.wParam);		// Exit The Program
}
