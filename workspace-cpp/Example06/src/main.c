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

#include <stdio.h>

#include "GL/glus.h"

GLUSshaderprogram g_program;

// The matrix locations in the program.

GLint g_viewProjectionMatrixLocation;

GLint g_modelMatrixLocation;

GLint g_normalMatrixLocation;

GLint g_lightDirectionLocation;

GLint g_vertexLocation;

GLint g_normalLocation;

GLint g_texCoordLocation;

GLint g_textureLocation;

//

GLuint g_verticesVBO;

GLuint g_normalsVBO;

GLuint g_texCoordsVBO;

GLuint g_indicesVBO;

//

GLuint g_vao;

//

GLuint g_texture;

//

GLuint g_numberIndices;

GLUSboolean init(GLUSvoid)
{
    GLfloat lightDirection[3] = { 1.0f, 1.0f, 1.0f };

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    GLUStgaimage image;

    GLUSshape cube;

    glusLoadTextFile("../Example06/shader/basic.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example06/shader/texture.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    // Retrieve the uniform locations in the program.
    g_viewProjectionMatrixLocation = glGetUniformLocation(g_program.program, "u_viewProjectionMatrix");
    g_modelMatrixLocation = glGetUniformLocation(g_program.program, "u_modelMatrix");
    g_normalMatrixLocation = glGetUniformLocation(g_program.program, "u_normalMatrix");
    g_lightDirectionLocation = glGetUniformLocation(g_program.program, "u_lightDirection");
    g_textureLocation = glGetUniformLocation(g_program.program, "u_texture");

    // Retrieve the attribute locations in the program.
    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_normalLocation = glGetAttribLocation(g_program.program, "a_normal");
    g_texCoordLocation = glGetAttribLocation(g_program.program, "a_texCoord");

    //

    // Texture set up.

    glusLoadTgaImage("crate.tga", &image);

    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    // Mipmap generation is now included in OpenGL 3 and above
    glGenerateMipmap(GL_TEXTURE_2D);

    // Trilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    //

    glusCreateCubef(&cube, 0.5f);

    g_numberIndices = cube.numberIndices;

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) cube.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) cube.normals, GL_STATIC_DRAW);

    glGenBuffers(1, &g_texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.numberVertices * 2 * sizeof(GLfloat), (GLfloat*) cube.texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, cube.numberIndices * sizeof(GLuint), (GLuint*) cube.indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glusDestroyShapef(&cube);

    //

    glUseProgram(g_program.program);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_texCoordLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    //

    glusVectorNormalizef(lightDirection);
    glUniform3fv(g_lightDirectionLocation, 1, lightDirection);

    //

    glBindTexture(GL_TEXTURE_2D, g_texture);
    glUniform1i(g_textureLocation, 0);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    GLfloat viewMatrix[16];
    GLfloat viewProjectionMatrix[16];

    glViewport(0, 0, width, height);

    glusLookAtf(viewMatrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glusPerspectivef(viewProjectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

    glusMatrixMultiplyf(viewProjectionMatrix, viewProjectionMatrix, viewMatrix);

    glUniformMatrix4fv(g_viewProjectionMatrixLocation, 1, GL_FALSE, viewProjectionMatrix);
}

GLUSboolean update(GLUSfloat time)
{
    static GLfloat angle = 0.0f;

    GLfloat modelMatrix[16];
    GLfloat normalMatrix[9];

    // 90 degrees per second. Rotate the texture cube along the y axis (yaw).
    angle += 90.0f * time;

    glusMatrixIdentityf(modelMatrix);
    glusMatrixRotateRzRxRyf(modelMatrix, 0.0f, 45.0f, angle);
    glUniformMatrix4fv(g_modelMatrixLocation, 1, GL_FALSE, modelMatrix);

    // Model matrix is a rigid body matrix.
    glusMatrixExtract3x3f(normalMatrix, modelMatrix);
    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

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

    if (g_normalsVBO)
    {
        glDeleteBuffers(1, &g_normalsVBO);

        g_normalsVBO = 0;
    }

    if (g_texCoordsVBO)
    {
        glDeleteBuffers(1, &g_texCoordsVBO);

        g_texCoordsVBO = 0;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (g_indicesVBO)
    {
        glDeleteBuffers(1, &g_indicesVBO);

        g_indicesVBO = 0;
    }

    glBindTexture(GL_TEXTURE_2D, 0);

	if (g_texture)
    {
        glDeleteTextures(1, &g_texture);

        g_texture = 0;
    }

    glBindVertexArray(0);

    if (g_vao)
    {
        glDeleteVertexArrays(1, &g_vao);

        g_vao = 0;
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

    if (!glusCreateWindow("GLUS Example Window", 640, 480, 16, 0, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
