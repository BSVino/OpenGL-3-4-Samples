/**
 * OpenGL 3 - Example 07
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <math.h>
#include <stdio.h>

#include "GL/glus.h"

GLUSshaderprogram g_program;

//

GLint g_projectionMatrixLocation;

GLint g_modelViewMatrixLocation;

GLint g_normalMatrixLocation;

//

GLint g_lightDirectionLocation;

GLint g_vertexLocation;

GLint g_tangentLocation;

GLint g_bitangentLocation;

GLint g_normalLocation;

GLint g_texCoordLocation;

//

GLint g_textureLocation;

GLint g_normalMapLocation;

//

GLfloat g_viewMatrix[16];

//

GLuint g_verticesVBO;

GLuint g_tangentsVBO;

GLuint g_bitangentsVBO;

GLuint g_normalsVBO;

GLuint g_texCoordsVBO;

GLuint g_indicesVBO;

//

GLuint g_vao;

//

GLuint g_texture;

GLuint g_normalMap;

//

GLuint g_numberIndices;

GLUSboolean init(GLUSvoid)
{
    GLUSshape plane;

    GLUStextfile vertexSource;
    GLUStextfile fragmentSource;

    GLUStgaimage image;

    GLfloat normalMatrix[9];

    glusLoadTextFile("../Example07/shader/normmap.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example07/shader/normmap.frag.glsl", &fragmentSource);

    glusBuildProgramFromSource(&g_program, (const GLUSchar**) &vertexSource.text, 0, 0, 0, (const GLUSchar**) &fragmentSource.text);

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&fragmentSource);

    //

    g_projectionMatrixLocation = glGetUniformLocation(g_program.program, "u_projectionMatrix");
    g_modelViewMatrixLocation = glGetUniformLocation(g_program.program, "u_modelViewMatrix");
    g_normalMatrixLocation = glGetUniformLocation(g_program.program, "u_normalMatrix");
    g_lightDirectionLocation = glGetUniformLocation(g_program.program, "u_lightDirection");

    // One texture for the color and one for the normals.
    g_textureLocation = glGetUniformLocation(g_program.program, "u_texture");
    g_normalMapLocation = glGetUniformLocation(g_program.program, "u_normalMap");

    g_vertexLocation = glGetAttribLocation(g_program.program, "a_vertex");
    // The tangent, bitangent and normal do define the tangent space. They are defined in object space, the inverse brings coordinates back to this tangent space.
    g_tangentLocation = glGetAttribLocation(g_program.program, "a_tangent");
    g_bitangentLocation = glGetAttribLocation(g_program.program, "a_bitangent");
    g_normalLocation = glGetAttribLocation(g_program.program, "a_normal");

    g_texCoordLocation = glGetAttribLocation(g_program.program, "a_texCoord");

    //

    glusLoadTgaImage("rock_color.tga", &image);

    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glusDestroyTgaImage(&image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);


    glusLoadTgaImage("rock_normal.tga", &image);

    glGenTextures(1, &g_normalMap);
    glBindTexture(GL_TEXTURE_2D, g_normalMap);
    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glusDestroyTgaImage(&image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    //

    glusCreatePlanef(&plane, 1.5f);

    g_numberIndices = plane.numberIndices;

    glGenBuffers(1, &g_verticesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 4 * sizeof(GLfloat), (GLfloat*) plane.vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &g_tangentsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_tangentsVBO);
    glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) plane.tangents, GL_STATIC_DRAW);

    glGenBuffers(1, &g_bitangentsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_bitangentsVBO);
    glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) plane.bitangents, GL_STATIC_DRAW);

    glGenBuffers(1, &g_normalsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 3 * sizeof(GLfloat), (GLfloat*) plane.normals, GL_STATIC_DRAW);

    glGenBuffers(1, &g_texCoordsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glBufferData(GL_ARRAY_BUFFER, plane.numberVertices * 2 * sizeof(GLfloat), (GLfloat*) plane.texCoords, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &g_indicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_numberIndices * sizeof(GLuint), (GLuint*) plane.indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glusDestroyShapef(&plane);

    //

    glUseProgram(g_program.program);

    glGenVertexArrays(1, &g_vao);
    glBindVertexArray(g_vao);

    glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
    glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_vertexLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_tangentsVBO);
    glVertexAttribPointer(g_tangentLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_tangentLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_bitangentsVBO);
    glVertexAttribPointer(g_bitangentLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_bitangentLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_normalsVBO);
    glVertexAttribPointer(g_normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_normalLocation);

    glBindBuffer(GL_ARRAY_BUFFER, g_texCoordsVBO);
    glVertexAttribPointer(g_texCoordLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(g_texCoordLocation);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesVBO);

    //

    glusLookAtf(g_viewMatrix, 0.0f, 0.0f, 5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    glUniformMatrix4fv(g_modelViewMatrixLocation, 1, GL_FALSE, g_viewMatrix);

    glusMatrixExtract3x3f(normalMatrix, g_viewMatrix);

    glUniformMatrix3fv(g_normalMatrixLocation, 1, GL_FALSE, normalMatrix);

    //

    // Activate and bind first ...
    glUniform1i(g_textureLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, g_texture);

    // .. and second texture.
    glUniform1i(g_normalMapLocation, 1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, g_normalMap);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
    static GLfloat angle = 0.0;

    // Light start direction, which will be animated for seeing the effect.
    GLfloat lightDirection[3] = { 1.0, 1.0f, 1.0f };

    // Increase the angle depending on the time.
    angle += time;

    // Let the light move from right to left and vice versa.
    lightDirection[0] = 2.0f * cosf(angle);
    // Need to normalize the light given in world space.
    glusVectorNormalizef(lightDirection);

    // Bring the light to camera space.
    glusMatrixMultiplyVectorf(lightDirection, g_viewMatrix, lightDirection);

    glUniform3fv(g_lightDirectionLocation, 1, (GLfloat*) lightDirection);

    glClear(GL_COLOR_BUFFER_BIT);

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

    if (g_tangentsVBO)
    {
        glDeleteBuffers(1, &g_tangentsVBO);

        g_tangentsVBO = 0;
    }

    if (g_bitangentsVBO)
    {
        glDeleteBuffers(1, &g_bitangentsVBO);

        g_bitangentsVBO = 0;
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

    if (g_texture)
    {
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDeleteTextures(1, &g_texture);

        g_texture = 0;
    }

    if (g_normalMap)
    {
        glActiveTexture(GL_TEXTURE1);

        glBindTexture(GL_TEXTURE_2D, 0);

        glDeleteTextures(1, &g_normalMap);

        g_normalMap = 0;
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

    if (!glusCreateWindow("GLUS Example Window", 640, 480, 0, 0, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    glusRun();

    return 0;
}
