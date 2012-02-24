/**
 * OpenGL 3 - Example 09
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include "GL/glus.h"

/**
 * Number of particles is PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH
 */
#define PARTICLE_TEXTURE_WIDTH 16

//

GLint g_widthViewport;
GLint g_heightViewport;

//

GLUSshaderprogram g_program;

//

GLfloat g_modelViewProjectionMatrix[16];

GLint g_modelViewProjectionMatrixLocation;

GLint g_positionTextureWidthLocation;

GLint g_timeLocation;

GLint g_vertexLocation;

GLint g_textureLocation;

GLint g_positionTextureLocation;

/**
 * The shader program to update the particle points.
 */
GLUSshaderprogram g_programUpdatePoints;

GLint g_vertexUpdatePointsLocation;

GLint g_positionTextureUpdatePointsLocation;

//

GLuint g_verticesVBO;

//

GLuint g_vao;

GLuint g_vaoUpdatePoints;

//

GLuint g_texture;

/**
 * Two textures, one for treading and one for writing.
 */
GLuint g_positionTexture[2];

/**
 * The framebuffers for the textures.
 */
GLuint g_positionFramebuffer[2];

GLUSboolean init(GLUSvoid)
{
    // Points for accessing the texel, which contain the particle data.
    GLfloat points[PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH * 2];

    // Array containing the initial particles.
    GLfloat particle[PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH * 4];

    GLUStgaimage image;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    GLuint x, y;

    for (y = 0; y < PARTICLE_TEXTURE_WIDTH; y++)
    {
        for (x = 0; x < PARTICLE_TEXTURE_WIDTH; x++)
        {
            points[x * 2 + 0 + y * PARTICLE_TEXTURE_WIDTH * 2] = (GLfloat) x / (GLfloat) PARTICLE_TEXTURE_WIDTH;
            points[x * 2 + 1 + y * PARTICLE_TEXTURE_WIDTH * 2] = (GLfloat) y / (GLfloat) PARTICLE_TEXTURE_WIDTH;
        }
    }

    for (y = 0; y < PARTICLE_TEXTURE_WIDTH; y++)
    {
        for (x = 0; x < PARTICLE_TEXTURE_WIDTH; x++)
        {
            particle[x * 4 + 0 + y * PARTICLE_TEXTURE_WIDTH * 4] = 0.0f;
            particle[x * 4 + 1 + y * PARTICLE_TEXTURE_WIDTH * 4] = 0.0f;
            particle[x * 4 + 2 + y * PARTICLE_TEXTURE_WIDTH * 4] = 0.0f;
            particle[x * 4 + 3 + y * PARTICLE_TEXTURE_WIDTH * 4] = -1.0f; // w < 0.0 means a dead particle. So renewed in first frame.
        }
    }

    //

    glusLoadTextFile("../Example09/shader/particle.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example09/shader/particle.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    g_modelViewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_modelViewProjectionMatrix");
    g_textureLocation = glGetUniformLocation(g_program.program, "u_texture");
    g_positionTextureLocation = glGetUniformLocation(g_program.program, "u_positionTexture");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");

    //

    glusLoadTextFile("../Example09/shader/update_points.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example09/shader/update_points.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_programUpdatePoints, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    g_positionTextureUpdatePointsLocation = glGetUniformLocation(g_programUpdatePoints.program, "u_positionTexture");
    g_positionTextureWidthLocation = glGetUniformLocation(g_programUpdatePoints.program, "u_positionTextureWidth");
    g_timeLocation = glGetUniformLocation(g_programUpdatePoints.program, "u_time");

    g_vertexUpdatePointsLocation = glGetAttribLocation(g_programUpdatePoints.program, "a_vertex");

    //

    glusLoadTgaImage("particle.tga", &image);

    glGenTextures(1, &g_texture);

    glBindTexture(GL_TEXTURE_2D, g_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glusDestroyTgaImage(&image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    //

    glGenTextures(2, g_positionTexture);

    glBindTexture(GL_TEXTURE_2D, g_positionTexture[0]);

    // Create a texture containing floats.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PARTICLE_TEXTURE_WIDTH, PARTICLE_TEXTURE_WIDTH, 0, GL_RGBA, GL_FLOAT, particle);

    // Important: No interpolation.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, g_positionTexture[1]);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, PARTICLE_TEXTURE_WIDTH, PARTICLE_TEXTURE_WIDTH, 0, GL_RGBA, GL_FLOAT, particle);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    //

    // Creation of the two framebuffers.

    glGenFramebuffers(2, g_positionFramebuffer);

    // Attach the texture.
    glBindFramebuffer(GL_FRAMEBUFFER, g_positionFramebuffer[0]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_positionTexture[0], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, g_positionFramebuffer[1]);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_positionTexture[1], 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //

    glGenBuffers(1, &g_verticesVBO);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);

    glBufferData(GL_ARRAY_BUFFER, PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH * 2 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //

    glUseProgram(g_program.program);

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(g_textureLocation, 0);
    glBindTexture(GL_TEXTURE_2D, g_texture);

    // Bindings happen during the update loop.
    glActiveTexture(GL_TEXTURE1);
    glUniform1i(g_positionTextureLocation, 1);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    //

    glUseProgram(g_programUpdatePoints.program);

    // We still work now on texture unit 1.
    glUniform1i(g_positionTextureUpdatePointsLocation, 1);

    glUniform1f(g_positionTextureWidthLocation, (GLfloat) PARTICLE_TEXTURE_WIDTH);

    // For each program, we do have a VAO.
    glGenVertexArrays(1, &g_vaoUpdatePoints);
    glBindVertexArray(g_vaoUpdatePoints);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexUpdatePointsLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexUpdatePointsLocation);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Use gl_PointSize in the shaders.
    glEnable(GL_PROGRAM_POINT_SIZE);

    // Formula to blend the particles.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    GLfloat viewMatrix[16];

    g_widthViewport = width;
    g_heightViewport = height;

    glViewport(0, 0, width, height);

    glusLookAtf(viewMatrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glusPerspectivef(g_modelViewProjectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

    glusMatrixMultiplyf(g_modelViewProjectionMatrix, g_modelViewProjectionMatrix, viewMatrix);

    glUseProgram(g_program.program);

    glUniformMatrix4fv(g_modelViewProjectionMatrixLocation, 1, GL_FALSE, g_modelViewProjectionMatrix);
}

GLUSboolean update(GLUSfloat time)
{
    // Variable for switching between the buffers.
    static GLint flip = 0;

    //
    // Update the particle point data.
    //

    glUseProgram(g_programUpdatePoints.program);

    glUniform1f(g_timeLocation, time);

     // Bind to the writing buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, g_positionFramebuffer[(flip + 1) % 2]);

    // Set the viewport to the texture size.
    glViewport(0, 0, PARTICLE_TEXTURE_WIDTH, PARTICLE_TEXTURE_WIDTH);

    // This texture contains the current particle data.
    glBindTexture(GL_TEXTURE_2D, g_positionTexture[flip]);

    glBindVertexArray(g_vaoUpdatePoints);

    // Draw all particles, data is updated in the shader.
    glDrawArrays(GL_POINTS, 0, PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH);

    // Go back to the window buffer.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Reset the viewport.
    glViewport(0, 0, g_widthViewport, g_heightViewport);

    //
    // Draw the particles on the screen.
    //

    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(g_program.program);

    glBindVertexArray(g_vao);

    // Use the particle data from current frame. Updated data is used in next frame.
    glBindTexture(GL_TEXTURE_2D, g_positionTexture[flip]);

    // Draw point sprites by blending them.
    glEnable(GL_BLEND);
    glDrawArrays(GL_POINTS, 0, PARTICLE_TEXTURE_WIDTH * PARTICLE_TEXTURE_WIDTH);
    glDisable(GL_BLEND);

    // Switch to the next buffer.
    flip = (flip + 1) % 2;

    return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (g_verticesVBO)
    {
        glDeleteBuffers(1, &g_verticesVBO);

        g_verticesVBO = 0;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (g_texture)
    {
        glDeleteTextures(1, &g_texture);

        g_texture = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (g_positionFramebuffer[0])
    {
        glDeleteFramebuffers(1, &g_positionFramebuffer[0]);

        g_positionFramebuffer[0] = 0;
    }

    if (g_positionFramebuffer[1])
    {
        glDeleteFramebuffers(1, &g_positionFramebuffer[1]);

        g_positionFramebuffer[1] = 0;
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (g_positionTexture[0])
    {
        glDeleteTextures(1, &g_positionTexture[0]);

        g_positionTexture[0] = 0;
    }

    if (g_positionTexture[1])
    {
        glDeleteTextures(1, &g_positionTexture[1]);

        g_positionTexture[1] = 0;
    }

    glBindVertexArray(0);

    if (g_vao)
    {
        glDeleteVertexArrays(1, &g_vao);

        g_vao = 0;
    }

    if (g_vaoUpdatePoints)
    {
        glDeleteVertexArrays(1, &g_vaoUpdatePoints);

        g_vaoUpdatePoints = 0;
    }

    glUseProgram(0);

    glusDestroyProgram(&g_program);
}

int main(int argc, char* argv[])
{
    glusInitFunc(init);

    glusReshapeFunc(reshape);

    glusUpdateFunc(update);

    glusTerminateFunc(terminate);

    glusPrepareContext(3, 2, GLUS_FORWARD_COMPATIBLE_BIT);

    if (!glusCreateWindow("GLUS Example Window", 640, 480, 0, 0, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
