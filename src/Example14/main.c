/**
 * OpenGL 4 - Example 14
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
 * Needed for malloc
 */
#include <stdlib.h>

/**
 * Needed for sine
 */
#include <math.h>

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
 * Length (width and height) of the water surface,
 */
const GLuint WATER_PLANE_LENGTH = 128;

/**
 * Projection matrix will be calculated later.
 */
GLfloat g_projection[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelView[16];

/**
 * Inverse camera needed for the final reflect/refratc normal calculation in the shader.
 */
GLfloat g_inverseCamera[16];

/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionLocation;

/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionLocationBackground;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewLocation;

/**
 * Loction of the inverse matrix.
 */
GLint g_inverseCameraLocation;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewLocationBackground;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waterPlaneLengthLocation;

/**
 * Location of the delta time in the shader program.
 */
GLint g_passedTimeLocation;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waveParametersLocation;

/**
 * Location of the delta time in the shader program.
 */
GLint g_waveDirectionsLocation;

/**
 * The used shader program.
 */
GLUSshaderprogram g_program;

/**
 * The used shader program.
 */
GLUSshaderprogram g_programBackground;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vao;

/**
 * The VBO for the vertices.
 */
GLuint g_vertices;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocation;

/**
 * The VBO for the indices.
 */
GLuint g_indices;

/**
 * The location of the texture.
 */
GLint g_cubemapLocation;

/**
 * The location of the texture.
 */
GLint g_waterLocation;

/**
 * The cubemap texture.
 */
GLuint g_cubemap;

//

/**
 * The VAO for the vertices etc.
 */
GLuint g_vaoBackground;

/**
 * The VBO for the vertices.
 */
GLuint g_verticesBackground;

/**
 * The VBO for the normals.
 */
GLuint g_normalsBackground;

/**
 * The VBO for the indices.
 */
GLuint g_indicesBackground;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocationBackground;

/**
 * The location of the texture coordinate in the shader program.
 */
GLint g_normalLocationBackground;

/**
 * The location of the texture.
 */
GLint g_cubemapLocationBackground;

/**
 * The number of the indices
 */
GLuint g_numberIndicesBackground;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// Points of a triangle.
	GLfloat* points = (GLfloat*)malloc(WATER_PLANE_LENGTH*WATER_PLANE_LENGTH*4*sizeof(GLfloat));

	GLuint* indices = (GLuint*)malloc(WATER_PLANE_LENGTH*(WATER_PLANE_LENGTH-1)*2*sizeof(GLuint));

	GLUSshape background;

	GLUStgaimage image;

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	GLuint x, z, i, k;

	GLuint waterTexture;

	for (z = 0; z < WATER_PLANE_LENGTH; z++)
	{
		for (x = 0; x < WATER_PLANE_LENGTH; x++)
		{
			points[(x+z*(WATER_PLANE_LENGTH))*4 + 0] = -(GLfloat)WATER_PLANE_LENGTH/2 + 0.5f + (GLfloat)x;
			points[(x+z*(WATER_PLANE_LENGTH))*4 + 1] = 0.0f;
			points[(x+z*(WATER_PLANE_LENGTH))*4 + 2] = +(GLfloat)WATER_PLANE_LENGTH/2 - 0.5f - (GLfloat)z;
			points[(x+z*(WATER_PLANE_LENGTH))*4 + 3] = 1.0f;
		}
	}

	for (k = 0; k < WATER_PLANE_LENGTH-1; k++)
	{
		for (i = 0; i < WATER_PLANE_LENGTH; i++)
		{
			if (k%2 == 0)
			{
				indices[(i+k*(WATER_PLANE_LENGTH))*2 + 0] = i + k*WATER_PLANE_LENGTH;
				indices[(i+k*(WATER_PLANE_LENGTH))*2 + 1] = i + (k+1)*WATER_PLANE_LENGTH;
			}
			else
			{
				indices[(i+k*(WATER_PLANE_LENGTH))*2 + 0] = WATER_PLANE_LENGTH - 1 - i + k*WATER_PLANE_LENGTH;
				indices[(i+k*(WATER_PLANE_LENGTH))*2 + 1] = WATER_PLANE_LENGTH - 1 - i + (k+1)*WATER_PLANE_LENGTH;
			}
		}
	}

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example14/Vertex.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example14/Fragment.fs", &fragmentSource);

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

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waterPlaneLengthLocation = glGetUniformLocation(g_program.program, "waterPlaneLength");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_inverseCameraLocation = glGetUniformLocation(g_program.program, "inverseCameraMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_cubemapLocation = glGetUniformLocation(g_program.program, "cubemap");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waterLocation = glGetUniformLocation(g_program.program, "waterTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_passedTimeLocation = glGetUniformLocation(g_program.program, "passedTime");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waveParametersLocation = glGetUniformLocation(g_program.program, "waveParameters");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_waveDirectionsLocation = glGetUniformLocation(g_program.program, "waveDirections");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocation = glGetAttribLocation(g_program.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, WATER_PLANE_LENGTH*WATER_PLANE_LENGTH * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, WATER_PLANE_LENGTH*(WATER_PLANE_LENGTH-1) * 2 * sizeof(GLuint), (GLuint*) indices, GL_STATIC_DRAW);

	//

	free(points);

	free(indices);

	//

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/gentextures.html
	glGenTextures(1, &g_cubemap);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

	glusLoadTgaImage("water_pos_x.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	glusLoadTgaImage("water_neg_x.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	glusLoadTgaImage("water_pos_y.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	glusLoadTgaImage("water_neg_y.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	glusLoadTgaImage("water_pos_z.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	glusLoadTgaImage("water_neg_z.tga", &image);
	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
	glusDestroyTgaImage(&image);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	//

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_waterPlaneLengthLocation, (GLUSfloat)WATER_PLANE_LENGTH);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture(GL_TEXTURE0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_cubemapLocation, 0);

	// Get the render buffer texture
	waterTexture = initTexture((GLUSfloat)WATER_PLANE_LENGTH);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture(GL_TEXTURE1);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, waterTexture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_waterLocation, 1);

	//

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example14/VertexBackground.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example14/FragmentBackground.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_programBackground, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// ToDo:
	glGenVertexArrays(1, &g_vaoBackground);

	// ToDo:
	glBindVertexArray(g_vaoBackground);

	glusCreateSpheref(&background, (GLfloat)(GLfloat)WATER_PLANE_LENGTH/2.0f + 0.5f, 32);
	g_numberIndicesBackground = background.numberIndices;

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_projectionLocationBackground = glGetUniformLocation(g_programBackground.program, "projectionMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_modelViewLocationBackground = glGetUniformLocation(g_programBackground.program, "modelViewMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_cubemapLocationBackground = glGetUniformLocation(g_programBackground.program, "cubemap");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocationBackground = glGetAttribLocation(g_programBackground.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_normalLocationBackground = glGetAttribLocation(g_programBackground.program, "normal");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_verticesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) background.vertices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_normalsBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) background.normals, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indicesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, background.numberIndices * sizeof(GLuint), (GLuint*) background.indices, GL_STATIC_DRAW);

	glusDestroyShapef(&background);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programBackground.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocationBackground, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocationBackground, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocationBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_normalLocationBackground, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_normalLocationBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_cubemapLocationBackground, 0);

	//

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_DEPTH_TEST);

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
	glusPerspectivef(g_projection, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 1000.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programBackground.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_projectionLocationBackground, 1, GL_FALSE, g_projection);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_projectionLocation, 1, GL_FALSE, g_projection);

	// Call the resahpe of the texture
	reshapeTexture(width, height);
}

