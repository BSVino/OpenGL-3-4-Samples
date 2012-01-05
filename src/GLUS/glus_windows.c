/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <windows.h>

#include <stdio.h>

#include "GL/glus.h"

#ifndef WGL_CONTEXT_DEBUG_BIT_ARB
#define WGL_CONTEXT_DEBUG_BIT_ARB      0x0001
#endif
#ifndef WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#endif
#ifndef WGL_CONTEXT_MAJOR_VERSION_ARB
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#endif
#ifndef WGL_CONTEXT_MINOR_VERSION_ARB
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#endif
#ifndef WGL_CONTEXT_LAYER_PLANE_ARB
#define WGL_CONTEXT_LAYER_PLANE_ARB    0x2093
#endif
#ifndef WGL_CONTEXT_FLAGS_ARB
#define WGL_CONTEXT_FLAGS_ARB          0x2094
#endif
#ifndef ERROR_INVALID_VERSION_ARB
#define ERROR_INVALID_VERSION_ARB      0x2095
#endif

typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROCTEMP) (HDC hDC, HGLRC hShareContext, const int *attribList);

static GLUSuint g_major = 1;
static GLUSuint g_minor = 0;
static GLUSint g_flags = 0;

static GLUSboolean g_fullscreen = GL_FALSE;
static GLUSboolean g_active = GL_FALSE;
static GLUSboolean g_initdone = GL_FALSE;
static GLUSboolean g_keydown = GL_FALSE;

static GLUSuint g_width = 640;
static GLUSuint g_height = 480;

static HDC g_hDC = NULL; // Private GDI Device Context
static HGLRC g_hRC = NULL; // Permanent Rendering Context
static HWND g_hWnd = NULL; // Holds Our Window Handle
static HINSTANCE g_hInstance = NULL; // Holds The Instance Of The Application

static GLUSboolean (*glusInit)(GLUSvoid) = NULL;
static GLUSvoid (*glusReshape)(GLUSuint width, GLUSuint height) = NULL;
static GLUSboolean (*glusUpdate)(GLUSfloat time) = NULL;
static GLUSvoid (*glusTerminate)(GLUSvoid) = NULL;

static GLUSvoid (*glusKey)(GLUSboolean pressed, GLUSuint key) = NULL;
static GLUSvoid (*glusMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos) = NULL;
static GLUSvoid (*glusMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos) = NULL;
static GLUSvoid (*glusMouseMove)(GLUSuint buttons, GLUSuint xPos, GLUSuint yPos) = NULL;

GLUSvoid GLUSAPIENTRY glusKeyFunc(GLUSvoid(*glusNewKey)(GLUSboolean pressed, GLUSuint key))
{
	glusKey = glusNewKey;
}

GLUSvoid GLUSAPIENTRY glusMouseFunc(GLUSvoid(*glusNewMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos))
{
	glusMouse = glusNewMouse;
}

GLUSvoid GLUSAPIENTRY glusMouseWheelFunc(GLUSvoid(*glusNewMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos))
{
	glusMouseWheel = glusNewMouseWheel;
}

GLUSvoid GLUSAPIENTRY glusMouseMoveFunc(GLUSvoid(*glusNewMouseMove)(GLUSuint buttons, GLUSuint xPos, GLUSuint yPos))
{
	glusMouseMove = glusNewMouseMove;
}

GLUSvoid GLUSAPIENTRY glusInitFunc(GLUSboolean(*glusNewInit)(GLUSvoid))
{
	glusInit = glusNewInit;
}

GLUSvoid GLUSAPIENTRY glusReshapeFunc(GLUSvoid(*glusNewReshape)(GLUSuint width, GLUSuint height))
{
	glusReshape = glusNewReshape;
}

GLUSvoid GLUSAPIENTRY glusUpdateFunc(GLUSboolean(*glusNewUpdate)(GLUSfloat time))
{
	glusUpdate = glusNewUpdate;
}

GLUSvoid GLUSAPIENTRY glusTerminateFunc(GLUSvoid(*glusNewTerminate)(GLUSvoid))
{
	glusTerminate = glusNewTerminate;
}

GLUSvoid GLUSAPIENTRY glusPrepareContext(const GLUSuint major, const GLUSuint minor, const GLUSint flags)
{
	g_major = major;
	g_minor = minor;
	g_flags = flags;
}

GLUSfloat glusGetElapsedTime(GLUSvoid)
{
	static GLUSboolean init = GLUS_FALSE;
	static GLUSfloat lastTime;
	static GLUSfloat currentTime;

	static LONGLONG Frequency = 0;
	LONGLONG Current;

	GLUSfloat time;

	if (Frequency == 0)
	{
		QueryPerformanceFrequency((LARGE_INTEGER*) &Frequency);
	}

	QueryPerformanceCounter((LARGE_INTEGER*) &Current);

	time = (GLUSfloat) Current / (GLUSfloat) Frequency;

	if (!init)
	{
		lastTime = time;

		currentTime = time;

		init = GLUS_TRUE;
	}
	else
	{
		lastTime = currentTime;

		currentTime = time;
	}

	return currentTime - lastTime;
}

GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid)
{
	if (g_fullscreen) // Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0); // If So Switch Back To The Desktop
		ShowCursor(TRUE); // Show Mouse Pointer
	}

	if (g_hRC) // Do We Have A Rendering Context?
	{
		wglMakeCurrent(NULL, NULL); // Release The DC And RC Contexts

		wglDeleteContext(g_hRC); // Delete The RC

		g_hRC = NULL; // Set RC To NULL
	}

	if (g_hDC)
	{
		ReleaseDC(g_hWnd, g_hDC); // Release The DC

		g_hDC = NULL; // Set DC To NULL
	}

	if (g_hWnd)
	{
		DestroyWindow(g_hWnd); // Are We Able To Destroy The Window?

		g_hWnd = NULL; // Set hWnd To NULL
	}

	if (g_hInstance)
	{
		UnregisterClass("GLUS", g_hInstance); // Unregister Class

		g_hInstance = NULL; // Set hInstance To NULL
	}

	g_initdone = GL_FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, // Handle For This Window
UINT uMsg, // Message For This Window
WPARAM wParam, // Additional Message Information
LPARAM lParam) // Additional Message Information
{
	switch (uMsg)
	// Check For Windows Messages
	{
		case WM_ACTIVATE: // Watch For Window Activate Message
		{
			if (!HIWORD(wParam)) // Check Minimization State
			{
				g_active = GLUS_TRUE; // Program Is Active
			}
			else
			{
				g_active = GLUS_FALSE; // Program Is No Longer Active
			}

			return 0; // Return To The Message Loop
		}

		case WM_SYSCOMMAND: // Intercept System Commands
		{
			switch (wParam)
			// Check System Calls
			{
				case SC_SCREENSAVE: // Screensaver Trying To Start?
				case SC_MONITORPOWER: // Monitor Trying To Enter Powersave?
					return 0; // Prevent From Happening
			}
			break; // Exit
		}

		case WM_CLOSE: // Did We Receive A Close Message?
		{
			PostQuitMessage(0); // Send A Quit Message
			return 0; // Jump Back
		}

		case WM_CHAR:
		{
			if (glusKey && g_initdone)
			{
				glusKey(g_keydown, wParam);
			}
			return 0; // Jump Back
		}

		case WM_KEYDOWN: // Is A Key Being Held Down?
		{
			if (wParam == VK_ESCAPE)
			{
				PostQuitMessage(0); // Send A Quit Message
			}
			else
			{
				g_keydown = GLUS_TRUE;
			}
			return 0; // Jump Back
		}

		case WM_KEYUP:
		{
			if (wParam == VK_ESCAPE)
			{
				// Do nothing
			}
			else
			{
				g_keydown = GLUS_FALSE;
			}
			return 0; // Jump Back
		}

		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
		{
			if (glusMouse && g_initdone)
			{
				GLUSuint button = 0;

				if (uMsg == WM_LBUTTONDOWN)
				{
					button = 1;
				}
				else if (uMsg == WM_MBUTTONDOWN)
				{
					button = 2;
				}
				else if (uMsg == WM_RBUTTONDOWN)
				{
					button = 4;
				}

				glusMouse(GLUS_TRUE, button, LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
		}

		case WM_LBUTTONUP:
		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
		{
			if (glusMouse && g_initdone)
			{
				GLUSuint button = 0;

				if (uMsg == WM_LBUTTONUP)
				{
					button = 1;
				}
				else if (uMsg == WM_MBUTTONUP)
				{
					button = 2;
				}
				else if (uMsg == WM_RBUTTONUP)
				{
					button = 4;
				}

				glusMouse(GLUS_FALSE, button, LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			if (glusMouseMove && g_initdone)
			{
				GLUSuint buttons = 0;

				if (LOWORD(wParam) & MK_LBUTTON)
				{
					buttons |= 1;
				}
				if (LOWORD(wParam) & MK_MBUTTON)
				{
					buttons |= 2;
				}
				if (LOWORD(wParam) & MK_RBUTTON)
				{
					buttons |= 4;
				}

				glusMouseMove(buttons, LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			if (glusMouseWheel && g_initdone)
			{
				GLUSuint buttons = 0;

				if (LOWORD(wParam) & MK_LBUTTON)
				{
					buttons |= 1;
				}
				if (LOWORD(wParam) & MK_MBUTTON)
				{
					buttons |= 2;
				}
				if (LOWORD(wParam) & MK_RBUTTON)
				{
					buttons |= 4;
				}

				glusMouseWheel(buttons, GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA, LOWORD(lParam), HIWORD(lParam));
			}
			return 0;
		}

		case WM_SIZE: // Resize The OpenGL Window
		{
			g_width = LOWORD(lParam); // LoWord=Width, HiWord=Height
			g_height = HIWORD(lParam);

			if (glusReshape && g_initdone)
			{
				glusReshape(g_width, g_height);
			}
			return 0; // Jump Back
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

GLUSboolean GLUSAPIENTRY glusCreateWindowX(const char* title, const GLUSuint width, const GLUSuint height, const GLUSuint depthBits, const GLUSuint stencilBits, const GLUSboolean fullscreen)
{
	GLUSenum err;

	GLUSint attribList[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 3, WGL_CONTEXT_MINOR_VERSION_ARB, 0, WGL_CONTEXT_FLAGS_ARB, 0, 0 };

	PIXELFORMATDESCRIPTOR pfd = // pfd Tells Windows How We Want Things To Be
			{ sizeof(PIXELFORMATDESCRIPTOR), // Size Of This Pixel Format Descriptor
			1, // Version Number
			PFD_DRAW_TO_WINDOW | // Format Must Support Window
					PFD_SUPPORT_OPENGL | // Format Must Support OpenGL
					PFD_DOUBLEBUFFER, // Must Support Double Buffering
			PFD_TYPE_RGBA, // Request An RGBA Format
			32, // Select Our Color Depth
			0, 0, 0, 0, 0, 0, // Color Bits Ignored
			0, // No Alpha Buffer
			0, // Shift Bit Ignored
			0, // No Accumulation Buffer
			0, 0, 0, 0, // Accumulation Bits Ignored
			depthBits, // 16Bit Z-Buffer (Depth Buffer)
			stencilBits, // No Stencil Buffer
			0, // No Auxiliary Buffer
			PFD_MAIN_PLANE, // Main Drawing Layer
			0, // Reserved
			0, 0, 0 // Layer Masks Ignored
			};

	GLUSuint PixelFormat; // Holds The Results After Searching For A Match
	WNDCLASS wc; // Windows Class Structure
	DWORD dwExStyle; // Window Extended Style
	DWORD dwStyle; // Window Style
	RECT WindowRect; // Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long) 0; // Set Left Value To 0
	WindowRect.right = (long) width; // Set Right Value To Requested Width
	WindowRect.top = (long) 0; // Set Top Value To 0
	WindowRect.bottom = (long) height; // Set Bottom Value To Requested Height

	g_fullscreen = fullscreen; // Set The Global Fullscreen Flag

	g_hInstance = GetModuleHandle(NULL); // Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC) WndProc; // WndProc Handles Messages
	wc.cbClsExtra = 0; // No Extra Window Data
	wc.cbWndExtra = 0; // No Extra Window Data
	wc.hInstance = g_hInstance; // Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW); // Load The Arrow Pointer
	wc.hbrBackground = NULL; // No Background Required For GL
	wc.lpszMenuName = NULL; // We Don't Want A Menu
	wc.lpszClassName = "GLUS"; // Set The Class Name

	if (!RegisterClass(&wc)) // Attempt To Register The Window Class
	{
		return GLUS_FALSE; // Return FALSE
	}

	if (fullscreen) // Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings; // Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings)); // Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings); // Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width; // Selected Screen Width
		dmScreenSettings.dmPelsHeight = height; // Selected Screen Height
		dmScreenSettings.dmBitsPerPel = 32; // Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			glusDestroyWindow();
			return GLUS_FALSE;
		}
	}

	if (fullscreen)
	{
		dwExStyle = WS_EX_APPWINDOW; // Window Extended Style
		dwStyle = WS_POPUP; // Windows Style
		ShowCursor(FALSE); // Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW; // Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle); // Adjust Window To True Requested Size

	// Create The Window
	if (!(g_hWnd = CreateWindowEx(dwExStyle, // Extended Style For The Window
	"GLUS", // Class Name
	title, // Window Title
	dwStyle | // Defined Window Style
			WS_CLIPSIBLINGS | // Required Window Style
			WS_CLIPCHILDREN, // Required Window Style
	0, 0, // Window Position
	WindowRect.right - WindowRect.left, // Calculate Window Width
	WindowRect.bottom - WindowRect.top, // Calculate Window Height
	NULL, // No Parent Window
	NULL, // No Menu
	g_hInstance, // Instance
	NULL))) // Dont Pass Anything To WM_CREATE
	{
		glusDestroyWindow();
		return GLUS_FALSE;
	}

	if (!(g_hDC = GetDC(g_hWnd))) // Did We Get A Device Context?
	{
		glusDestroyWindow();
		return GLUS_FALSE; // Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(g_hDC, &pfd))) // Did Windows Find A Matching Pixel Format?
	{
		glusDestroyWindow();
		return GLUS_FALSE; // Return FALSE
	}

	if (!SetPixelFormat(g_hDC, PixelFormat, &pfd)) // Are We Able To Set The Pixel Format?
	{
		glusDestroyWindow();
		return GLUS_FALSE; // Return FALSE
	}

	if (!(g_hRC = wglCreateContext(g_hDC))) // Are We Able To Get A Rendering Context?
	{
		glusDestroyWindow();
		return GLUS_FALSE; // Return FALSE
	}

	if (!wglMakeCurrent(g_hDC, g_hRC)) // Try To Activate The Rendering Context
	{
		glusDestroyWindow();
		return GLUS_FALSE; // Return FALSE
	}

	if (g_major >= 3)
	{
		PFNWGLCREATECONTEXTATTRIBSARBPROCTEMP wglCreateContextAttribsARBTemp = NULL;
		HGLRC hRCTemp = NULL;

		GLUSint attribList[] = { WGL_CONTEXT_MAJOR_VERSION_ARB, 1, WGL_CONTEXT_MINOR_VERSION_ARB, 0, WGL_CONTEXT_FLAGS_ARB, 0, 0 };

		attribList[1] = g_major;
		attribList[3] = g_minor;
		attribList[5] = g_flags;

		if (!(wglCreateContextAttribsARBTemp = (PFNWGLCREATECONTEXTATTRIBSARBPROCTEMP) wglGetProcAddress("wglCreateContextAttribsARB")))
		{
			glusDestroyWindow();
			return GLUS_FALSE; // Return FALSE
		}

		if (!(hRCTemp = wglCreateContextAttribsARBTemp(g_hDC, 0, attribList)))
		{
			glusDestroyWindow();
			return GLUS_FALSE; // Return FALSE
		}

		if (!wglMakeCurrent(NULL, NULL))
		{
			wglDeleteContext(hRCTemp);

			glusDestroyWindow();
			return GLUS_FALSE; // Return FALSE
		}

		if (!wglDeleteContext(g_hRC))
		{
			wglDeleteContext(hRCTemp);

			glusDestroyWindow();
			return GLUS_FALSE; // Return FALSE
		}

		g_hRC = hRCTemp;

		if (!wglMakeCurrent(g_hDC, g_hRC))
		{
			glusDestroyWindow();
			return GLUS_FALSE; // Return FALSE
		}
	}

	ShowWindow(g_hWnd, SW_SHOW); // Show The Window
	SetForegroundWindow(g_hWnd); // Slightly Higher Priority
	SetFocus(g_hWnd); // Sets Keyboard Focus To The Window

	glewExperimental = GL_TRUE;

	err = glewInit();

	if (GLEW_OK != err)
	{
		glusDestroyWindow();

		return GLUS_FALSE;
	}

	if (!glusIsSupported(g_major, g_minor))
	{
		glusDestroyWindow();

		printf("OpenGL %u.%u not supported.\n", g_major, g_minor);

		return GLUS_FALSE;
	}

	g_width = width;
	g_height = height;

	return GLUS_TRUE; // Success
}

GLUSboolean GLUSAPIENTRY glusCreateWindow(const char* title, const GLUSuint width, const GLUSuint height, const GLUSboolean fullscreen)
{
    return glusCreateWindowX(title, width, height, 16, 0, fullscreen);
}

GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid)
{
	MSG msg; // Windows Message Structure

	GLUSboolean done = GLUS_FALSE; // Bool Variable To Exit Loop

	// Init Engine
	if (glusInit)
	{
		if (!glusInit())
		{
			glusDestroyWindow(); // Destroy The Window

			return GLUS_FALSE; // Exit The Program
		}
	}

	g_initdone = GLUS_TRUE;

	// Do the first reshape
	if (glusReshape)
	{
		glusReshape(g_width, g_height);
	}

	while (!done) // Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT) // Have We Received A Quit Message?
			{
				done = GLUS_TRUE; // If So done=TRUE
			}
			else // If Not, Deal With Window Messages
			{
				TranslateMessage(&msg); // Translate The Message
				DispatchMessage(&msg); // Dispatch The Message
			}
		}

		// Draw The Scene.  Watch For ESC Key
		if (g_active && !done) // Program Active?
		{
			if (glusUpdate)
			{
				done = !glusUpdate(glusGetElapsedTime());
			}

			if (!done)
			{
				SwapBuffers(g_hDC); // Swap Buffers (Double Buffering)
			}
		}
	}

	// Terminate Game
	if (glusTerminate)
	{
		glusTerminate();
	}

	// Shutdown
	glusDestroyWindow(); // Destroy The Window

	return GLUS_TRUE; // Exit The Program
}
