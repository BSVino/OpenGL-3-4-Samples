/**
 * OpenGL 3 - Example 03
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
 * The used shader program.
 */
GLUSshaderprogram g_program;

/**
 * Projection matrix will be calculated later.
 */
GLfloat g_projection[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelView[16];
/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionLocation;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewLocation;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vao;

/**
 * The VBO for the vertices.
 */
GLuint g_vertices;

/**
 * The VBO for the normals.
 */
GLuint g_normals;

/**
 * The VBO for the indices.
 */
GLuint g_indices;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocation;

/**
 * The location of the normal in the shader program.
 */
GLint g_normalLocation;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// Points of a cube.
	GLfloat
			points[] =
					{ -0.5f, -0.5f, -0.5f, 1.0f, -0.5f, -0.5f, +0.5f, 1.0f, +0.5f, -0.5f, +0.5f, 1.0f, +0.5f, -0.5f, -0.5f, 1.0f, -0.5f, +0.5f, -0.5f, 1.0f, -0.5f, +0.5f, +0.5f, 1.0f, +0.5f, +0.5f, +0.5f, 1.0f, +0.5f, +0.5f, -0.5f, 1.0f, -0.5f, -0.5f, -0.5f, 1.0f, -0.5f, +0.5f, -0.5f, 1.0f, +0.5f, +0.5f, -0.5f, 1.0f, +0.5f, -0.5f, -0.5f, 1.0f, -0.5f, -0.5f, +0.5f, 1.0f, -0.5f, +0.5f, +0.5f, 1.0f, +0.5f, +0.5f, +0.5f, 1.0f, +0.5f, -0.5f, +0.5f, 1.0f, -0.5f, -0.5f, -0.5f, 1.0f, -0.5f, -0.5f, +0.5f, 1.0f, -0.5f, +0.5f, +0.5f, 1.0f, -0.5f, +0.5f, -0.5f, 1.0f, +0.5f, -0.5f, -0.5f, 1.0f, +0.5f, -0.5f, +0.5f, 1.0f, +0.5f, +0.5f, +0.5f, 1.0f, +0.5f, +0.5f, -0.5f, 1.0f };

	// Normals of a cube.
	GLfloat
			normals[] =
					{ +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, -1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f, +1.0f, +0.0f, +0.0f };

	// The associated indices.
	GLuint indices[] = { 0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20, 23, 22, 20, 22, 21 };

	// Matrix for the model
	GLfloat model[16];

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example03/Vertex.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example03/Fragment.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// ToDo:
	glGenVertexArrays(1, &g_vao);

	// ToDo:
	glBindVertexArray(g_vao);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_projectionLocation = glGetUniformLocation(g_program.program, "projectionMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_modelViewLocation = glGetUniformLocation(g_program.program, "modelViewMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocation = glGetAttribLocation(g_program.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_normalLocation = glGetAttribLocation(g_program.program, "normal");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, 24 * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(GLfloat), (GLfloat*) normals, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 2 * 3 * sizeof(GLuint), (GLuint*) indices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	glusRotateRzRyRxf(model, 30.0f, 30.0f, 0.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelView, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelView, g_modelView, model);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_normalLocation);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_DEPTH_TEST);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_CULL_FACE);

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

	// Calculate the projection matrix and set it
	glusPerspectivef(g_projection, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_projectionLocation, 1, GL_FALSE, g_projection);
}

/**
 * Function to render and display content.
 */
GLUSboolean update(GLUSfloat time)
{
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, 6 * 2 * 3, GL_UNSIGNED_INT, 0);

	return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminate(GLUSvoid)
{
	if (g_vertices)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_vertices);

		g_vertices = 0;
	}

	if (g_normals)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_normals);

		g_normals = 0;
	}

	if (g_indices)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_indices);

		g_indices = 0;
	}

	// ToDo:
	glDeleteVertexArrays(1, &g_vao);

	// Delete shader program etc..
	glusDestroyProgram(&g_program);
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