/**
 * Renders the water
 */
GLUSvoid renderWater(GLUSfloat passedTime, GLUSfloat angle)
{
	static WaveParameters waveParameters[NUMBERWAVES];
	static WaveDirections waveDirections[NUMBERWAVES];

	static GLfloat overallSteepness = 0.2f;

	GLfloat model[16];

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	// ... and the view matrix ...
	glusLookAtf(g_modelView, 0.0f, 1.0f, 0.0f, (GLfloat)0.5f * sinf(angle), 1.0f, -(GLfloat)0.5f * cosf(angle), 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelView, g_modelView, model);

	// Copy the original view matrix ...
	glusCopyMatrixf(g_inverseCamera, g_modelView, GLUS_TRUE);
	// ... and inverse it
	glusInverseMatrixf(g_inverseCamera);

	memset(waveParameters, 0, sizeof(waveParameters));
	memset(waveDirections, 0, sizeof(waveDirections));

	// Waves can be faded in and out.

	// Wave One
	waveParameters[0].speed = 1.0f;
	waveParameters[0].amplitude = 0.01f;
	waveParameters[0].wavelength = 4.0f;
	waveParameters[0].steepness = overallSteepness/(waveParameters[0].wavelength*waveParameters[0].amplitude*(GLfloat)NUMBERWAVES);
	waveDirections[0].x = +1.0f;
	waveDirections[0].z = +1.0f;

	// Wave Two
	waveParameters[1].speed = 0.5f;
	waveParameters[1].amplitude = 0.02f;
	waveParameters[1].wavelength = 3.0f;
	waveParameters[1].steepness = overallSteepness/(waveParameters[1].wavelength*waveParameters[1].amplitude*(GLfloat)NUMBERWAVES);
	waveDirections[1].x = +1.0f;
	waveDirections[1].z = +0.0f;

	// Wave Thre
	waveParameters[2].speed = 0.1f;
	waveParameters[2].amplitude = 0.015f;
	waveParameters[2].wavelength = 2.0f;
	waveParameters[2].steepness = overallSteepness/(waveParameters[1].wavelength*waveParameters[1].amplitude*(GLfloat)NUMBERWAVES);
	waveDirections[2].x = -0.1f;
	waveDirections[2].z = -0.2f;

	// Wave Four
	waveParameters[3].speed = 1.1f;
	waveParameters[3].amplitude = 0.008f;
	waveParameters[3].wavelength = 1.0f;
	waveParameters[3].steepness = overallSteepness/(waveParameters[1].wavelength*waveParameters[1].amplitude*(GLfloat)NUMBERWAVES);
	waveDirections[3].x = -0.2f;
	waveDirections[3].z = -0.1f;

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_inverseCameraLocation, 1, GL_FALSE, g_inverseCamera);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_passedTimeLocation, passedTime);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform4fv(g_waveParametersLocation, 4*NUMBERWAVES, (GLfloat*)waveParameters);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform2fv(g_waveDirectionsLocation, 2*NUMBERWAVES, (GLfloat*)waveDirections);

	// ToDo:
	glBindVertexArray(g_vao);

	// ToDo:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/disable.html
	glDisable(GL_CULL_FACE);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLE_STRIP, WATER_PLANE_LENGTH*(WATER_PLANE_LENGTH-1)*2, GL_UNSIGNED_INT, 0);
}

