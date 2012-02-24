/**
 * OpenGL 3 - Example 15
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "GL/glus.h"

#include "globals.h"
#include "renderBackground.h"
#include "renderWaterTexture.h"

GLfloat g_projectionMatrix[16];

GLfloat g_viewMatrix[16];

GLfloat g_inverseViewNormalMatrix[9];

//

GLUSshaderprogram g_program;

GLint g_projectionMatrixLocation;

GLint g_viewMatrixLocation;

GLint g_inverseViewNormalMatrixLocation;

GLint g_waterPlaneLengthLocation;

GLint g_passedTimeLocation;

GLint g_waveParametersLocation;

GLint g_waveDirectionsLocation;

GLint g_vertexLocation;

GLint g_cubemapLocation;

GLint g_waterTextureLocation;

//

GLuint g_vao;

GLuint g_verticesVBO;

GLuint g_indicesVBO;

//

GLuint g_cubemap;

GLUSboolean init(GLUSvoid)
{
    GLfloat* points = (GLfloat*) malloc(WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat));
    GLuint* indices = (GLuint*) malloc(WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint));

    GLUStgaimage image;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    GLuint x, z, i, k;

    GLuint waterTexture;

    for (z = 0; z < WATER_PLANE_LENGTH; z++)
    {
        for (x = 0; x < WATER_PLANE_LENGTH; x++)
        {
            points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 0] = -(GLfloat) WATER_PLANE_LENGTH / 2 + 0.5f + (GLfloat) x;
            points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 1] = 0.0f;
            points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 2] = +(GLfloat) WATER_PLANE_LENGTH / 2 - 0.5f - (GLfloat) z;
            points[(x + z * (WATER_PLANE_LENGTH)) * 4 + 3] = 1.0f;
        }
    }

    for (k = 0; k < WATER_PLANE_LENGTH - 1; k++)
    {
        for (i = 0; i < WATER_PLANE_LENGTH; i++)
        {
            if (k % 2 == 0)
            {
                indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 0] = i + (k + 1) * WATER_PLANE_LENGTH;
                indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 1] = i + k * WATER_PLANE_LENGTH;
            }
            else
            {
                indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 0] = WATER_PLANE_LENGTH - 1 - i + k * WATER_PLANE_LENGTH;
                indices[(i + k * (WATER_PLANE_LENGTH)) * 2 + 1] = WATER_PLANE_LENGTH - 1 - i + (k + 1) * WATER_PLANE_LENGTH;
            }
        }
    }

    //

    glusLoadTextFile("../Example15/shader/Water.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example15/shader/Water.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    g_projectionMatrixLocation = glGetUniformLocation(g_program.program, "u_projectionMatrix");
    g_viewMatrixLocation = glGetUniformLocation(g_program.program, "u_viewMatrix");
    g_inverseViewNormalMatrixLocation = glGetUniformLocation(g_program.program, "u_inverseViewNormalMatrix");

    g_waterPlaneLengthLocation = glGetUniformLocation(g_program.program, "u_waterPlaneLength");

    g_cubemapLocation = glGetUniformLocation(g_program.program, "u_cubemap");

    g_waterTextureLocation = glGetUniformLocation(g_program.program, "u_waterTexture");

    g_passedTimeLocation = glGetUniformLocation(g_program.program, "u_passedTime");

    g_waveParametersLocation = glGetUniformLocation(g_program.program, "u_waveParameters");
    g_waveDirectionsLocation = glGetUniformLocation(g_program.program, "u_waveDirections");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");

    //

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, WATER_PLANE_LENGTH * WATER_PLANE_LENGTH * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2 * sizeof(GLuint), (GLuint*) indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //

    free(points);
    free(indices);

    //

    glGenTextures(1, &g_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

    glusLoadTgaImage("water_pos_x.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("water_neg_x.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("water_pos_y.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("water_neg_y.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("water_pos_z.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("water_neg_z.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //

    waterTexture = initWaterTexture((GLUSfloat) WATER_PLANE_LENGTH);

    glUseProgram(g_program.program);

    glUniform1f(g_waterPlaneLengthLocation, (GLUSfloat) WATER_PLANE_LENGTH);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);
    glUniform1i(g_cubemapLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, waterTexture);
    glUniform1i(g_waterTextureLocation, 1);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    //

    initBackground();

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    glViewport(0, 0, width, height);

    glusPerspectivef(g_projectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 1000.0f);

    reshapeBackground(g_projectionMatrix);

    reshapeWaterTexture(width, height);

    glUseProgram(g_program.program);

    glUniformMatrix4fv(g_projectionMatrixLocation, 1, GL_FALSE, g_projectionMatrix);
}

GLUSvoid renderWater(GLUSfloat passedTime)
{
    static WaveParameters waveParameters[NUMBERWAVES];
    static WaveDirections waveDirections[NUMBERWAVES];

    static GLfloat overallSteepness = 0.2f;

    memset(waveParameters, 0, sizeof(waveParameters));
    memset(waveDirections, 0, sizeof(waveDirections));

    // Waves can be faded in and out.

    // Wave One
    waveParameters[0].speed = 1.0f;
    waveParameters[0].amplitude = 0.01f;
    waveParameters[0].wavelength = 4.0f;
    waveParameters[0].steepness = overallSteepness / (waveParameters[0].wavelength * waveParameters[0].amplitude * (GLfloat) NUMBERWAVES);
    waveDirections[0].x = +1.0f;
    waveDirections[0].z = +1.0f;

    // Wave Two
    waveParameters[1].speed = 0.5f;
    waveParameters[1].amplitude = 0.02f;
    waveParameters[1].wavelength = 3.0f;
    waveParameters[1].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
    waveDirections[1].x = +1.0f;
    waveDirections[1].z = +0.0f;

    // Wave Three
    waveParameters[2].speed = 0.1f;
    waveParameters[2].amplitude = 0.015f;
    waveParameters[2].wavelength = 2.0f;
    waveParameters[2].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
    waveDirections[2].x = -0.1f;
    waveDirections[2].z = -0.2f;

    // Wave Four
    waveParameters[3].speed = 1.1f;
    waveParameters[3].amplitude = 0.008f;
    waveParameters[3].wavelength = 1.0f;
    waveParameters[3].steepness = overallSteepness / (waveParameters[1].wavelength * waveParameters[1].amplitude * (GLfloat) NUMBERWAVES);
    waveDirections[3].x = -0.2f;
    waveDirections[3].z = -0.1f;

    glUseProgram(g_program.program);

    glUniformMatrix4fv(g_viewMatrixLocation, 1, GL_FALSE, g_viewMatrix);
    glUniformMatrix3fv(g_inverseViewNormalMatrixLocation, 1, GL_FALSE, g_inverseViewNormalMatrix);

    glUniform1f(g_passedTimeLocation, passedTime);

    glUniform4fv(g_waveParametersLocation, 4 * NUMBERWAVES, (GLfloat*) waveParameters);
    glUniform2fv(g_waveDirectionsLocation, 2 * NUMBERWAVES, (GLfloat*) waveDirections);

    glBindVertexArray(g_vao);

    glDrawElements(GL_TRIANGLE_STRIP, WATER_PLANE_LENGTH * (WATER_PLANE_LENGTH - 1) * 2, GL_UNSIGNED_INT, 0);
}

GLUSboolean update(GLUSfloat time)
{
    static GLfloat passedTime = 0.0f;

    static GLUSfloat angle = 0.0f;

    GLfloat inverseViewMatrix[16];

    glusLookAtf(g_viewMatrix, 0.0f, 1.0f, 0.0f, (GLfloat) 0.5f * sinf(angle), 1.0f, -(GLfloat) 0.5f * cosf(angle), 0.0f, 1.0f, 0.0f);

    glusMatrixCopyf(inverseViewMatrix, g_viewMatrix, GLUS_TRUE);
    glusMatrixInverseRigidBodyf(inverseViewMatrix);
    glusMatrixExtract3x3f(g_inverseViewNormalMatrix, inverseViewMatrix);

    // Render the background
    renderBackground(g_viewMatrix);

    // Render the water texture
    renderWaterTexture(passedTime);

    // Render the water scene
    renderWater(passedTime);

    passedTime += time;

    angle += 2.0f * PIf / 120.0f * time;

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (g_indicesVBO)
    {
        glDeleteBuffers(1, &g_indicesVBO);

        g_indicesVBO = 0;
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (g_cubemap)
    {
        glDeleteTextures(1, &g_cubemap);

        g_cubemap = 0;
    }

    glBindVertexArray(0);

    if (g_vao)
    {
        glDeleteVertexArrays(1, &g_vao);

        g_vao = 0;
    }

    glUseProgram(0);

    glusDestroyProgram(&g_program);

    //

    terminateBackground();

    terminateWaterTexture();
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

    if (!glusCreateWindow("GLUS Example Window", 640, 480, 16, 0, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
