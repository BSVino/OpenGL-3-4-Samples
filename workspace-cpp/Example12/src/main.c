/**
 * OpenGL 3 - Example 12
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
 * Width of the viewport, needed to reset from framebuffer.
 */
GLUSuint g_width;

/**
 * Height of the viewport.
 */
GLUSuint g_height;

GLUSfloat g_cameraPosition[3] = { -5.0f, 5.0f, 10.0f };

GLUSfloat g_lightPosition[3] = { 0.0f, 0.0f, 10.0f };

/**
 * Matrix, to convert from scene world space to shadow projection space.
 */
GLfloat g_shadowMatrix[16];

GLUSshaderprogram g_program;

GLint g_shadowMatrixLocation;

GLint g_projectionMatrixLocation;

GLint g_modelViewMatrixLocation;

GLint g_viewMatrixLocation;

GLint g_modelMatrixLocation;

GLint g_normalMatrixLocation;

GLint g_shadowTextureLocation;

GLint g_lightDirectionLocation;

GLint g_colorLocation;

GLint g_vertexLocation;

GLint g_normalLocation;

//

GLUSshaderprogram g_programShadow;

GLint g_projectionMatrixShadowLocation;

GLint g_modelViewMatrixShadowLocation;

GLint g_vertexShadowLocation;

//

GLuint g_verticesVBO;

GLuint g_normalsVBO;

GLuint g_indicesVBO;

GLuint g_verticesBackgroundVBO;

GLuint g_normalsBackgroundVBO;

GLuint g_indicesBackgroundVBO;

GLuint g_vao;

GLuint g_vaoBackground;

GLuint g_vaoShadow;

GLuint g_vaoShadowBackground;

//

GLuint g_numberIndices;

GLuint g_numberIndicesBackground;

//

/**
 * The shadow texture size.
 */
GLuint g_shadowTextureSize = 1024;

/**
 * The shadow texture.
 */
GLuint g_shadowTexture;

/**
 * The frame buffer object for the shadow texture.
 */
GLuint g_fbo;

