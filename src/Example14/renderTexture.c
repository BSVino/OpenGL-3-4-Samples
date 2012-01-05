/**
 * OpenGL 4.1 with GLEW - Example 14
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

/**
 * GLEW is used to retrieve all OpenGL 4.1 functions.
 */
#include <GL/glew.h>

/**
 * Needed for printf
 */
#include <stdio.h>

/**
 * Needed for memset
 */
#include <string.h>

#include "GL/glus.h"

#include "renderTexture.h"

/**
 * The number of waves. Also has to be changed in the shader.
 */
#define NUMBERWAVES 4

/**
 * Size of the normal map texture.
 */
const GLuint TEXTURE_SIZE = 1024;

/**
 * The used shader program.
 */
GLUSshaderprogram g_programTexture;

/**
 * Projection matrix will be calculated later.
 */
GLfloat g_projectionTexture[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelViewTexture[16];

/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionLocationTexture;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewLocationTexture;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vaoTexture;

/**
 * The VBO for the vertices.
 */
GLuint g_verticesTexture;

/**
 * The VBO for the texture coordinates.
 */
GLuint g_texCoordsTexture;

/**
 * The VBO for the indices.
 */
GLuint g_indicesTexture;

/**
 * The texture.
 */
GLuint g_textureTexture;

/**
 * The depth texture.
 */
GLuint g_depthTexture;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocationTexture;

/**
 * The location of the texture coordinate in the shader program.
 */
GLint g_texCoordLocationTexture;

/**
 * The number of the indices
 */
GLuint numberIndicesTexture;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waterPlaneLengthLocationTexture;

/**
 * Location of the delta time in the shader program.
 */
GLint g_passedTimeLocationTexture;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waveParametersLocationTexture;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waveDirectionsLocationTexture;

//

/**
 * The frame buffer object
 */
GLuint g_fboTexture;

//

/**
 * Width of the parent/caller element.
 */
GLuint g_parentWidth;

/**
 * Height of the parent/caller element.
 */
GLuint g_parentHeight;

/**
 * Function for initialization.
 */
GLUSuint initTexture(GLUSfloat waterPlaneLength)
{
	GLUSshape plane;

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example14/VertexWaterTexture.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example14/FragmentWaterTexture.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_programTexture, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// ToDo:
	glGenVertexArrays(1, &g_vaoTexture);

	// ToDo:
	glBindVertexArray(g_vaoTexture);

	glusCreatePlanef(&plane, TEXTURE_SIZE / 2.0f);
	numberIndicesTexture = plane.numberIndices;

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_projectionLocationTexture = glGetUniformLocation(g_programTexture.program, "projectionMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_modelViewLocationTexture = glGetUniformLocation(g_programTexture.program, "modelViewMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waterPlaneLengthLocationTexture = glGetUniformLocation(g_programTexture.program, "waterPlaneLength");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_passedTimeLocationTexture = glGetUniformLocation(g_programTexture.program, "passedTime");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waveParametersLocationTexture = glGetUniformLocation(g_programTexture.program, "waveParameters");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waveDirectionsLocationTexture = glGetUniformLocation(g_programTexture.program, "waveDirections");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocationTexture = glGetAttribLocation(g_programTexture.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_texCoordLocationTexture = glGetAttribLocation(g_programTexture.program, "texCoord");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_verticesTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) plane.vertices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_texCoordsTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 2 * sizeof(GLfloat), (GLfloat*) plane.texCoords, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indicesTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, plane.numberIndices * sizeof(GLuint), (GLuint*) plane.indices, GL_STATIC_DRAW);

	glusDestroyShapef(&plane);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programTexture.program);

	// Calculate the model matrix ...
	glusLoadIdentityf(g_modelViewTexture);

	// ... and the view matrix ...
	glusLookAtf(g_modelViewTexture, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocationTexture, 1, GL_FALSE, g_modelViewTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_waterPlaneLengthLocationTexture, waterPlaneLength);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocationTexture, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocationTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_texCoordLocationTexture, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_texCoordLocationTexture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable(GL_DEPTH_TEST);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable(GL_CULL_FACE);

	//

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/gentextures.html
	glGenTextures(1, &g_textureTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture(GL_TEXTURE1);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_textureTexture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_2D, 0, GLUS_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GLUS_RGB, GL_UNSIGNED_BYTE, 0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ToDo:
	glGenFramebuffers(1, &g_fboTexture);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_fboTexture);

	// ToDo:
	glGenRenderbuffers(1, &g_depthTexture);

	// ToDo:
	glBindRenderbuffer(GL_RENDERBUFFER, g_depthTexture);

	// ToDo:
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, TEXTURE_SIZE, TEXTURE_SIZE);

	// Attach the color buffer ...
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_textureTexture, 0);

	// ... and the depth buffer,
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depthTexture);

	// ToDo:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// ToDo:
		printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, 0);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return g_textureTexture;
}

