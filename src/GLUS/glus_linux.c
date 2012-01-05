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

/**
 * Additional code by Alexandr Palamar
 */

#include <X11/X.h>

#include <time.h>

#include "GL/glus.h"

#include <GL/gl.h>
#include <GL/glx.h>

#include <stdio.h>
#include <string.h>

#ifndef GLX_CONTEXT_DEBUG_BIT_ARB
#define GLX_CONTEXT_DEBUG_BIT_ARB 0x0001
#endif
#ifndef GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#endif
#ifndef GLX_CONTEXT_MAJOR_VERSION_ARB
#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#endif
#ifndef GLX_CONTEXT_MINOR_VERSION_ARB
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#endif
#ifndef GLX_CONTEXT_FLAGS_ARB
#define GLX_CONTEXT_FLAGS_ARB 0x2094
#endif

typedef GLXContext (* PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP)(Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

static GLUSuint g_major = 1;
static GLUSuint g_minor = 0;
static GLUSint g_flags = 0;

static GLUSboolean g_fullscreen = GL_FALSE;
static GLUSboolean g_active = GL_FALSE;
static GLUSboolean g_initdone = GL_FALSE;

static GLUSuint g_width = 640;
static GLUSuint g_height = 480;

static Display* g_Display = None;
static Window g_Window = 0;
static GLXContext g_Context = None;
static Atom g_DeleteMessage;
static Atom g_StateMessage;
static Atom g_FullscreenMessage;

static GLUSboolean (*glusInit)(GLUSvoid) = None;
static GLUSvoid (*glusReshape)(GLUSuint width, GLUSuint height) = None;
static GLUSboolean (*glusUpdate)(GLUSfloat time) = None;
static GLUSvoid (*glusTerminate)(GLUSvoid) = None;

static GLUSvoid (*glusKey)(GLUSboolean pressed, GLUSuint key) = NULL;
static GLUSvoid (*glusMouse)(GLUSboolean pressed, GLUSuint button, GLUSuint xPos, GLUSuint yPos) = NULL;
static GLUSvoid (*glusMouseWheel)(GLUSuint buttons, GLUSint ticks, GLUSuint xPos, GLUSuint yPos) = NULL;
static GLUSvoid
(*glusMouseMove)(GLUSuint buttons, GLUSuint xPos, GLUSuint yPos) = NULL;

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

GLUSvoid GLUSAPIENTRY glusPrepareContext(GLUSuint major, GLUSuint minor, GLUSint flags)
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

	static struct timespec initial;

	GLUSfloat measuredTime;

	struct timespec now;

	if (!init)
	{
		clock_gettime(CLOCK_REALTIME, &initial);
	}

	clock_gettime(CLOCK_REALTIME, &now);

	measuredTime = (GLUSfloat) (now.tv_sec - initial.tv_sec) + (GLUSfloat) now.tv_nsec / 1000000000.0f;

	if (!init)
	{
		lastTime = measuredTime;

		currentTime = measuredTime;

		init = GLUS_TRUE;
	}
	else
	{
		lastTime = currentTime;

		currentTime = measuredTime;
	}

	return currentTime - lastTime;
}

GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid)
{
	if (g_fullscreen && g_Display && g_Window) // Are We In Fullscreen Mode?
	{
		XEvent xev;

		memset(&xev, 0, sizeof(xev));

		XUndefineCursor(g_Display, g_Window);

		xev.xclient.type = ClientMessage;
		xev.xclient.window = g_Window;
		xev.xclient.message_type = g_StateMessage;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 0;
		xev.xclient.data.l[1] = g_FullscreenMessage;

		XSendEvent(g_Display, DefaultRootWindow(g_Display), False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);
	}

	if (g_Display)
	{
		glXMakeCurrent(g_Display, None, None);
	}

	if (g_Display && g_Context)
	{
		glXDestroyContext(g_Display, g_Context);

		g_Context = None;
	}

	if (g_Display && g_Window)
	{
		XDestroyWindow(g_Display, g_Window);
	}

	if (g_Display)
	{
		XCloseDisplay(g_Display);

		g_Display = None;
	}

	g_initdone = GL_FALSE;
}