GLUSboolean init(GLUSvoid)
{
    GLUSshape background;

    GLUSshape torus;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    GLfloat viewMatrix[16];

    GLfloat lightDirection[3];

    lightDirection[0] = g_lightPosition[0];
    lightDirection[1] = g_lightPosition[1];
    lightDirection[2] = g_lightPosition[2];

    glusVectorNormalizef(lightDirection);

    //

    glusLoadTextFile("../Example12/shader/rendershadow.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example12/shader/rendershadow.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_programShadow, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    glusLoadTextFile("../Example12/shader/useshadow.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example12/shader/useshadow.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    g_projectionMatrixShadowLocation = glGetUniformLocation(g_programShadow.program, "u_projectionMatrix");
    g_modelViewMatrixShadowLocation = glGetUniformLocation(g_programShadow.program, "u_modelViewMatrix");
    g_vertexShadowLocation = glGetAttribLocation(g_programShadow.program, "a_vertex");

    //

    g_projectionMatrixLocation = glGetUniformLocation(g_program.program, "u_projectionMatrix");
    g_viewMatrixLocation = glGetUniformLocation(g_program.program, "u_viewMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    g_normalMatrixLocation = glGetUniformLocation(g_program.program, "u_normalMatrix");
    g_shadowMatrixLocation = glGetUniformLocation(g_program.program, "u_shadowMatrix");
    g_shadowTextureLocation = glGetUniformLocation(g_program.program, "u_shadowTexture");
    g_colorLocation = glGetUniformLocation(g_program.program, "u_shapeColor");
    g_lightDirectionLocation = glGetUniformLocation(g_program.program, "u_lightDirection");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_normalLocation = glGetAttribLocation(g_program.program, "a_normal");

    //

    glGenTextures(1, &g_shadowTexture);
    glBindTexture(GL_TEXTURE_2D, g_shadowTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, g_shadowTextureSize, g_shadowTextureSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindTexture(GL_TEXTURE_2D, 0);

    //

    glGenFramebuffers(1, &g_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, g_shadowTexture, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("GL_FRAMEBUFFER_COMPLETE error 0x%x", glCheckFramebufferStatus(GL_FRAMEBUFFER));

        return GLUS_FALSE;
    }

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //

    glusCreateTorusf(&torus, 0.5f, 1.0f, 32, 32);
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

    glusCreatePlanef(&background, 10.0f);
    g_numberIndicesBackground = background.numberIndices;

    glGenBuffers(1, &g_verticesBackgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) background.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsBackgroundVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, background.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) background.normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesBackgroundVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, background.numberIndices * sizeof(GLuint), (GLuint*) background.indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glusDestroyShapef(&background);

    //

    glUseProgram(g_program.program);

    glusLookAtf(viewMatrix, g_cameraPosition[0], g_cameraPosition[1], g_cameraPosition[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glusMatrixMultiplyVectorf(lightDirection, viewMatrix, lightDirection);

    glUniform3fv(g_lightDirectionLocation, 1, lightDirection);

    glUniform1i(g_shadowTextureLocation, 0);

    // Torus

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_normalLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    // Plane

    glGenVertexArrays(1, &g_vaoBackground);
    glBindVertexArray(g_vaoBackground);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_normalsBackgroundVBO);
    glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_normalLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);

    //

    glUseProgram(g_programShadow.program);

    // Torus

    glGenVertexArrays(1, &g_vaoShadow);
    glBindVertexArray(g_vaoShadow);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexShadowLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexShadowLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    // Plane

    glGenVertexArrays(1, &g_vaoShadowBackground);
    glBindVertexArray(g_vaoShadowBackground);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesBackgroundVBO);
    glVertexAttribPointer(g_vertexShadowLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexShadowLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesBackgroundVBO);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    static GLfloat bias[] = { 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f };

    GLfloat projectionMatrix[16];

    g_width = width;

    g_height = height;

    //

    glUseProgram(g_programShadow.program);

    glusPerspectivef(projectionMatrix, 40.0f, (GLfloat) g_shadowTextureSize / (GLfloat) g_shadowTextureSize, 1.0f, 100.0f);

    glUniformMatrix4fv(g_projectionMatrixShadowLocation, 1, GL_FALSE, projectionMatrix);

    glusMatrixIdentityf(g_shadowMatrix);
    glusMatrixMultiplyf(g_shadowMatrix, g_shadowMatrix, bias);
    glusMatrixMultiplyf(g_shadowMatrix, g_shadowMatrix, projectionMatrix);

    //

    glUseProgram(g_program.program);

    glusPerspectivef(projectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

    glUniformMatrix4fv(g_projectionMatrixLocation, 1, GL_FALSE, projectionMatrix);
}

GLUSboolean update(GLUSfloat time)
{
    static GLfloat angle = 0.0f;

    GLfloat shadowMatrix[16];
    GLfloat modelViewMatrix[16];
    GLfloat viewMatrix[16];
    GLfloat modelMatrix[16];
    GLfloat normalMatrix[9];

    // Rendering into the shadow texture.

    glBindTexture(GL_TEXTURE_2D, 0);

    // Setup for the framebuffer.
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    glViewport(0, 0, g_shadowTextureSize, g_shadowTextureSize);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    glusLookAtf(viewMatrix, g_lightPosition[0], g_lightPosition[1], g_lightPosition[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glusMatrixMultiplyf(shadowMatrix, g_shadowMatrix, viewMatrix);

    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_programShadow.program);

    // Render the background plane.

    glusMatrixIdentityf(modelMatrix);
    glusMatrixTranslatef(modelMatrix, 0.0f, 0.0f, -5.0f);
    glusMatrixMultiplyf(modelViewMatrix, viewMatrix, modelMatrix);

    glUniformMatrix4fv(g_modelViewMatrixShadowLocation, 1, GL_FALSE, modelViewMatrix);

    glBindVertexArray(g_vaoShadowBackground);
    glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

    // Render the torus.

    glusMatrixIdentityf(modelMatrix);
    glusMatrixRotateRzRxRyf(modelMatrix, 0.0f, 0.0f, angle);
    glusMatrixMultiplyf(modelViewMatrix, viewMatrix, modelMatrix);

    glUniformMatrix4fv(g_modelViewMatrixShadowLocation, 1, GL_FALSE, modelViewMatrix);

    glBindVertexArray(g_vaoShadow);
    glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

    // Revert for the scene.
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glViewport(0, 0, g_width, g_height);

    glBindTexture(GL_TEXTURE_2D, g_shadowTexture);

    //

    // Render the scene.

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(g_program.program);

    glusLookAtf(viewMatrix, g_cameraPosition[0], g_cameraPosition[1], g_cameraPosition[2], 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glUniformMatrix4fv(g_viewMatrixLocation, 1, GL_FALSE, viewMatrix);
    glUniformMatrix4fv(g_shadowMatrixLocation, 1, GL_FALSE, shadowMatrix);

    // Plane
    glusMatrixIdentityf(modelMatrix);
    glusMatrixTranslatef(modelMatrix, 0.0f, 0.0f, -5.0f);
    glusMatrixMultiplyf(modelViewMatrix, viewMatrix, modelMatrix);
    glusMatrixExtract3x3f(normalMatrix, modelViewMatrix);

    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);
    glUniform4f(g_colorLocation, 0.0f, 0.5f, 0.0f, 1.0f);

    glBindVertexArray(g_vaoBackground);
    glDrawElements(GL_TRIANGLES, g_numberIndicesBackground, GL_UNSIGNED_INT, 0);

    // Torus
    glusMatrixIdentityf(modelMatrix);
    glusMatrixRotateRzRxRyf(modelMatrix, 0.0f, 0.0f, angle);
    glusMatrixMultiplyf(modelViewMatrix, viewMatrix, modelMatrix);
    glusMatrixExtract3x3f(normalMatrix, modelViewMatrix);

    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);
    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);
    glUniform4f(g_colorLocation, 0.33f, 0.0f, 0.5f, 1.0f);

    glBindVertexArray(g_vao);
    glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

    //

    angle += 20.0f * time;

    return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
    glBindTexture(GL_TEXTURE_2D, 0);

    if (g_shadowTexture)
    {
        glDeleteRenderbuffers(1, &g_shadowTexture);

        g_shadowTexture = 0;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    if (g_fbo)
    {
        glDeleteFramebuffers(1, &g_fbo);

        g_fbo = 0;
    }

    //

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

    //

    if (g_verticesBackgroundVBO)
    {
        glDeleteBuffers(1, &g_verticesBackgroundVBO);

        g_verticesBackgroundVBO = 0;
    }

    if (g_normalsBackgroundVBO)
    {
        glDeleteBuffers(1, &g_normalsBackgroundVBO);

        g_normalsBackgroundVBO = 0;
    }

    if (g_indicesBackgroundVBO)
    {
        glDeleteBuffers(1, &g_indicesBackgroundVBO);

        g_indicesBackgroundVBO = 0;
    }

    if (g_vaoBackground)
    {
        glDeleteVertexArrays(1, &g_vaoBackground);

        g_vao = 0;
    }

    glUseProgram(0);

    glusDestroyProgram(&g_program);

    //

    if (g_vaoShadow)
    {
        glDeleteVertexArrays(1, &g_vaoShadow);

        g_vaoShadow = 0;
    }

    if (g_vaoShadowBackground)
    {
        glDeleteVertexArrays(1, &g_vaoShadowBackground);

        g_vaoShadowBackground = 0;
    }

    glusDestroyProgram(&g_programShadow);
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
