/**
 * OpenGL 3 - Example 01
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

/**
 * Needed for printf
 */
#include <stdio.h>

#include "GL/glus.h"

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	return GLUS_TRUE;
}

/**
 * Function is called, if the window is resized.
 *
 * @param w	width of the window
 * @param h	height of the window
 */
GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, width, height);
}

/**
 * Function to render and display content.
 */
GLUSboolean update(GLUSfloat time)
{
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
	glusInitFunc(init);

	glusReshapeFunc(reshape);

	glusUpdateFunc(update);

	glusTerminateFunc(terminate);

	glusPrepareContext(3, 2, GLUS_FORWARD_COMPATIBLE_BIT);

	if (!glusCreateWindow("GLUS Example Window", 640, 480, GLUS_FALSE))
	{
		printf("Could not create window!\n");
		return -1;
	}

	glusRun();

	return 0;
}
