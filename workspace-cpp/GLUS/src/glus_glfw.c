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

#include <stdio.h>
#include <string.h>

#include "GL/glus.h"

#include <GL/glfw.h>

static GLUSuint g_major = 1;
static GLUSuint g_minor = 0;
static GLUSint g_flags = 0;

static GLUSuint g_numberSamples = 0;

static GLUSboolean g_initdone = GLUS_FALSE;
static GLUSboolean g_done = GLUS_FALSE;
static GLUSuint g_buttons = 0;
static GLUSuint g_mouseX = 0;
static GLUSuint g_mouseY = 0;

static GLUSuint g_width = 640;
static GLUSuint g_height = 480;

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

GLUSvoid GLUSAPIENTRY glusPrepareContext(GLUSuint major, GLUSuint minor, GLUSint flags)
{
    g_major = major;
    g_minor = minor;
    g_flags = flags;
}

GLUSvoid GLUSAPIENTRY glusPrepareMSAA(const GLUSuint numberSamples)
{
    g_numberSamples = numberSamples;
}

GLUSfloat glusGetElapsedTime(GLUSvoid)
{
    static GLUSboolean init = GLUS_FALSE;
    static GLUSfloat lastTime;
    static GLUSfloat currentTime;

    GLUSfloat measuredTime;

    measuredTime = (GLUSfloat) glfwGetTime();

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

GLUSvoid glusInternalReshape(GLUSint width, GLUSint height)
{
    if (glusReshape && g_initdone)
    {
        glusReshape(width, height);
    }
}

GLUSint glusInternalClose()
{
    g_done = GLUS_TRUE;

    return GLUS_FALSE;
}

GLUSvoid glusInternalChar(GLUSint key, GLUSint state)
{
    if (state == 0)
    {
        if (glusKey)
        {
            glusKey(GLUS_FALSE, key);
        }
    }
    else
    {
        if (glusKey)
        {
            glusKey(GLUS_TRUE, key);
        }
    }
}

GLUSvoid glusInternalKey(GLUSint key, GLUSint state)
{
    if (state == 0)
    {
        if (key == 257)
        {
            g_done = GLUS_TRUE;
        }
    }
}

GLUSvoid glusInternalMouse(GLUSint button, GLUSint action)
{
    GLUSuint usedButton = 0;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        usedButton = 1;
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
    {
        usedButton = 2;
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        usedButton = 4;
    }

    if (action == GLFW_PRESS)
    {
        g_buttons |= usedButton;
    }
    else
    {
        g_buttons ^= usedButton;
    }

    if (glusMouse)
    {
        glusMouse(action == GLFW_PRESS, usedButton, g_mouseX, g_mouseY);
    }
}

GLUSvoid glusInternalMouseWheel(GLUSint pos)
{
    if (glusMouseWheel)
    {
        glusMouseWheel(g_buttons, pos, g_mouseX, g_mouseY);
    }
}

GLUSvoid glusInternalMouseMove(GLUSint x, GLUSint y)
{
    g_mouseX = (GLUSuint) x;
    g_mouseY = (GLUSuint) y;

    if (glusMouseMove)
    {
        glusMouseMove(g_buttons, g_mouseX, g_mouseY);
    }
}

GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid)
{
    glfwTerminate();

    g_initdone = GLUS_FALSE;
}

GLUSboolean GLUSAPIENTRY glusCreateWindow(const char* title, GLUSuint width, GLUSuint height, const GLUSuint depthBits, const GLUSuint stencilBits, GLUSboolean fullscreen)
{
#ifndef __APPLE__
    GLUSenum err;
#endif

    if (!glfwInit())
    {
        return GLUS_FALSE;
    }

    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, g_major);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, g_minor);
    glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, g_flags & GLUS_FORWARD_COMPATIBLE_BIT);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, (g_flags & GLUS_FORWARD_COMPATIBLE_BIT) ? GLFW_OPENGL_CORE_PROFILE : GLFW_OPENGL_COMPAT_PROFILE);
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, g_numberSamples);

    if (!glfwOpenWindow(width, height, 8, 8, 8, 8, depthBits, stencilBits, fullscreen ? GLFW_FULLSCREEN : GLFW_WINDOW))
    {
        return GLUS_FALSE;
    }
    glfwSetWindowTitle(title);

#ifndef __APPLE__
    glewExperimental = GLUS_TRUE;

    err = glewInit();

    if (GLUS_OK != err)
    {
        glusDestroyWindow();

        return GLUS_FALSE;
    }
#endif

    if (!glusIsSupported(g_major, g_minor))
    {
        glusDestroyWindow();

        printf("OpenGL %u.%u not supported.\n", g_major, g_minor);

        return GLUS_FALSE;
    }

    glfwSetWindowSizeCallback(glusInternalReshape);
    glfwSetWindowCloseCallback(glusInternalClose);
    glfwSetCharCallback(glusInternalChar);
    glfwSetKeyCallback(glusInternalKey);
    glfwSetMouseButtonCallback(glusInternalMouse);
    glfwSetMouseWheelCallback(glusInternalMouseWheel);
    glfwSetMousePosCallback(glusInternalMouseMove);

    g_width = width;
    g_height = height;

    return GLUS_TRUE; // Success
}

GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid)
{
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

    while (!g_done) // Loop That Runs While done=FALSE
    {
		if (glusUpdate)
		{
			g_done = !glusUpdate(glusGetElapsedTime());
		}

		glfwSwapBuffers(); // Swap Buffers (Double Buffering)
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
