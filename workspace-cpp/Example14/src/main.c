/**
 * OpenGL 4 - Example 14
 *
 * Method for Real-Time LOD Terrain Rendering on Modern GPU
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	05.02.2012
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

#include <stdio.h>

#include <math.h>

#include <stdlib.h>

#include "GL/glus.h"

/**
 * gl_Position will be transformed to the buffer
 */

const char* TRANSFORM_VARYING = "v_vertex";

//
// Map metrics etc.
//

/**
 * One turn / cycle around takes x seconds.
 */
const GLfloat TURN_DURATION = 20.0f;

/**
 * The circle around radius.
 */
const GLfloat TURN_RADIUS = 6000.0f;

/**
 * The space when traveling horizontal form one pixel to the next in meters.
 */
const GLfloat HORIZONTAL_PIXEL_SPACING = 60.0f;

/**
 * The space when traveling all color pixel in meters.
 */
const GLfloat VERTICAL_PIXEL_RANGE = 10004.0f;

/**
 * The scale to convert the real world meters in virtual world scale.
 */
const GLfloat METERS_TO_VIRTUAL_WORLD_SCALE = 5.0f;

/**
 * Detail level at the beginning of the map.
 */
const GLuint MINIMUM_DETAIL_LEVEL = 4;

/**
 * Additional detail level in the first pass. Adjust in GeometryPassOne max_vertices = 4^(firstPassDetailLevel+1)
 */
const GLuint DETAIL_LEVEL_FIRST_PASS = 2;

/**
 * FOV radius
 */
const GLfloat FOV_RADIUS = 10000.0f;

/**
 * Number of quadrants when going to the next detail level.
 */
const GLint QUADRANT_STEP = 2;

/**
 * The height map of the terrain. Width and height does not have to be the same but they have to be power of two plus one.
 */
const char* HEIGHT_MAP = "grand_canyon_height.tga";

/**
 * The color map of the terrain.
 */
const char* COLOR_MAP = "grand_canyon_color.tga";

/**
 * The normal map of the terrain. Width and height does not have to be the same but they have to be power of two plus one.
 */
const char* NORMAL_MAP = "grand_canyon_normal.tga";

/**
 * Flag, if animation is on or off
 */
GLboolean g_animationOn = GLUS_TRUE;

/**
 * The extend of the map in s coordinate direction. This is basically the width of the texture.
 */
GLfloat g_sMapExtend;

/**
 * The extend of the map in t coordinate direction. Basically the height.
 */
GLfloat g_tMapExtend;

/**
 * Number of points in s direction.
 */
GLuint g_sNumPoints;

/**
 * Number of points in t direction.
 */
GLuint g_tNumPoints;

//
// Buffer objects etc.
//

GLuint g_vaoPassOne = 0;

GLuint g_verticesPassOneVBO = 0;

GLuint g_indicesPassOneVBO = 0;

GLuint g_vaoPassTwo = 0;

GLuint g_verticesPassTwoVBO = 0;

//

GLUSshaderprogram g_programPassOne;

GLint g_halfDetailStepPassOneLocation;

GLint g_detailLevelPassOneLocation;

GLint g_positionTextureSpacePassOneLocation;

GLint g_fovRadiusPassOneLocation;

GLint g_leftNormalTextureSpacePassOneLocation;

GLint g_rightNormalTextureSpacePassOneLocation;

GLint g_backNormalTextureSpacePassOneLocation;

GLUSshaderprogram g_shaderProgramPassTwo;

//

GLint g_tmvpPassTwoLocation;

GLint g_maxTessellationLevelPassTwoLocation;

GLint g_quadrantStepPassTwoLocation;

GLint g_positionTextureSpacePassTwoLocation;

GLint g_heightMapTexturePassTwoLocation;

GLint g_colorMapTexturePassTwoLocation;

GLint g_normalMapTexturePassTwoLocation;

GLint g_lightDirectionPassTwoLocation;

//

GLfloat g_projectionMatrix[16];

GLfloat g_viewMatrix[16];

GLfloat g_textureToWorldMatrix[16];

GLfloat g_worldToTextureMatrix[16];

