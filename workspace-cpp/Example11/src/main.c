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

#include <stdio.h>

#include <math.h>

#include "GL/glus.h"

GLUSshaderprogram g_program;

GLint g_viewProjectionMatrixLocation;

GLint g_modelMatrixLocation;

GLint g_normalMatrixLocation;

GLint g_cameraLocation;

GLint g_vertexLocation;

GLint g_normalLocation;

GLint g_cubemapLocation;

GLUSshaderprogram g_programBackground;

GLint g_viewProjectionMatrixBackgroundLocation;

GLint g_modelMatrixBackgroundLocation;

GLint g_vertexBackgroundLocation;

GLint g_cubemapBackgroundLocation;

//

GLfloat g_projectionMatrix[16];

//

GLuint g_cubemap;

//

GLuint g_verticesVBO;

GLuint g_normalsVBO;

GLuint g_indicesVBO;

GLuint g_vao;

//

GLuint g_verticesBackgroundVBO;

GLuint g_indicesBackgroundVBO;

GLuint g_vaoBackground;

//

GLuint g_numberIndices;

GLuint g_numberIndicesBackground;

//

GLfloat g_circleRadius = 5.0f;

GLUSboolean init(GLUSvoid)
{
    GLUSshape torus;

    GLUSshape backgroundSphere;

    GLUStgaimage image;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    glusLoadTextFile("../Example11/shader/glass.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example11/shader/glass.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    glusLoadTextFile("../Example11/shader/background.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example11/shader/background.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_programBackground, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    g_normalMatrixLocation = glGetUniformLocation(g_program.program, "u_normalMatrix");
    g_cameraLocation = glGetUniformLocation(g_program.program, "u_camera");
    g_cubemapLocation = glGetUniformLocation(g_program.program, "u_cubemap");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_normalLocation = glGetAttribLocation(g_program.program, "a_normal");

    //

    g_viewProjectionMatrixBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_viewProjectionMatrix");
    g_modelMatrixBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_modelMatrix");
    g_cubemapBackgroundLocation = glGetUniformLocation(g_programBackground.program, "u_cubemap");

    g_vertexBackgroundLocation = glGetAttribLocation(g_programBackground.program, "a_vertex");

    //

    glGenTextures(1, &g_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

    glusLoadTgaImage("cm_left.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("cm_right.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("cm_top.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("cm_bottom.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("cm_back.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glusLoadTgaImage("cm_front.tga", &image);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);
    glusDestroyTgaImage(&image);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    //

    glusCreateTorusf(&torus, 0.25f, 1.0f, 32, 32);
    g_numberIndices = torus.numberIndices;

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, torus.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) torus.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, torus.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) torus.normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, torus.numberIndices * sizeof(GLuint), (GLuint*) torus.indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glusDestroyShapef(&torus);

    //

    glusCreateSpheref(&backgroundSphere, g_circleRadius, 32);
    g_numberIndicesBackground = backgroundSphere.numberIndices;

    glGenBuffers(1, &g_verticesBackgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, backgroundSphere.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) backgroundSphere.vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesBackgroundVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, backgroundSphere.numberIndices * sizeof(GLuint), (GLuint*) backgroundSphere.indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glusDestroyShapef(&backgroundSphere);

    //

    glUseProgram(g_program.program);

    glUniform1i(g_cubemapLocation, 0);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_normalLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    //

    glUseProgram(g_programBackground.program);

    glUniform1i(g_cubemapBackgroundLocation, 0);

    glGenVertexArrays(1, &g_vaoBackground);
    glBindVertexArray(g_vaoBackground);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
    glVertexAttribPointer(g_vertexBackgroundLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexBackgroundLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);

    //

    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemap);

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

    glusPerspectivef(g_projectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);
}

GLUSboolean update(GLUSfloat time)
{
    static GLfloat angle = 0.0f;

    GLfloat angleRadians;

    GLfloat viewProjectionMatrix[16];
    GLfloat viewMatrix[16];
    GLfloat modelMatrix[16];
    GLfloat normalMatrix[9];

    GLfloat camera[4] = {0.0, 0.0, 0.0, 1.0};

    angleRadians = glusDegToRadf(angle);

    camera[0] = g_circleRadius * -sinf(angleRadians);
    camera[2] = g_circleRadius * cosf(angleRadians);

    // Circle with the camera around the origin by looking at it.
    glusLookAtf(viewMatrix, camera[0], 0.0f, camera[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glusMatrixMultiplyf(viewProjectionMatrix, g_projectionMatrix, viewMatrix);

    glusMatrixIdentityf(modelMatrix);

    glusMatrixTranslatef(modelMatrix, 0.0f, -0.5f, 0.0f);
    glusMatrixRotateRxf(modelMatrix, 45.0f);

    glusMatrixExtract3x3f(normalMatrix, modelMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //

    // First render the background.

    glUseProgram(g_programBackground.program);

    glUniformMatrix4fv(g_viewProjectionMatrixBackgroundLocation, 1, GL_FALSE, viewProjectionMatrix);

    glUniformMatrix4fv(g_modelMatrixBackgroundLocation, 1, GL_FALSE, modelMatrix);

    glBindVertexArray(g_vaoBackground);

    glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

    // Now render the sphere.

    glUseProgram(g_program.program);

    glUniformMatrix4fv(g_viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMatrix);

    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);

    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);

    glUniform4fv(g_cameraLocation, 1, camera);

    glBindVertexArray(g_vao);

    glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

    // Increase the angle 30 degree per second.
    angle += 30.0f * time;

    return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (g_cubemap)
    {
        glDeleteTextures(1, &g_cubemap);

        g_cubemap = 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (g_verticesVBO)
    {
        glDeleteBuffers(1, &g_verticesVBO);

        g_verticesVBO = 0;
    }

    if (g_normalsVBO)
    {
        glDeleteBuffers(1, &g_normalsVBO);

        g_normalsVBO = 0;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (g_indicesVBO)
    {
        glDeleteBuffers(1, &g_indicesVBO);

        g_indicesVBO = 0;
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

    if (g_verticesBackgroundVBO)
    {
        glDeleteBuffers(1, &g_verticesBackgroundVBO);

        g_verticesBackgroundVBO = 0;
    }

    if (g_indicesBackgroundVBO)
    {
        glDeleteBuffers(1, &g_indicesBackgroundVBO);

        g_indicesBackgroundVBO = 0;
    }

    if (g_vaoBackground)
    {
        glDeleteVertexArrays(1, &g_vaoBackground);

        g_vaoBackground = 0;
    }

    glusDestroyProgram(&g_programBackground);
}

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
