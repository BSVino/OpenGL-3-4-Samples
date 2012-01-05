/**
 * OpenGL 3 - Example 11
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

/**
 * Needed for sinf, cosf
 */
#include <math.h>

#include "GL/glus.h"

/**
 * Global width of the viewport
 */
GLUSuint g_width;

/**
 * Global height of the viewport
 */
GLUSuint g_height;

/**
 * Camera position
 */
GLUSfloat g_camera[3] = { -5.0f, 5.0f, 10.0f };

/**
 * Light
 */
GLUSfloat g_light[3] = { 0.0f, 0.0f, 10.0f };

/**
 * The used shader program.
 */
GLUSshaderprogram g_program;

/**
 * Shadow matrix.
 */
GLfloat g_shadowMatrix[16];

/**
 * Projection matrix will be calculated later.
 */
GLfloat g_projection[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelView[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelViewBackground[16];

/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionLocation;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewLocation;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_shadowMatrixLocation;

/**
 * Location of the shadow texture.
 */
GLint g_shadowTextureLocation;

/**
 * The location of the color.
 */
GLint g_colorLocation;

/**
 * The location of the color.
 */
GLint g_lightLocation;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vao;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vaoShadow;

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
 * The number of the indices
 */
GLuint g_numberIndices;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vaoBackground;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vaoShadowBackground;

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
 * The number of the indices
 */
GLuint g_numberIndicesBackground;

/**
 * The used shader program.
 */
GLUSshaderprogram g_programShadow;

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelViewShadow[16];

/**
 * Move the cube along the minus z axis for making it visible. Also rotated.
 */
GLfloat g_modelViewShadowBackground[16];

/**
 * Location of the projection matrix in the shader program.
 */
GLint g_projectionShadowLocation;

/**
 * Location of the model view matrix in the shader program.
 */
GLint g_modelViewShadowLocation;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexShadowLocation;

//

/**
 * The shadow texture size
 */
GLuint g_shadowTextureSize = 1024;

/**
 * The shadow texture
 */
GLuint g_shadowTexture;

/**
 * The frame buffer object
 */
GLuint g_fbo;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// Matrix for the model
	GLfloat model[16];

	// Matrix for the model
	GLfloat lightCamera[3];

	GLUSshape torus;

	GLUSshape background;

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	// ToDo:
	glGenTextures(1, &g_shadowTexture);

	// ToDo:
	glBindTexture(GL_TEXTURE_2D, g_shadowTexture);

	// ToDo:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// ToDo:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ToDo:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// ToDo:
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ToDo:
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, g_shadowTextureSize, g_shadowTextureSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

	// ToDo:
	glBindTexture(GL_TEXTURE_2D, 0);

	// ToDo:
	glGenFramebuffers(1, &g_fbo);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);

	// ToDo:
	glDrawBuffer( GL_NONE);

	// ToDo:
	glReadBuffer(GL_NONE);

	// ToDo:
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_shadowTexture, 0);

	// ToDo:
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		// ToDo:
		printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));
	}

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_DEPTH_TEST);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_CULL_FACE);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example11/VertexShadow.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example11/FragmentShadow.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_programShadow, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_projectionShadowLocation = glGetUniformLocation(g_programShadow.program, "projectionMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_modelViewShadowLocation = glGetUniformLocation(g_programShadow.program, "modelViewMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexShadowLocation = glGetAttribLocation(g_programShadow.program, "vertex");

	//

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example11/Vertex.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example11/Fragment.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_projectionLocation = glGetUniformLocation(g_program.program, "projectionMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_modelViewLocation = glGetUniformLocation(g_program.program, "modelViewMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_shadowMatrixLocation = glGetUniformLocation(g_program.program, "shadowMatrix");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_shadowTextureLocation = glGetUniformLocation(g_program.program, "shadowTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_colorLocation = glGetUniformLocation(g_program.program, "shapeColor");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_lightLocation = glGetUniformLocation(g_program.program, "lightDirection");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocation = glGetAttribLocation(g_program.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_normalLocation = glGetAttribLocation(g_program.program, "normal");

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	glusLookAtf(model, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glusMultVectorf(lightCamera, model, g_light);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform3f(g_lightLocation, lightCamera[0], lightCamera[1], lightCamera[2]);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_shadowTextureLocation, 0);

	//
	// Torus
	//

	// ToDo:
	glGenVertexArrays(1, &g_vao);

	// ToDo:
	glBindVertexArray(g_vao);

	glusCreateTorusf(&torus, 0.5f, 1.0f, 32, 32);
	g_numberIndices = torus.numberIndices;

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, torus.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) torus.vertices, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, torus.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) torus.normals, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, torus.numberIndices * sizeof(GLuint), (GLuint*) torus.indices, GL_STATIC_DRAW);

	glusDestroyShapef(&torus);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	// ... and the view matrix ...
	glusLookAtf(g_modelView, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelView, g_modelView, model);

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

	// ToDo:
	glBindTexture(GL_TEXTURE_2D, g_shadowTexture);

	//

	// ToDo:
	glGenVertexArrays(1, &g_vaoShadow);

	// ToDo:
	glBindVertexArray(g_vaoShadow);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programShadow.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexShadowLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexShadowLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	//
	// Background plane
	//

	// ToDo:
	glGenVertexArrays(1, &g_vaoBackground);

	// ToDo:
	glBindVertexArray(g_vaoBackground);

	glusCreatePlanef(&background, 10.0f);
	g_numberIndicesBackground = background.numberIndices;

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
	glUseProgram(g_program.program);

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	// ... move back a bit
	glusTranslatef(model, 0.0f, 0.0f, -5.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelViewBackground, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelViewBackground, g_modelViewBackground, model);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_normalLocation);

	// ToDo:
	glBindTexture(GL_TEXTURE_2D, g_shadowTexture);

	//

	// ToDo:
	glGenVertexArrays(1, &g_vaoShadowBackground);

	// ToDo:
	glBindVertexArray(g_vaoShadowBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programShadow.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexShadowLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexShadowLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackground);

	//

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/cleardepth.html
	glClearDepth(1.0f);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable(GL_DEPTH_TEST);

	//http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable(GL_CULL_FACE);

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
	static GLfloat bias[] = { 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f };

	// Matrix for the model
	GLfloat model[16];

	// Matrix for the model
	GLfloat modelViewInverse[16];

	g_width = width;

	g_height = height;

	//

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programShadow.program);

	// Calculate the projection matrix and set it
	glusPerspectivef(g_projection, 40.0f, (GLfloat) g_shadowTextureSize / (GLfloat) g_shadowTextureSize, 1.0f, 100.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_projectionShadowLocation, 1, GL_FALSE, g_projection);

	glusLookAtf(g_modelViewShadow, g_light[0], g_light[1], g_light[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	glusLoadIdentityf(g_shadowMatrix);
	glusMultMatrixf(g_shadowMatrix, g_shadowMatrix, bias);
	glusMultMatrixf(g_shadowMatrix, g_shadowMatrix, g_projection);
	glusMultMatrixf(g_shadowMatrix, g_shadowMatrix, g_modelViewShadow);

	glusLoadIdentityf(model);
	glusLookAtf(model, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	glusCopyMatrixf(modelViewInverse, model, GLUS_FALSE);
	glusInverseMatrixf(modelViewInverse);

	glusMultMatrixf(g_shadowMatrix, g_shadowMatrix, modelViewInverse);

	//

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

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
	// Angle for rotation
	static GLfloat angle = 0.0f;

	// Matrix for the model
	GLfloat model[16];

	//

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);

	// http://www.opengl.org/sdk/docs/man/xhtml/glColorMask.xml
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	glViewport(0, 0, g_shadowTextureSize, g_shadowTextureSize);

	//

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	glusRotateRzRyRxf(model, 0.0f, angle, 0.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelViewShadow, g_light[0], g_light[1], g_light[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelViewShadow, g_modelViewShadow, model);

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	// ... move back a bit
	glusTranslatef(model, 0.0f, 0.0f, -5.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelViewShadowBackground, g_light[0], g_light[1], g_light[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelViewShadowBackground, g_modelViewShadowBackground, model);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear( GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programShadow.program);

	// ToDo:
	glBindVertexArray(g_vaoShadowBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewShadowLocation, 1, GL_FALSE, g_modelViewShadowBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

	// ToDo:
	glBindVertexArray(g_vaoShadow);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewShadowLocation, 1, GL_FALSE, g_modelViewShadow);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

	//

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glColorMask.xml
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, g_width, g_height);

	//

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	glusRotateRzRyRxf(model, 0.0f, angle, 0.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelView, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelView, g_modelView, model);

	// Calculate the model matrix ...
	glusLoadIdentityf(model);
	glusTranslatef(model, 0.0f, 0.0f, -5.0f);
	// ... and the view matrix ...
	glusLookAtf(g_modelViewBackground, g_camera[0], g_camera[1], g_camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	// ... to get the final model view matrix
	glusMultMatrixf(g_modelViewBackground, g_modelViewBackground, model);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// ToDo:
	glBindVertexArray(g_vaoBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_shadowMatrixLocation, 1, GL_FALSE, g_shadowMatrix);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelViewBackground);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform4f(g_colorLocation, 0.0f, 0.5f, 0.0f, 1.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

	// ToDo:
	glBindVertexArray(g_vao);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_shadowMatrixLocation, 1, GL_FALSE, g_shadowMatrix);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform4f(g_colorLocation, 0.33f, 0.0f, 0.5f, 1.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

	//

	angle += 20.0f * time;

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

	// ToDo:
	glDeleteVertexArrays(1, &g_vaoShadow);

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

	// ToDo:
	glDeleteVertexArrays(1, &g_vaoShadowBackground);

	//

	if (g_fbo)
	{
		// ToDo:
		glDeleteFramebuffers(1, &g_fbo);

		g_fbo = 0;
	}

	if (g_shadowTexture)
	{
		// ToDo:
		glDeleteRenderbuffers(1, &g_shadowTexture);

		g_shadowTexture = 0;
	}

	//

	// Delete shader program etc..
	glusDestroyProgram(&g_program);

	glusDestroyProgram(&g_programShadow);
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
