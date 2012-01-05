/**
 * OpenGL 3 - Example 08
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

/**
 * Needed for rand
 */
#include <stdlib.h>

/**
 * Needed for printf
 */
#include <stdio.h>

#include "GL/glus.h"

/**
 * Define number of particels
 */
#define PARTICLES_LOOKUP_WIDTH 16

/**
 * Width of the viewport
 */
GLint g_widthViewport;

/**
 * Height of the viewport
 */
GLint g_heightViewport;

/**
 * The used shader program.
 */
GLUSshaderprogram g_program;

/**
 * The used shader program.
 */
GLUSshaderprogram g_programUpdate;

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
 * Size of the lookup 
 */
GLint g_sizeLocation;

/**
 * Time of the lookup 
 */
GLint g_timeLocation;

/**
 * The VAO for the vertices etc.
 */
GLuint g_vao;

/**
 * The VBO for the vertices.
 */
GLuint g_vertices;

/**
 * The texture.
 */
GLuint g_texture;

/**
 * The lookup textures.
 */
GLuint g_lookuptexture[2];

/**
 * The framebuffers for the texture.
 */
GLuint g_lookupframebuffer[2];

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocation;

/**
 * The location of the vertex in the shader program.
 */
GLint g_vertexLocationUpdate;

/**
 * The location of the texture.
 */
GLint g_textureLocation;

/**
 * The location of the texture.
 */
GLint g_lookuptextureLocation;

/**
 * The location of the texture.
 */
GLint g_lookuptextureLocationUpdate;

/**
 * Function for initialization.
 */