/**
 * Function to render and display content.
 */
GLUSboolean update(GLUSfloat time)
{
	static GLfloat passedTime = 0.0f;

	static GLUSfloat angle = 0.0f;

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programBackground.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocationBackground, 1, GL_FALSE, g_modelView);

	// ToDo:
	glBindVertexArray(g_vaoBackground);

	// ToDo:
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_CULL_FACE);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);
	
	// Update the texture
	updateTexture(passedTime);

	// Render the water scene
	renderWater(passedTime, angle);

	passedTime += time;

	angle += 2.0f * PIf / 120.0f * time;

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

	if (g_cubemap)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_cubemap);

		g_cubemap = 0;
	}

	// ToDo:
	glDeleteVertexArrays(1, &g_vao);

	// Delete shader program etc..
	glusDestroyProgram(&g_program);

	//

	if (g_verticesBackground)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_verticesBackground);

		g_verticesBackground = 0;
	}

	if (g_normalsBackground)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_normalsBackground);

		g_normalsBackground = 0;
	}

	if (g_indicesBackground)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_indicesBackground);

		g_indicesBackground = 0;
	}

	// ToDo:
	glDeleteVertexArrays(1, &g_vaoBackground);

	// Delete shader program etc..
	glusDestroyProgram(&g_programBackground);

	// Terminate the texture stuff.
	terminateTexture();
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