/**
 * Function is called, if the window is resized.
 *
 * @param w	width of the window
 * @param h	height of the window
 */
GLUSvoid reshapeTexture(GLUSuint width, GLUSuint height)
{
	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_fboTexture);

	// Calculate the projection matrix and set it
	glusOrthof(g_projectionTexture, -(GLfloat) TEXTURE_SIZE / 2, (GLfloat) TEXTURE_SIZE / 2, -(GLfloat) TEXTURE_SIZE / 2, (GLfloat) TEXTURE_SIZE / 2, 1.0f, 100.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programTexture.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_projectionLocationTexture, 1, GL_FALSE, g_projectionTexture);

	// Store parent/caller width ..
	g_parentWidth = width;

	// ... and height for later usage to set to the original values
	g_parentHeight = height;

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/**
 * Function to render and display content.
 */
GLUSboolean updateTexture(GLUSfloat passedTime)
{
	static WaveParameters waveParameters[NUMBERWAVES];
	static WaveDirections waveDirections[NUMBERWAVES];

	static GLfloat overallSteepness = 0.2f;

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, TEXTURE_SIZE, TEXTURE_SIZE);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_fboTexture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//
	memset(waveParameters, 0, sizeof(waveParameters));

	//
	memset(waveDirections, 0, sizeof(waveDirections));

	// Waves can be faded in and out.

	// Wave One
	waveParameters[0].speed = 0.05f;
	waveParameters[0].amplitude = 0.02f;
	waveParameters[0].wavelength = 0.3f;
	waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (GLfloat) NUMBERWAVES);
	waveDirections[0].x = +1.0f;
	waveDirections[0].z = +1.5f;

	// Wave Two
	waveParameters[1].speed = 0.1f;
	waveParameters[1].amplitude = 0.01f;
	waveParameters[1].wavelength = 0.4f;
	waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
	waveDirections[1].x = +0.8f;
	waveDirections[1].z = +0.2f;

	// Wave Thre
	waveParameters[2].speed = 0.04f;
	waveParameters[2].amplitude = 0.035f;
	waveParameters[2].wavelength = 0.1f;
	waveParameters[2].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
	waveDirections[2].x = -0.2f;
	waveDirections[2].z = -0.1f;

	// Wave Four
	waveParameters[3].speed = 0.05f;
	waveParameters[3].amplitude = 0.007f;
	waveParameters[3].wavelength = 0.2f;
	waveParameters[3].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
	waveDirections[3].x = -0.4f;
	waveDirections[3].z = -0.3f;

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programTexture.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_passedTimeLocationTexture, passedTime);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform4fv(g_waveParametersLocationTexture, 4 * NUMBERWAVES, (GLfloat*) waveParameters);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform2fv(g_waveDirectionsLocationTexture, 2 * NUMBERWAVES, (GLfloat*) waveDirections);

	// TODO:
	glBindVertexArray(g_vaoTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, numberIndicesTexture, GL_UNSIGNED_INT, 0);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, g_parentWidth, g_parentHeight);

	return GLUS_TRUE;
}

/**
 * Function to clean up things.
 */
GLUSvoid terminateTexture(GLUSvoid)
{
	if (g_verticesTexture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_verticesTexture);

		g_verticesTexture = 0;
	}

	if (g_texCoordsTexture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_texCoordsTexture);

		g_texCoordsTexture = 0;
	}

	if (g_indicesTexture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_indicesTexture);

		g_indicesTexture = 0;
	}

	if (g_textureTexture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_textureTexture);

		g_textureTexture = 0;
	}

	// ToDo:
	glDeleteVertexArrays(1, &g_vaoTexture);

	// Delete shader program etc..
	glusDestroyProgram(&g_programTexture);

	//

	if (g_fboTexture)
	{
		// ToDo:
		glDeleteFramebuffers(1, &g_fboTexture);

		g_fboTexture = 0;
	}

	if (g_depthTexture)
	{
		// ToDo:
		glDeleteRenderbuffers(1, &g_depthTexture);

		g_depthTexture = 0;
	}
}