GLUSboolean init(GLUSvoid)
{
	// Points for referencing each texel
	GLfloat points[PARTICLES_LOOKUP_WIDTH * PARTICLES_LOOKUP_WIDTH * 2];

	// Initial particle data
	GLfloat particle[PARTICLES_LOOKUP_WIDTH * PARTICLES_LOOKUP_WIDTH * 4];

	GLUStgaimage image;

	GLUStextfile vertexSource;

	GLUStextfile fragmentSource;

	GLuint x, y;

	// Create points which are refernceing on the different texels
	for (y = 0; y < PARTICLES_LOOKUP_WIDTH; y++)
	{
		for (x = 0; x < PARTICLES_LOOKUP_WIDTH; x++)
		{
			points[x * 2 + 0 + y * PARTICLES_LOOKUP_WIDTH * 2] = (GLfloat) x / (GLfloat) PARTICLES_LOOKUP_WIDTH;
			points[x * 2 + 1 + y * PARTICLES_LOOKUP_WIDTH * 2] = (GLfloat) y / (GLfloat) PARTICLES_LOOKUP_WIDTH;
		}
	}

	// Create the initial particles data
	for (y = 0; y < PARTICLES_LOOKUP_WIDTH; y++)
	{
		for (x = 0; x < PARTICLES_LOOKUP_WIDTH; x++)
		{
			particle[x * 4 + 0 + y * PARTICLES_LOOKUP_WIDTH * 4] = 0.0f;
			particle[x * 4 + 1 + y * PARTICLES_LOOKUP_WIDTH * 4] = 0.0f;
			particle[x * 4 + 2 + y * PARTICLES_LOOKUP_WIDTH * 4] = 0.0f;
			particle[x * 4 + 3 + y * PARTICLES_LOOKUP_WIDTH * 4] = (rand() % 101) / 100.0f;
		}
	}

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example08/Vertex.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example08/Fragment.fs", &fragmentSource);

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
	g_textureLocation = glGetUniformLocation(g_program.program, "firstTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_lookuptextureLocation = glGetUniformLocation(g_program.program, "readTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocation = glGetAttribLocation(g_program.program, "vertex");

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example08/UpdatePoints.vs", &vertexSource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example08/UpdatePoints.fs", &fragmentSource);

	// Build and ...
	glusBuildProgram(&g_programUpdate, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_lookuptextureLocationUpdate = glGetUniformLocation(g_programUpdate.program, "readTexture");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_sizeLocation = glGetUniformLocation(g_programUpdate.program, "size");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetUniformLocation.xml
	g_timeLocation = glGetUniformLocation(g_programUpdate.program, "time");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGetAttribLocation.xml
	g_vertexLocationUpdate = glGetAttribLocation(g_programUpdate.program, "vertex");

	// http://www.opengl.org/sdk/docs/man/xhtml/glGenBuffers.xml
	glGenBuffers(1, &g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBufferData.xml
	glBufferData(GL_ARRAY_BUFFER, PARTICLES_LOOKUP_WIDTH * PARTICLES_LOOKUP_WIDTH * 2 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

	glusLoadTgaImage("particle.tga", &image);

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

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/gentextures.html
	glGenTextures(2, g_lookuptexture);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_lookuptexture[0]);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PARTICLES_LOOKUP_WIDTH, PARTICLES_LOOKUP_WIDTH, 0, GL_RGBA, GL_FLOAT, particle);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_lookuptexture[1]);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/teximage2d.html
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PARTICLES_LOOKUP_WIDTH, PARTICLES_LOOKUP_WIDTH, 0, GL_RGBA, GL_FLOAT, particle);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/texparameter.html
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ToDo:
	glGenFramebuffers(2, g_lookupframebuffer);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_lookupframebuffer[0]);

	// ToDo:
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_lookuptexture[0], 0);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_lookupframebuffer[1]);

	// ToDo:
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_lookuptexture[1], 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// Calculate the view matrix ...
	glusLookAtf(g_modelView, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniformMatrix4fv(g_modelViewLocation, 1, GL_FALSE, g_modelView);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocation);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_textureLocation, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture( GL_TEXTURE0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_texture);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programUpdate.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_sizeLocation, (GLfloat) PARTICLES_LOOKUP_WIDTH);

	// http://www.opengl.org/sdk/docs/man/xhtml/glBindBuffer.xml
	glBindBuffer(GL_ARRAY_BUFFER, g_vertices);

	// http://www.opengl.org/sdk/docs/man/xhtml/glVertexAttribPointer.xml
	glVertexAttribPointer(g_vertexLocationUpdate, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// http://www.opengl.org/sdk/docs/man/xhtml/glEnableVertexAttribArray.xml
	glEnableVertexAttribArray(g_vertexLocationUpdate);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clearcolor.html
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable( GL_VERTEX_PROGRAM_POINT_SIZE);

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
	g_widthViewport = width;

	g_heightViewport = height;

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
	// Variable for switching between the buffers
	static GLint flip = 0;

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_programUpdate.program);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, g_lookupframebuffer[(flip + 1) % 2]);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, PARTICLES_LOOKUP_WIDTH, PARTICLES_LOOKUP_WIDTH);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1f(g_timeLocation, time * 2.0f);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_lookuptextureLocationUpdate, 1);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture( GL_TEXTURE1);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_lookuptexture[flip]);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glDisable( GL_BLEND);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawArrays.xml
	glDrawArrays(GL_POINTS, 0, PARTICLES_LOOKUP_WIDTH * PARTICLES_LOOKUP_WIDTH);

	// ToDo:
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/viewport.html
	glViewport(0, 0, g_widthViewport, g_heightViewport);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/clear.html
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUseProgram.xml
	glUseProgram(g_program.program);

	// http://www.opengl.org/sdk/docs/man/xhtml/glUniform.xml
	glUniform1i(g_lookuptextureLocation, 1);

	// http://www.opengl.org/sdk/docs/man/xhtml/glActiveTexture.xml
	glActiveTexture(GL_TEXTURE1);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/bindtexture.html
	glBindTexture(GL_TEXTURE_2D, g_lookuptexture[flip]);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/enable.html
	glEnable(GL_BLEND);

	// http://www.opengl.org/documentation/specs/man_pages/hardcopy/GL/html/gl/blendfunc.html
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	// http://www.opengl.org/sdk/docs/man/xhtml/glDrawArrays.xml
	glDrawArrays(GL_POINTS, 0, PARTICLES_LOOKUP_WIDTH * PARTICLES_LOOKUP_WIDTH);

	// Swap the framebuffers/textures
	flip = (flip + 1) % 2;

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

	if (g_texture)
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_texture);

		g_texture = 0;
	}

	if (g_lookupframebuffer[0])
	{
		// ToDo:
		glDeleteFramebuffers(1, &g_lookupframebuffer[0]);
		g_lookupframebuffer[0] = 0;
	}

	if (g_lookupframebuffer[1])
	{
		// ToDo:
		glDeleteFramebuffers(1, &g_lookupframebuffer[1]);
		g_lookupframebuffer[1] = 0;
	}

	if (g_lookuptexture[0])
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_lookuptexture[0]);

		g_lookuptexture[0] = 0;
	}

	if (g_lookuptexture[1])
	{
		// http://www.opengl.org/sdk/docs/man/xhtml/glDeleteTextures.xml
		glDeleteTextures(1, &g_lookuptexture[1]);

		g_lookuptexture[1] = 0;
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