GLUSboolean GLUSAPIENTRY glusCreateWindowX(const char* title, GLUSuint width, GLUSuint height, const GLUSuint depthBits, const GLUSuint stencilBits, GLUSboolean fullscreen)
{
	GLUSenum err;
	Window RootWindow = 0;
	XVisualInfo* VisualInfo = None;
	Colormap CurrentColorMap = 0;
	XSetWindowAttributes CurrentSetWindowAttibutes;
	GLUSint fbcCount = 0;
	GLXFBConfig* fbc = None;

	GLUSint
			visualAttribList[] =
					{ GLX_X_RENDERABLE, True, GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT, GLX_RENDER_TYPE, GLX_RGBA_BIT, GLX_X_VISUAL_TYPE, GLX_TRUE_COLOR, GLX_RED_SIZE, 8, GLX_GREEN_SIZE, 8, GLX_BLUE_SIZE, 8, GLX_ALPHA_SIZE, 8, GLX_DEPTH_SIZE, depthBits, GLX_STENCIL_SIZE, stencilBits, GLX_DOUBLEBUFFER, True, None };

	g_Display = XOpenDisplay(None);

	if (g_Display == None)
	{
		return GLUS_FALSE;
	}

	RootWindow = DefaultRootWindow(g_Display);

	if (!(fbc = glXChooseFBConfig(g_Display, DefaultScreen(g_Display), visualAttribList, &fbcCount)))
	{
		glusDestroyWindow();

		return GLUS_FALSE;
	}

	VisualInfo = glXGetVisualFromFBConfig(g_Display, fbc[0]);

	if (VisualInfo == None)
	{
		XFree(fbc);

		glusDestroyWindow();

		return GLUS_FALSE;
	}

	CurrentColorMap = XCreateColormap(g_Display, RootWindow, VisualInfo->visual, AllocNone);

	CurrentSetWindowAttibutes.colormap = CurrentColorMap;
	CurrentSetWindowAttibutes.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

	g_Window
			= XCreateWindow(g_Display, RootWindow, 0, 0, width, height, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &CurrentSetWindowAttibutes);

	if (g_Window == 0)
	{
		XFree(fbc);

		glusDestroyWindow();

		return GLUS_FALSE;
	}

	g_DeleteMessage = XInternAtom(g_Display, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(g_Display, g_Window, &g_DeleteMessage, 1);

	XMapWindow(g_Display, g_Window);

	XStoreName(g_Display, g_Window, title);

	if (fullscreen)
	{
		XEvent xev;

		Cursor InvisibleCursor = None;
		char cursorData[32];
		XColor cursorColor;
		Pixmap cursorPixmap;

		memset(cursorData, 0, sizeof(cursorData));

		memset(&cursorColor, 0, sizeof(cursorColor));

		cursorPixmap = XCreateBitmapFromData(g_Display, RootWindow, cursorData, 16, 16);

		if (cursorPixmap != None)
		{
			InvisibleCursor = XCreatePixmapCursor(g_Display, cursorPixmap, cursorPixmap, &cursorColor, &cursorColor, 0, 0);

			XFreePixmap(g_Display, cursorPixmap);

			if (InvisibleCursor != None)
			{
				XDefineCursor(g_Display, g_Window, InvisibleCursor);
			}
		}

		//

		g_StateMessage = XInternAtom(g_Display, "_NET_WM_STATE", False);
		g_FullscreenMessage = XInternAtom(g_Display, "_NET_WM_STATE_FULLSCREEN", False);

		memset(&xev, 0, sizeof(xev));

		xev.xclient.type = ClientMessage;
		xev.xclient.window = g_Window;
		xev.xclient.message_type = g_StateMessage;
		xev.xclient.format = 32;
		xev.xclient.data.l[0] = 1;
		xev.xclient.data.l[1] = g_FullscreenMessage;

		XSendEvent(g_Display, RootWindow, False, SubstructureRedirectMask | SubstructureNotifyMask, &xev);

		g_fullscreen = fullscreen;
	}

	g_Context = glXCreateContext(g_Display, VisualInfo, None, GLUS_TRUE);

	if (g_Context == None)
	{
		XFree(fbc);

		glusDestroyWindow();

		return GLUS_FALSE;
	}

	if (!glXMakeCurrent(g_Display, g_Window, g_Context))
	{
		XFree(fbc);

		glusDestroyWindow();

		return GLUS_FALSE;
	}

	if (g_major >= 3)
	{
		GLXContext Context = None;
		PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP glXCreateContextAttribsARBTemp = None;

		GLUSint attribList[] = { GLX_CONTEXT_MAJOR_VERSION_ARB, 1, GLX_CONTEXT_MINOR_VERSION_ARB, 0, GLX_CONTEXT_FLAGS_ARB, 0, 0 };

		attribList[1] = g_major;
		attribList[3] = g_minor;
		attribList[5] = g_flags;

		if (!(glXCreateContextAttribsARBTemp = (PFNGLXCREATECONTEXTATTRIBSARBPROCTEMP) glXGetProcAddress((const GLUSubyte *) "glXCreateContextAttribsARB")))
		{
			XFree(fbc);

			glusDestroyWindow();

			return GLUS_FALSE; // Return FALSE
		}

		if (!(Context = glXCreateContextAttribsARBTemp(g_Display, fbc[0], 0, True, attribList)))
		{
			XFree(fbc);

			glusDestroyWindow();

			return GLUS_FALSE; // Return FALSE
		}
		XFree(fbc);

		if (!glXMakeCurrent(g_Display, None, None))
		{
			glXDestroyContext(g_Display, Context);

			glusDestroyWindow();

			return GLUS_FALSE; // Return FALSE
		}

		glXDestroyContext(g_Display, g_Context);

		g_Context = Context;

		if (!glXMakeCurrent(g_Display, g_Window, g_Context))
		{
			glusDestroyWindow();

			return GLUS_FALSE;
		}
	}

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
	//MSG		msg;									// Windows Message Structure
	XEvent msg;

	GLUSboolean done = GLUS_FALSE; // Bool Variable To Exit Loop

	GLUSuint mouseButtons = 0;

	GLUSint mouseX, mouseY, mousePrevX, mousePrevY, dummy;

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

	XQueryPointer(g_Display, g_Window, (Window *) &dummy, (Window *) &dummy, (GLUSint*) &dummy, (GLUSint*) &dummy, &mousePrevX, &mousePrevY, (GLUSuint*) &dummy);

	while (!done) // Loop That Runs While done=FALSE
	{
		if (XPending(g_Display))
		{
			XNextEvent(g_Display, &msg);

			if (msg.type == KeyPress)
			{
				if (msg.xkey.keycode == 0x09) // Have We Received the Esc Key
				{
					// do nothing
				}
				else
				{
					if (glusKey && g_initdone)
					{
						char c;

						if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
						{
							glusKey(GLUS_TRUE, c);
						}
					}
				}
			}
			else if (msg.type == KeyRelease)
			{
				if (msg.xkey.keycode == 0x09) // Have We Received the Esc Key
				{
					done = GLUS_TRUE; // If So done=TRUE
				}
				else
				{
					if (glusKey && g_initdone)
					{
						char c;

						if (XLookupString(&msg.xkey, &c, 1, 0, 0) > 0)
						{
							glusKey(GLUS_FALSE, c);
						}
					}
				}
			}
			else if (msg.type == ButtonPress)
			{
				if (glusMouse && g_initdone)
				{
					if (msg.xbutton.button <= 3)
					{
						mouseButtons |= 1 << (msg.xbutton.button - 1);

						glusMouse(GLUS_TRUE, msg.xbutton.button, msg.xbutton.x, msg.xbutton.y);
					}
					else if (msg.xbutton.button == 4)
					{
						glusMouseWheel(mouseButtons, 1, msg.xbutton.x, msg.xbutton.y);
					}
					else if (msg.xbutton.button == 5)
					{
						glusMouseWheel(mouseButtons, -1, msg.xbutton.x, msg.xbutton.y);
					}
				}
			}
			else if (msg.type == ButtonRelease)
			{
				if (glusMouse && g_initdone)
				{
					if (msg.xbutton.button <= 3)
					{
						mouseButtons ^= 1 << (msg.xbutton.button - 1);

						glusMouse(GLUS_FALSE, msg.xbutton.button, msg.xbutton.x, msg.xbutton.y);
					}
				}
			}
			else if (msg.type == ClientMessage)
			{
				if (msg.xclient.data.l[0] == g_DeleteMessage) // Have We Received a Delete Message
				{
					done = GLUS_TRUE; // If So done=TRUE
				}
			}
			else if (msg.type == ConfigureNotify)
			{
				g_width = msg.xconfigure.width;
				g_height = msg.xconfigure.height;

				if (glusReshape && g_initdone)
				{
					glusReshape(g_width, g_height);
				}
			}
			else if (msg.type == MapNotify)
			{
				g_active = GLUS_TRUE;
			}
			else if (msg.type == UnmapNotify)
			{
				g_active = GLUS_FALSE;
			}
		}

		// Draw The Scene.
		if (g_active && !done) // Program Active?
		{
			if (glusUpdate)
			{
				if (glusMouseMove && g_initdone)
				{
					XQueryPointer(g_Display, g_Window, (Window *) &dummy, (Window *) &dummy, (GLUSint*) &dummy, (GLUSint*) &dummy, &mouseX, &mouseY, (GLUSuint*) &dummy);

					if (mouseX != mousePrevX || mouseY != mousePrevY && (mouseX >= 0 && mouseX <= g_width && mouseY >= 0 && mouseY <= g_height))
					{
						mousePrevX = mouseX;
						mousePrevY = mouseY;

						glusMouseMove(mouseButtons, mouseX, mouseY);
					}
				}

				done = !glusUpdate(glusGetElapsedTime());
			}

			if (!done)
			{
				glXSwapBuffers(g_Display, g_Window); // Swap Buffers (Double Buffering)
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
