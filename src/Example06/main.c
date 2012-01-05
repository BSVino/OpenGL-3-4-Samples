/**
 * OpenGL 3 - Example 06
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
 * Location of the model view matrix in the shader program.
 */
GLint g_lightDirLocation;

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
 * The VBO for the tangents.
 */
GLuint g_tangents;

/**
 * The VBO for the texture coordinates.
 */
GLuint g_texCoords;

/**
 * The VBO for the indices.
 */
GLuint g_indices;

/**
 * The texture.
 */
GLuint g_texture;

/**
 * The normal map.
 */
GLuint g_normalMap;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocation;

/**
 * The location of the normal in the shader program.
 */
GLint g_normalLocation;

/**
 * The location of the tangent in the shader program.
 */
GLint g_tangentLocation;

/**
 * The location of the texture coordinate in the shader program.
 */
GLint g_texCoordLocation;

/**
 * The location of the texture.
 */
GLint g_textureLocation;

/**
 * The location of the normal map.
 */
GLint g_normalMapLocation;

/**
 * The number of the indices
 */
GLuint numberIndices;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	GLfloat points[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };

	GLfloat normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

	GLfloat tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

	GLfloat texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	GLuint indices[] = { 0, 1, 2, 1, 3, 2 };

	GLuint numberVertices = 4;

	GLUStgaimage image;

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	numberIndices = 6;

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example06/Vertex.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example06/Fragment.fs", &fragmentSource);

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
	g_lightDirLocation = glGetUniformLocation(g_program.program, "lightDir");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_textureLocation = glGetUniformLocation(g_program.program, "firstTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_normalMapLocation = glGetUniformLocation(g_program.program, "normalMap");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocation = glGetAttribLocation(g_program.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_normalLocation = glGetAttribLocation(g_program.program, "normal");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_tangentLocation = glGetAttribLocation(g_program.program, "tangent");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_texCoordLocation = glGetAttribLocation(g_program.program, "texCoord");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, numberVertices * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, numberVertices * 3 * sizeof(GLfloat), (GLfloat*) normals, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_tangents);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_tangents);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, numberVertices * 3 * sizeof(GLfloat), (GLfloat*) tangents, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_texCoords);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_texCoords);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, numberVertices * 2 * sizeof(GLfloat), (GLfloat*) texCoords, GL_STATIC_DRAW);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numberIndices * sizeof(GLuint), (GLuint*) indices, GL_STATIC_DRAW);

	glusLoadTgaImage("rock_color.tga", &image);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/gentextures.html
	glGenTextures(1, &g_texture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_texture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glusDestroyTgaImage(&image);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glusLoadTgaImage("rock_normal.tga", &image);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/gentextures.html
	glGenTextures(1, &g_normalMap);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_normalMap);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glusDestroyTgaImage(&image);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_normals);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_normalLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_tangents);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_tangentLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_tangentLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_texCoords);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_texCoordLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_textureLocation, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture( GL_TEXTURE0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_texture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_normalMapLocation, 1);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture( GL_TEXTURE1);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_normalMap);

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
	static GLfloat angle = 0.0;

	// http://www.cplusplus.com/reference/clibrary/cmath/cos.html
	GLfloat lightDir[] = { 2.0f * cosf(angle), 0.0f, 1.0f };
	angle += time;

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform3fv(g_lightDirLocation, 1, (GLfloat*) lightDir);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawElements.xml
	glDrawElements(GL_TRIANGLES, numberIndices, GL_UNSIGNED_INT, 0);

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

	if (g_tangents)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_tangents);

		g_tangents = 0;
	}

	if (g_texCoords)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_texCoords);

		g_texCoords = 0;
	}

	if (g_indices)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteBuffers.xml
		glDeleteBuffers(1, &g_indices);

		g_indices = 0;
	}

	if (g_texture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_texture);

		g_texture = 0;
	}

	if (g_normalMap)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_normalMap);

		g_normalMap = 0;
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
