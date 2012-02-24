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

#include <stdio.h>

#include "GL/glus.h"

GLUSshaderprogram g_program;

//

GLint g_projectionMatrixLocation;

GLint g_modelViewMatrixLocation;

GLint g_normalMatrixLocation;

/**
 * Inverse view matrix needed to go from view space back to world space.
 */
GLint g_inverseViewMatrixLocation;

GLint g_vertexLocation;

GLint g_normalLocation;

/**
 * The location of the cube map texture.
 */
GLint g_cubemapTextureLocation;

//

GLfloat g_viewMatrix[16];

//

GLuint g_verticesVBO;

GLuint g_normalsVBO;

GLuint g_indicesVBO;

GLuint g_vao;

//

/**
 * The cube map texture.
 */
GLuint g_cubemapTexture;

//

GLuint g_numberIndices;

GLUSboolean init(GLUSvoid)
{
    GLUSshape cube;

    GLUStgaimage image;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    glusLoadTextFile("../Example08/shader/cubemap.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example08/shader/cubemap.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    g_projectionMatrixLocation = glGetUniformLocation(g_program.program, "u_projectionMatrix");
    g_modelViewMatrixLocation = glGetUniformLocation(g_program.program, "u_modelViewMatrix");
    g_normalMatrixLocation = glGetUniformLocation(g_program.program, "u_normalMatrix");
    g_inverseViewMatrixLocation = glGetUniformLocation(g_program.program, "u_inverseViewMatrix");

    g_cubemapTextureLocation = glGetUniformLocation(g_program.program, "u_cubemapTexture");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    g_normalLocation = glGetAttribLocation(g_program.program, "a_normal");

    //

    // Here we create the cube map.

    glGenTextures(1, &g_cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemapTexture);

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

    glusCreateCubef(&cube, 0.5f);

    g_numberIndices = cube.numberIndices;

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.numberIndices * 4 * sizeof(GLfloat), (GLfloat*) cube.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, cube.numberIndices * 3 * sizeof(GLfloat), (GLfloat*) cube.normals, GL_STATIC_DRAW);

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    //

    // Activate and set the cube map.

    glUniform1i(g_cubemapTextureLocation, 0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, g_cubemapTexture);

    //

    // As the camera does not move, we can create the view matrix here.
    glusLookAtf(g_viewMatrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    GLfloat projectionMatrix[16];

    glViewport(0, 0, width, height);

    glusPerspectivef(projectionMatrix, 40.0f, (GLfloat) width / (GLfloat) height, 1.0f, 100.0f);

    glUniformMatrix4fv(g_projectionMatrixLocation, 1, GL_FALSE, projectionMatrix);
}

GLUSboolean update(GLUSfloat time)
{
    // Angle for rotation
    static GLfloat angle = 0.0f;

    // Matrix for the model
    GLfloat modelViewMatrix[16];
    GLfloat normalMatrix[9];
    GLfloat viewMatrix[9];

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Calculate the model matrix ...
    glusMatrixIdentityf(modelViewMatrix);
    // ... by finally rotating the cube.
    glusMatrixRotateRzRxRyf(modelViewMatrix, 0.0f, 15.0f, angle);

    // Create the model view matrix.
    glusMatrixMultiplyf(modelViewMatrix, g_viewMatrix, modelViewMatrix);

    // Again, extract the normal matrix. Remember, so far the model view matrix (rotation part) is orthogonal.
    glusMatrixExtract3x3f(normalMatrix, modelViewMatrix);

    glUniformMatrix4fv(g_modelViewMatrixLocation, 1, GL_FALSE, modelViewMatrix);

    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);

    //

    // Extract the rotation part of the view matrix.
    glusMatrixExtract3x3f(viewMatrix, g_viewMatrix);

    // Pass this matrix to the shader with the transpose flag. As the view matrix is orthogonal, the transposed is the inverse view matrix.
    glUniformMatrix3fv(g_inverseViewMatrixLocation, 1, GL_TRUE, viewMatrix);

    //

    glDrawElements(GL_TRIANGLES, g_numberIndices, GL_UNSIGNED_INT, 0);

    angle += 20.0f * time;

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

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (g_indicesVBO)
    {
        glDeleteBuffers(1, &g_indicesVBO);

        g_indicesVBO = 0;
    }

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    if (g_cubemapTexture)
    {
        glDeleteTextures(1, &g_cubemapTexture);

        g_cubemapTexture = 0;
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