GLfloat g_worldToTextureNormalMatrix[16];

//

/**
 * Variable to query how much primitives were written.
 */
GLuint g_transformFeedbackQuery;

//

GLuint g_normalMapTexture;

GLuint g_heightMapTexture;

GLuint g_colorMapTexture;

//

typedef struct _ViewData
{
    GLfloat cameraPosition[4];
    GLfloat cameraDirection[3];
    GLfloat cameraUp[3];
    GLfloat fov;
} ViewData;

ViewData g_topView;

ViewData g_personView;

ViewData* g_activeView;

//

GLfloat g_width;

GLfloat g_height;

GLUSboolean init(GLUSvoid)
{
    GLfloat textureToWorldNormalMatrix[16];

    // The maximum detail level which is 2^s = sMapExtend.
    GLuint sMaxDetailLevel;

    // The maximum detail level which is 2^t = tMapExtend.
    GLuint tMaxDetailLevel;

    // The overall maximum detail level from s and t.
    GLuint overallMaxDetailLevel;

    // Step for s and t direction.
    GLfloat detailStep;

    GLuint s, t;

    GLUStgaimage image;

    GLfloat* map = 0;

    GLuint* indices = 0;

    GLUStextfile vertexSource;
    GLUStextfile controlSource;
    GLUStextfile evaluationSource;
    GLUStextfile geometrySource;
    GLUStextfile fragmentSource;

    GLfloat lightDirection[3] = { 1.0f, 1.0f, 1.0f };

    glusVectorNormalizef(lightDirection);

    g_topView.cameraPosition[0] = 0.0f;
    g_topView.cameraPosition[1] = 30000.0f * METERS_TO_VIRTUAL_WORLD_SCALE;
    g_topView.cameraPosition[2] = 0.0f;
    g_topView.cameraPosition[3] = 1.0;
    g_topView.cameraDirection[0] = 0.0f;
    g_topView.cameraDirection[1] = -1.0f;
    g_topView.cameraDirection[2] = 0.0f;
    g_topView.cameraUp[0] = 0.0f;
    g_topView.cameraUp[1] = 0.0f;
    g_topView.cameraUp[2] = -1.0f;
    g_topView.fov = 40.0f;

    g_personView.cameraPosition[0] = 0.0f;
    g_personView.cameraPosition[1] = 4700.0f * METERS_TO_VIRTUAL_WORLD_SCALE;
    g_personView.cameraPosition[2] = 0.0f;
    g_personView.cameraPosition[3] = 1.0;
    g_personView.cameraDirection[0] = 0.0f;
    g_personView.cameraDirection[1] = 0.0f;
    g_personView.cameraDirection[2] = -1.0f;
    g_personView.cameraUp[0] = 0.0f;
    g_personView.cameraUp[1] = 1.0f;
    g_personView.cameraUp[2] = 0.0f;
    g_personView.fov = 60.0f;

    g_activeView = &g_personView;

    if (!glusLoadTgaImage(NORMAL_MAP, &image))
    {
        printf("Could not load normal picture '%s'!\n", NORMAL_MAP);

        return GLUS_FALSE;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &g_normalMapTexture);

    glBindTexture(GL_TEXTURE_RECTANGLE, g_normalMapTexture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glusDestroyTgaImage(&image);

    if (!glusLoadTgaImage(HEIGHT_MAP, &image))
    {
        printf("Could not load height picture '%s'!\n", HEIGHT_MAP);

        return GLUS_FALSE;
    }

    glGenTextures(1, &g_heightMapTexture);

    glBindTexture(GL_TEXTURE_RECTANGLE, g_heightMapTexture);

    glTexImage2D(GL_TEXTURE_RECTANGLE, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    g_sMapExtend = (GLfloat) image.width;

    g_tMapExtend = (GLfloat) image.height;

    glusDestroyTgaImage(&image);

    // Calculate the detail level for the s and ...
    sMaxDetailLevel = (GLuint) floorf(logf(g_sMapExtend) / logf(2.0f));

    // ... t extend
    tMaxDetailLevel = (GLuint) floorf(logf(g_tMapExtend) / logf(2.0f));

    overallMaxDetailLevel = glusMinf(sMaxDetailLevel, tMaxDetailLevel);

    // Do checking of calculated parameters
    if (MINIMUM_DETAIL_LEVEL > overallMaxDetailLevel)
    {
        printf("Detail level to high %d > %d\n", MINIMUM_DETAIL_LEVEL, overallMaxDetailLevel);

        return GLUS_FALSE;
    }

    if (MINIMUM_DETAIL_LEVEL + DETAIL_LEVEL_FIRST_PASS > overallMaxDetailLevel)
    {
        printf("First pass detail level to high %d > %d\n", MINIMUM_DETAIL_LEVEL + DETAIL_LEVEL_FIRST_PASS, overallMaxDetailLevel);

        return GLUS_FALSE;
    }

    if (powf(2.0f, overallMaxDetailLevel - (MINIMUM_DETAIL_LEVEL + DETAIL_LEVEL_FIRST_PASS)) > 32.0f)
    {
        printf("Tessellation level to high %d > 32\n", (GLint) powf(2.0f, overallMaxDetailLevel - (MINIMUM_DETAIL_LEVEL + DETAIL_LEVEL_FIRST_PASS)));

        return GLUS_FALSE;
    }

    detailStep = powf(2.0f, overallMaxDetailLevel - MINIMUM_DETAIL_LEVEL);

    g_sNumPoints = (GLuint) ceilf(g_sMapExtend / detailStep) - 1;

    g_tNumPoints = (GLuint) ceilf(g_tMapExtend / detailStep) - 1;

    //
    // Generate the flat terrain mesh.
    //

    map = (GLUSfloat*) malloc(g_sNumPoints * g_tNumPoints * 2 * sizeof(GLfloat));

    indices = (GLuint*) malloc(g_sNumPoints * g_tNumPoints * sizeof(GLuint));

    for (t = 0; t < g_tNumPoints; t++)
    {
        for (s = 0; s < g_sNumPoints; s++)
        {
            map[t * g_sNumPoints * 2 + s * 2 + 0] = 0.5f + detailStep / 2.0f + (GLfloat) s * detailStep;
            map[t * g_sNumPoints * 2 + s * 2 + 1] = 0.5f + detailStep / 2.0f + (GLfloat) t * detailStep;

            indices[t * g_sNumPoints + s + 0] = (t + 0) * g_sNumPoints + s + 0;
        }
    }

    //
    // Transferring vertices and indices into GPU
    //

    // Pass one

    glGenVertexArrays(1, &g_vaoPassOne);
    glBindVertexArray(g_vaoPassOne);

    glGenBuffers(1, &g_verticesPassOneVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesPassOneVBO);
    glBufferData(GL_ARRAY_BUFFER, g_sNumPoints * g_tNumPoints * 2 * sizeof(GLfloat), map, GL_STATIC_DRAW);
    // First 0 is the location = 0. See shader source
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable location = 0
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &g_indicesPassOneVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesPassOneVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, g_sNumPoints * g_tNumPoints * sizeof(GLuint), indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_indicesPassOneVBO);

    // Pass two.

    glGenVertexArrays(1, &g_vaoPassTwo);
    glBindVertexArray(g_vaoPassTwo);

    glGenBuffers(1, &g_verticesPassTwoVBO);
    glBindBuffer(GL_ARRAY_BUFFER, g_verticesPassTwoVBO);
    // Calculate enough space!
    glBufferData(GL_ARRAY_BUFFER, g_sNumPoints * g_tNumPoints * (GLuint) pow(4, DETAIL_LEVEL_FIRST_PASS + 1) * 2 * sizeof(GLfloat), 0, GL_STATIC_DRAW);
    // First 0 is the location = 0. See shader source
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // Enable location = 0
    glEnableVertexAttribArray(0);

    //

    free(map);
    map = 0;

    free(indices);
    indices = 0;

    //

    if (!glusLoadTgaImage(COLOR_MAP, &image))
    {
        printf("Could not load color picture '%s'!\n", COLOR_MAP);

        return GLUS_FALSE;
    }

    glGenTextures(1, &g_colorMapTexture);

    glBindTexture(GL_TEXTURE_2D, g_colorMapTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glusDestroyTgaImage(&image);

    //

    glGenQueries(1, &g_transformFeedbackQuery);

    //
    // Creating the shader program.
    //

    // Pass one.

    glusLoadTextFile("../Example14/shader/PassOne.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example14/shader/PassOne.geom.glsl", &geometrySource);
    glusLoadTextFile("../Example14/shader/PassOne.frag.glsl", &fragmentSource);

    // Compile and ...
    glusCreateProgramFromSource(&g_programPassOne, (const GLUSchar**) &vertexSource.text, 0, 0, (const GLUSchar**) &geometrySource.text, (const GLUSchar**) &fragmentSource.text);

    // ... add the transform variable ...
    glTransformFeedbackVaryings(g_programPassOne.program, 1, (const GLchar**) &TRANSFORM_VARYING, GL_SEPARATE_ATTRIBS);

    // ... and link the program
    if (!glusLinkProgram(&g_programPassOne))
    {
        printf("Could not build program one\n");

        return GLUS_FALSE;
    }

    // Destroy the text resource
    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&geometrySource);
    glusDestroyTextFile(&fragmentSource);

    g_halfDetailStepPassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_halfDetailStep");

    g_detailLevelPassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_detailLevel");

    g_fovRadiusPassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_fovRadius");

    g_positionTextureSpacePassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_positionTextureSpace");
    g_leftNormalTextureSpacePassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_leftNormalTextureSpace");
    g_rightNormalTextureSpacePassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_rightNormalTextureSpace");
    g_backNormalTextureSpacePassOneLocation = glGetUniformLocation(g_programPassOne.program, "u_backNormalTextureSpace");

    // Pass two.

    glusLoadTextFile("../Example14/shader/PassTwo.vert.glsl", &vertexSource);
    glusLoadTextFile("../Example14/shader/PassTwo.cont.glsl", &controlSource);
    glusLoadTextFile("../Example14/shader/PassTwo.eval.glsl", &evaluationSource);
    glusLoadTextFile("../Example14/shader/PassTwo.geom.glsl", &geometrySource);
    glusLoadTextFile("../Example14/shader/PassTwo.frag.glsl", &fragmentSource);

    if (!glusBuildProgramFromSource(&g_shaderProgramPassTwo, (const GLUSchar**) &vertexSource.text, (const GLUSchar**) &controlSource.text, (const GLUSchar**) &evaluationSource.text, (const GLUSchar**) &geometrySource.text, (const GLUSchar**) &fragmentSource.text))
    {
        printf("Could not build program two\n");

        return GLUS_FALSE;
    }

    glusDestroyTextFile(&vertexSource);
    glusDestroyTextFile(&controlSource);
    glusDestroyTextFile(&evaluationSource);
    glusDestroyTextFile(&geometrySource);
    glusDestroyTextFile(&fragmentSource);

    g_maxTessellationLevelPassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_maxTessellationLevel");

    g_quadrantStepPassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_quadrantStep");

    g_positionTextureSpacePassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_positionTextureSpace");

    g_heightMapTexturePassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_heightMapTexture");
    g_normalMapTexturePassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_normalMapTexture");

    g_tmvpPassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_tmvpMatrix");

    g_lightDirectionPassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_lightDirection");

    g_colorMapTexturePassTwoLocation = glGetUniformLocation(g_shaderProgramPassTwo.program, "u_colorMapTexture");

    //

    // One time matrix calculations to convert between texture and world space

    glusMatrixIdentityf(g_textureToWorldMatrix);
    glusMatrixIdentityf(textureToWorldNormalMatrix);

    glusMatrixScalef(g_textureToWorldMatrix, HORIZONTAL_PIXEL_SPACING * METERS_TO_VIRTUAL_WORLD_SCALE, VERTICAL_PIXEL_RANGE * METERS_TO_VIRTUAL_WORLD_SCALE, HORIZONTAL_PIXEL_SPACING * METERS_TO_VIRTUAL_WORLD_SCALE);
    // Skip this scale for the normal matrix

    glusMatrixScalef(g_textureToWorldMatrix, 1.0f, 1.0f, -1.0f);
    glusMatrixScalef(textureToWorldNormalMatrix, 1.0f, 1.0f, -1.0f);

    glusMatrixTranslatef(g_textureToWorldMatrix, -g_sMapExtend / 2.0f, 0.0f, -g_tMapExtend / 2.0f);
    // No need for the translation matrix in the normal matrix

    glusMatrixCopyf(g_worldToTextureMatrix, g_textureToWorldMatrix, GLUS_FALSE);
    glusMatrixInversef(g_worldToTextureMatrix);

    glusMatrixCopyf(g_worldToTextureNormalMatrix, textureToWorldNormalMatrix, GLUS_FALSE);
    glusMatrixInversef(g_worldToTextureNormalMatrix);

    // Pass one

    glUseProgram(g_programPassOne.program);

    glUniform1f(g_halfDetailStepPassOneLocation, detailStep / 2.0f);
    glUniform1ui(g_detailLevelPassOneLocation, DETAIL_LEVEL_FIRST_PASS);
    glUniform1f(g_fovRadiusPassOneLocation, FOV_RADIUS / HORIZONTAL_PIXEL_SPACING * METERS_TO_VIRTUAL_WORLD_SCALE);

    // Pass two

    glUseProgram(g_shaderProgramPassTwo.program);

    glUniform3fv(g_lightDirectionPassTwoLocation, 3, lightDirection);
    glUniform1ui(g_maxTessellationLevelPassTwoLocation, overallMaxDetailLevel - (MINIMUM_DETAIL_LEVEL + DETAIL_LEVEL_FIRST_PASS));
    glUniform1i(g_quadrantStepPassTwoLocation, QUADRANT_STEP);

    //

    glActiveTexture(GL_TEXTURE0);
    glUniform1i(g_heightMapTexturePassTwoLocation, 0);
    glBindTexture(GL_TEXTURE_RECTANGLE, g_heightMapTexture);

    glActiveTexture(GL_TEXTURE1);
    glUniform1i(g_colorMapTexturePassTwoLocation, 1);
    glBindTexture(GL_TEXTURE_2D, g_colorMapTexture);

    glActiveTexture(GL_TEXTURE2);
    glUniform1i(g_normalMapTexturePassTwoLocation, 2);
    glBindTexture(GL_TEXTURE_RECTANGLE, g_normalMapTexture);

    //

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glClearDepth(1.0f);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    return GLUS_TRUE;
}

GLUSvoid reshape(GLUSuint width, GLUSuint height)
{
    g_width = (GLfloat) width;
    g_height = (GLfloat) height;

    glViewport(0, 0, width, height);

    glusPerspectivef(g_projectionMatrix, g_activeView->fov, (GLfloat) width / (GLfloat) height, 1.0f, 1000000.0f);
}

GLUSvoid key(GLUSboolean pressed, GLUSuint key)
{
    static GLboolean filled = GL_TRUE;

    static GLboolean topViewActive = GL_TRUE;

    if (pressed)
    {
        // w for wireframe on / off
        if (key == 'w')
        {
            if (filled)
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else
            {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            filled = !filled;
        }

        // a for animation on / off
        if (key == 'a')
        {
            g_animationOn = !g_animationOn;
        }

        // space for changing the view position
        if (key == ' ')
        {
            topViewActive = !topViewActive;

            if (topViewActive)
            {
                g_activeView = &g_personView;
            }
            else
            {
                g_activeView = &g_topView;
            }

            glusPerspectivef(g_projectionMatrix, g_activeView->fov, g_width / g_height, 1.0f, 1000000.0f);
        }
    }
}

GLUSboolean update(GLUSfloat time)
{
    static GLfloat angle = 0.0f;

    GLuint primitivesWritten;

    // Field of view

    GLfloat rotationMatrix[16];

    GLfloat positionTextureSpace[4];
    GLfloat directionTextureSpace[3];
    GLfloat leftNormalTextureSpace[3];
    GLfloat rightNormalTextureSpace[3];
    GLfloat backNormalTextureSpace[3];

    GLfloat xzPosition2D[4];

    //

    GLfloat tmvpMatrix[16];

    // Animation update

    g_personView.cameraPosition[0] = -cosf(2.0f * PIf * angle / TURN_DURATION) * TURN_RADIUS * METERS_TO_VIRTUAL_WORLD_SCALE;
    g_personView.cameraPosition[2] = -sinf(2.0f * PIf * angle / TURN_DURATION) * TURN_RADIUS * METERS_TO_VIRTUAL_WORLD_SCALE;

    g_personView.cameraDirection[0] = sinf(2.0f * PIf * angle / TURN_DURATION);
    g_personView.cameraDirection[2] = -cosf(2.0f * PIf * angle / TURN_DURATION);

    if (g_animationOn)
    {
        angle += time;
    }

    glusLookAtf(g_viewMatrix, g_activeView->cameraPosition[0], g_activeView->cameraPosition[1], g_activeView->cameraPosition[2], g_activeView->cameraPosition[0] + g_activeView->cameraDirection[0], g_activeView->cameraPosition[1] + g_activeView->cameraDirection[1],
            g_activeView->cameraPosition[2] + g_activeView->cameraDirection[2], g_activeView->cameraUp[0], g_activeView->cameraUp[1], g_activeView->cameraUp[2]);

    glusMatrixIdentityf(tmvpMatrix);
    glusMatrixMultiplyf(tmvpMatrix, tmvpMatrix, g_projectionMatrix);
    glusMatrixMultiplyf(tmvpMatrix, tmvpMatrix, g_viewMatrix);
    glusMatrixMultiplyf(tmvpMatrix, tmvpMatrix, g_textureToWorldMatrix);

    // Position

    xzPosition2D[0] = g_personView.cameraPosition[0];
    xzPosition2D[1] = 0.0f;
    xzPosition2D[2] = g_personView.cameraPosition[2];
    xzPosition2D[3] = g_personView.cameraPosition[3];

    glusMatrixMultiplyPointf(positionTextureSpace, g_worldToTextureMatrix, xzPosition2D);

    // Direction

    glusMatrixMultiplyVectorf(directionTextureSpace, g_worldToTextureMatrix, g_personView.cameraDirection);

    // Left normal of field of view

    glusMatrixIdentityf(rotationMatrix);
    glusMatrixRotateRyf(rotationMatrix, g_personView.fov * (g_width / g_height) / 2.0f + 90.0f);
    glusMatrixMultiplyVectorf(leftNormalTextureSpace, rotationMatrix, g_personView.cameraDirection);
    glusMatrixMultiplyVectorf(leftNormalTextureSpace, g_worldToTextureNormalMatrix, leftNormalTextureSpace);

    // Right normal of field of view

    glusMatrixIdentityf(rotationMatrix);
    glusMatrixRotateRyf(rotationMatrix, -g_personView.fov * (g_width / g_height) / 2.0f - 90.0f);
    glusMatrixMultiplyVectorf(rightNormalTextureSpace, rotationMatrix, g_personView.cameraDirection);
    glusMatrixMultiplyVectorf(rightNormalTextureSpace, g_worldToTextureNormalMatrix, rightNormalTextureSpace);

    // Back normal of field of view

    glusMatrixIdentityf(rotationMatrix);
    glusMatrixRotateRyf(rotationMatrix, 180.0f);
    glusMatrixMultiplyVectorf(backNormalTextureSpace, rotationMatrix, g_personView.cameraDirection);
    glusMatrixMultiplyVectorf(backNormalTextureSpace, g_worldToTextureNormalMatrix, backNormalTextureSpace);

    // OpenGL stuff

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Pass one.

    // Disable any rasterization
    glEnable(GL_RASTERIZER_DISCARD);

    glUseProgram(g_programPassOne.program);

    glUniform4fv(g_positionTextureSpacePassOneLocation, 4, positionTextureSpace);
    glUniform3fv(g_leftNormalTextureSpacePassOneLocation, 3, leftNormalTextureSpace);
    glUniform3fv(g_rightNormalTextureSpacePassOneLocation, 3, rightNormalTextureSpace);
    glUniform3fv(g_backNormalTextureSpacePassOneLocation, 3, backNormalTextureSpace);

    glBindVertexArray(g_vaoPassOne);

    // Bind to vertices used in render pass two. To this buffer is written.
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, g_verticesPassTwoVBO);

    // We need to know, how many primitives are written. So start the query.
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, g_transformFeedbackQuery);

    // Start the operation ...
    glBeginTransformFeedback(GL_POINTS);

    // ... render the elements ...
    glDrawElements(GL_POINTS, g_sNumPoints * g_tNumPoints, GL_UNSIGNED_INT, 0);

    // ... and stop the operation.
    glEndTransformFeedback();

    // Now, we can also stop the query.
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

    glDisable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);

    glBindVertexArray(0);

    // Pass two

    glUseProgram(g_shaderProgramPassTwo.program);

    glUniformMatrix4fv(g_tmvpPassTwoLocation, 1, GL_FALSE, tmvpMatrix);
    glUniform4fv(g_positionTextureSpacePassTwoLocation, 4, positionTextureSpace);

    glBindVertexArray(g_vaoPassTwo);

    // Now get the number of primitives written in the first render pass.
    glGetQueryObjectuiv(g_transformFeedbackQuery, GL_QUERY_RESULT, &primitivesWritten);

    // No draw the final terrain.
    glDrawArrays(GL_PATCHES, 0, primitivesWritten);

    return GLUS_TRUE;
}

