#include "background.h"
#include "resource.h"
using namespace std;

bool keys[256];		// Array Used For The Keyboard Routine
bool mouse[3];		// Array Used For The Mouse Routine(0-L, 1-M, 2-R)
bool active = TRUE;	// Window Active Flag Set To TRUE By Default

Window::Window(string t, int w, int h, int b, Handlers *hdl)
	: title(t), width(w), height(h), bits(b), handlers(hdl) {
	hDC = NULL;
	hRC = NULL;
	hWnd = NULL;	
}

GLvoid Window::reSizeGLScene(GLsizei width, GLsizei height) {	// Resize And Initialize The GL Window
	if (height == 0)									// Prevent A Divide By Zero By
		height = 1;

	glViewport(0, 0, width, height);						// Reset The Current Viewport
	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix
														// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

GLvoid Window::loopGLWindow(MSG &msg) {
	BOOL done = FALSE;								// Bool Variable To Exit Loop

	while (!done) {									// Loop That Runs While done=FALSE
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {	// Is There A Message Waiting?
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
				done = TRUE;							
			else {	
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else {										// If There Are No Messages
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active) {							// Program Active?
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
					done = TRUE;						// ESC Signalled A Quit
				else {								// Not Time To Quit, Update Screen
					handlers->drawHandler();		// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}
		}
	}
}

GLvoid Window::killGLWindow() {							// Properly Kill The Window
	if (hRC) {											// Do We Have A Rendering Context?
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC)) {					// Are We Able To Release The DC
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd)) {					// Are We Able To Destroy The Window?
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance)) {			// Are We Able To Unregister Class
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

GLboolean Window::createGLWindow() {
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)wndProc;			// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);		// Load A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc)) {									// Attempt To Register The Window Class
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
	dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title.c_str(),								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	// Did We Get A Device Context?
	if (!(hDC = GetDC(hWnd)))	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	// Did Windows Find A Matching Pixel Format?
	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd))) { 
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	// Are We Able To Set The Pixel Format?
	if (!SetPixelFormat(hDC, PixelFormat, &pfd)) {		
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	// Are We Able To Get A Rendering Context?
	if (!(hRC = wglCreateContext(hDC)))	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	// Try To Activate The Rendering Context
	if (!wglMakeCurrent(hDC, hRC))	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	reSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	// Initialize Our Newly Created GL Window												
	if (!handlers->initalHandler())	{
		killGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK Window::wndProc(HWND	hWnd,			// Handle For This Window
			UINT	uMsg,								// Message For This Window
			WPARAM	wParam,								// Additional Message Information
			LPARAM	lParam)								// Additional Message Information
{
	// Check For Windows Messages
	switch (uMsg) {
	case WM_ACTIVATE:							// Watch For Window Activate Message
		if (!HIWORD(wParam))					// Check Minimization State
			active = TRUE;						// Program Is Active
		else
			active = FALSE;						// Program Is No Longer Active
		return 0;								// Return To The Message Loop

	case WM_CLOSE:								// Did We Receive A Close Message?
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back

	case WM_KEYDOWN:							// Is A Key Being Held Down?
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		break;									// Jump Back

	case WM_KEYUP:								// Has A Key Been Released?
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		break;									// Jump Back

	case WM_LBUTTONDOWN:						// Is Left Button Being Held Down?
		mouse[0] = TRUE;							// If So, Mark It As TRUE
		break;									// Jump Back

	case WM_LBUTTONUP:							// Is Left Button Being Released?
		mouse[0] = FALSE;						// If So, Mark It As FALSE
		break;									// Jump Back

	case WM_MBUTTONDOWN:						// Is Middle Button Being Held Down?
		mouse[1] = TRUE;							// If So, Mark It As TRUE
		break;									// Jump Back

	case WM_MBUTTONUP:							// Is Middle Button Being Released?
		mouse[1] = FALSE;						// If So, Mark It As FALSE
		break;									// Jump Back

	case WM_RBUTTONDOWN:						// Is Right Button Being Held Down?
		mouse[2] = TRUE;							// If So, Mark It As TRUE
		break;									// Jump Back

	case WM_RBUTTONUP:							// Is Right Button Being Released?
		mouse[2] = FALSE;						// If So, Mark It As FALSE
		break;									// Jump Back

	case WM_SIZE:								// Resize The OpenGL Window
		reSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back

	case WM_SYSCOMMAND: {						// Intercept System Commands
		switch (wParam) {						// Check System Calls
		case SC_SCREENSAVE:						// Screensaver Trying To Start?
		case SC_MONITORPOWER:					// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		default:
			break;
		}
		break;									// Exit
	}

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {					// Command Of Menu
		case ID_EXIT:								// "ÍË³ö"
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back

		default:
			break;
		}
		break;
	}

	default:
		break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}