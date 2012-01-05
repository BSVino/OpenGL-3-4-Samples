/**
 * OpenGL 4 - Example 12
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
 * Needed for cosf
 */
#include <math.h>

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
 * The VBO for the indices.
 */
GLuint g_indices;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocation;

/**
 * The used plane
 */
GLUSshape plane;

/**
 * The indices for the patch
 */
GLUSuint plane_indices[] = { 0, 1, 3, 2 };

/**
 * The number of indices
 */
GLUSuint numberIndices = 4;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	GLUStextfile vertexSource;

	GLUStextfile controlSource;

	GLUStextfile evaluationSource;

	GLUStextfile geometrySource;

	GLUStextfile fragmentSource;

	glusCreatePlanef(&plane, 1.0f);

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example12/Vertex.vs", &vertexSource);

	// Load the source of the control shader.
	glusLoadTextFile("../src/Example12/Control.tcs", &controlSource);

	// Load the source of the evaluation shader.
	glusLoadTextFile("../src/Example12/Evaluation.tes", &evaluationSource);

	// Load the source of the geometry shader.
	glusLoadTextFile("../src/Example12/Geometry.gs", &geometrySource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example12/Fragment.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_program, (const GLUSchar**) &vertexSource.text, (const GLUSchar**) &controlSource.text, (const GLUSchar**) &evaluationSource.text, (const GLUSchar**) &geometrySource.text, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&controlSource);

	// Destroy the text resource
	glusDestroyTextFile(&evaluationSource);

	// Destroy the text resource
	glusDestroyTextFile(&geometrySource);

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

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) plane.vertices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberIndices * sizeof(GLuint), (GLuint*) plane_indices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// Calculate the view matrix ...
	glusLookAtf(g_modelView, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocation);

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

	// http://www.opengl.org/sdk/docs/man4/xhtml/glPatchParameter.xml
	glPatchParameteri(GL_PATCH_VERTICES, 4);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_PATCHES, numberIndices, GL_UNSIGNED_INT, 0);

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

	glusPrepareContext(4, 1, GLUS_FORWARD_COMPATIBLE_BIT);

	if (!glusCreateWindow("GLUS Example Window", 640, 480, GLUS_FALSE))
	{
		printf("Could not create window!\n");
		return -1;
	}

	glusRun();

	return 0;
}