GLUSvoid terminate(GLUSvoid)
{
    // Pass one.

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    if (g_verticesPassOneVBO)
    {
        glDeleteBuffers(1, &g_verticesPassOneVBO);

        g_verticesPassOneVBO = 0;
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    if (g_indicesPassOneVBO)
    {
        glDeleteBuffers(1, &g_indicesPassOneVBO);

        g_indicesPassOneVBO = 0;
    }

    glBindVertexArray(0);

    if (g_vaoPassOne)
    {
        glDeleteVertexArrays(1, &g_vaoPassOne);

        g_vaoPassOne = 0;
    }

    glUseProgram(0);

    glusDestroyProgram(&g_programPassOne);

    // Pass two.

    if (g_verticesPassTwoVBO)
    {
        glDeleteBuffers(1, &g_verticesPassTwoVBO);

        g_verticesPassTwoVBO = 0;
    }

    if (g_vaoPassTwo)
    {
        glDeleteVertexArrays(1, &g_vaoPassTwo);

        g_vaoPassTwo = 0;
    }

    glusDestroyProgram(&g_shaderProgramPassTwo);

    //

    if (g_transformFeedbackQuery)
    {
        glDeleteQueries(1, &g_transformFeedbackQuery);

        g_transformFeedbackQuery = 0;
    }

    //

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    if (g_heightMapTexture)
    {
        glDeleteTextures(1, &g_heightMapTexture);

        g_heightMapTexture = 0;
    }

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (g_colorMapTexture)
    {
        glDeleteTextures(1, &g_colorMapTexture);

        g_colorMapTexture = 0;
    }

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_RECTANGLE, 0);

    if (g_normalMapTexture)
    {
        glDeleteTextures(1, &g_normalMapTexture);

        g_normalMapTexture = 0;
    }
}

int main(int argc, char* argv[])
{
    glusInitFunc(init);

    glusReshapeFunc(reshape);

    glusKeyFunc(key);

    glusUpdateFunc(update);

    glusTerminateFunc(terminate);

    glusPrepareContext(4, 2, GLUS_FORWARD_COMPATIBLE_BIT);

    // Create the output window.
    if (!glusCreateWindow("GLUS Example Window", 640, 480, 16, 0, GLUS_FALSE))
    {
        printf("Could not create window!\n");
        return -1;
    }

    // Print out the keys
    printf("Keys:\n");
    printf("[Space] = Toggle view\n");
    printf("a       = Toggle animation on/off\n");
    printf("w       = Toggle wireframe on/off\n");

    // Enter and run the main loop.
    glusRun();

    return 0;
}
