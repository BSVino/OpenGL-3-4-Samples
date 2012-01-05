/**
 * OpenGL 4 - Example 13
 *
 * Method for Real-Time LOD Terrain Rendering on Modern GPU
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	04.09.2011
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

/**
 * Needed for printf
 */
#include <stdio.h>

/**
 * Needed for powf, logf, ceilf, floorf, min
 */
#include <math.h>

/**
 * Needed for malloc
 */
#include <stdlib.h>

/**
 * Helper library for OS abstraction
 */
#include "GL/glus.h"

#define minimum(a,b) ((a)<(b)?(a):(b))

#define maximum(a,b) ((a)>(b)?(a):(b))

//
// Map metrics etc.
//

/**
 * The space when travelling horizontal form one pixel to the next in meters.
 */
const GLUSfloat horizontalPixelSpacing = 60.0f;

/**
 * One turn takes x seconds.
 */
const GLUSfloat turnDuration = 20.0f;

/**
 * The space when travelling all pixel colors in meters.
 */
const GLUSfloat verticalPixelRange = 10004.0f;

/**
 * The scale to convert the real world meters in virtual worls scale.
 */
const GLUSfloat metersToVirtualWorldScale = 5.0f;

/**
 * The circle around radius.
 */
const GLUSfloat RADIUS = 6000.0f;

/**
 * The normal map of the terrain. Width and height does not have to be the same but they have to be power of two plus one. 
 */
const char* normalMap = "grand_canyon_normal.tga";

/**
 * The height map of the terrain. Width and height does not have to be the same but they have to be power of two plus one. 
 */
const char* heightMap = "grand_canyon_height.tga";

/**
 * The color map of the terrain.
 */
const char* colorMap = "grand_canyon_color.tga";

/**
 * Flag, if animation is on or off
 */
GLUSboolean animationOn = GLUS_TRUE;

/**
 * The extend of the map in s coordinate direction.
 */
GLUSfloat sMapExtend;

/**
 * The extend of the map in t coordinate direction.
 */
GLUSfloat tMapExtend;

/**
 * The maximum detail level which is 2^s = sMapExtend
 */
GLUSuint sMaxDetailLevel;

/**
 * The maximum detail level which is 2^t = tMapExtend
 */
GLUSuint tMaxDetailLevel;

/**
 * The overall maximum detail level from s and t.
 */
GLUSuint overallMaxDetailLevel;

/**
 * Number of points in s direction.
 */
GLUSuint sNumPoints;

/**
 * Number of points in t direction.
 */
GLUSuint tNumPoints;

/**
 * Step for s and t direction.
 */
GLUSfloat detailStep;

/**
 * FOV radius
 */
GLUSfloat fovRadius = 10000.0f;

//
// Buffer objects etc.
//

/**
 * The VAO for the vertices etc.
 */
GLuint vertexArrayObjectPassOne = 0;

/**
 * The VBO for the vertices.
 */
GLuint verticesBufferPassOne = 0;

/**
 * The VBO for the indices.
 */
GLuint indicesBufferPassOne = 0;

/**
 * The VAO for the vertices etc.
 */
GLuint vertexArrayObjectPassTwo = 0;

/**
 * The VBO for the vertices.
 */
GLuint verticesBufferPassTwo = 0;

/**
 * The VBO for the indices.
 */
GLuint indicesBufferPassTwo = 0;

/**
 * The used shader program.
 */
GLUSshaderprogram shaderProgramPassOne;

/**
 * The used shader program.
 */
GLUSshaderprogram shaderProgramPassTwo;

/**
 * Projection matrix.
 */
GLfloat projection[16];

/**
 * Location of the projection matrix in the shader program.
 */
GLint tmvpLocationPassTwo;

/**
 * Model view matrix.
 */
GLfloat modelView[16];

GLfloat lightDir[3];

/**
 * Texture to world space matrix.
 */
GLfloat textureToWorld[16];

GLfloat textureToWorldNormal[16];

/**
 * World to texture space matrix.
 */
GLfloat worldToTexture[16];

GLfloat worldToTextureNormal[16];

GLint halfDetailStepLocationPassOne;

GLint firstPassDetailLevelLocationPassOne;

GLint positionTextureLocationPassOne;

GLint fovRadiusLocationPassOne;

GLint leftNormalTextureLocationPassOne;

GLint rightNormalTextureLocationPassOne;

GLint backNormalTextureLocationPassOne;

//

GLint maxTessellationLevelLocationPassTwo;

GLint quadrantStepLocationPassTwo;

GLint positionTextureLocationPassTwo;

GLint leftNormalTextureLocationPassTwo;

GLint rightNormalTextureLocationPassTwo;

GLint backNormalTextureLocationPassTwo;

GLint heightMapTextureLocationPassTwo;

GLint colorMapTextureLocationPassTwo;

GLint normalMapTextureLocationPassTwo;

GLint lightDirLocationPassTwo;

/**
 * Varable to query how much primitives were written.
 */
GLuint transformFeedbackQuery;

GLuint normalMapTexture;

GLuint heightMapTexture;

GLuint colorMapTexture;

typedef struct _ViewData
{
	GLUSfloat cameraPosition[4];
	GLUSfloat cameraDirection[3];
	GLUSfloat cameraUp[3];
	GLUSfloat fov;
} ViewData;

ViewData topView;

ViewData personView;

ViewData* activeView;

GLUSfloat g_width;

GLUSfloat g_height;

//
// Rendering parameters
//

/**
 * Detail level at the beginning of the map.
 */
const GLUSuint minimumDetailLevel = 4;

/**
 * Additional detail level in the first pass. Adjust in GeometryPassOne max_vertices = 4^(firstPassDetailLevel+1)
 */
const GLUSuint firstPassDetailLevel = 2;

/**
 * Number of quadrants when going to the next detail level.
 */
const GLUSint quadrantStep = 2;

/**
 * Function for initialization.
 *
 * @return true, if initialization successful. False otherwise.
 */
GLUSboolean init(GLUSvoid)
{
	GLUSuint s, t;

	GLUStgaimage image;

	GLUSfloat* map = 0;

	GLUSuint* indices = 0;

	GLUStextfile vertexSource;

	GLUStextfile controlSource;

	GLUStextfile evaluationSource;

	GLUStextfile geometrySource;

	GLUStextfile fragmentSource;

	// gl_Position will be transformed to the buffer 
	GLUSchar* transformVaryings = "vertexTransform";

	topView.cameraPosition[0] = 0.0f;
	topView.cameraPosition[1] = 30000.0f * metersToVirtualWorldScale;
	topView.cameraPosition[2] = 0.0f;
	topView.cameraPosition[3] = 1.0;
	topView.cameraDirection[0] = 0.0f;
	topView.cameraDirection[1] = -1.0f;
	topView.cameraDirection[2] = 0.0f;
	topView.cameraUp[0] = 0.0f;
	topView.cameraUp[1] = 0.0f;
	topView.cameraUp[2] = -1.0f;
	topView.fov = 40.0f;

	personView.cameraPosition[0] = 0.0f;
	personView.cameraPosition[1] = 4700.0f * metersToVirtualWorldScale;
	personView.cameraPosition[2] = 0.0f;
	personView.cameraPosition[3] = 1.0;
	personView.cameraDirection[0] = 0.0f;
	personView.cameraDirection[1] = 0.0f;
	personView.cameraDirection[2] = -1.0f;
	personView.cameraUp[0] = 0.0f;
	personView.cameraUp[1] = 1.0f;
	personView.cameraUp[2] = 0.0f;
	personView.fov = 60.0f;

	lightDir[0] = 1.0f;
	lightDir[1] = 1.0f;
	lightDir[2] = 1.0f;

	activeView = &personView;

	if (!glusLoadTgaImage(normalMap, &image))
	{
		printf("Could not load normal picture '%s'!\n", normalMap);

		return GLUS_FALSE;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &normalMapTexture);

	glBindTexture(GL_TEXTURE_RECTANGLE, normalMapTexture);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glusDestroyTgaImage(&image);

	if (!glusLoadTgaImage(heightMap, &image))
	{
		printf("Could not load height picture '%s'!\n", heightMap);

		return GLUS_FALSE;
	}

	glGenTextures(1, &heightMapTexture);

	glBindTexture(GL_TEXTURE_RECTANGLE, heightMapTexture);

	glTexImage2D(GL_TEXTURE_RECTANGLE, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	sMapExtend = (GLUSfloat) image.width;

	tMapExtend = (GLUSfloat) image.height;

	glusDestroyTgaImage(&image);

	// Calculate the detail level for the s and ...
	sMaxDetailLevel = (GLUSuint) floorf(logf(sMapExtend) / logf(2.0f));

	// ... t extend
	tMaxDetailLevel = (GLUSuint) floorf(logf(tMapExtend) / logf(2.0f));

	overallMaxDetailLevel = minimum(sMaxDetailLevel, tMaxDetailLevel);

	// Do checking of calculated parameters
	if (minimumDetailLevel > overallMaxDetailLevel)
	{
		printf("Detail level to high %d > %d\n", minimumDetailLevel, overallMaxDetailLevel);

		return GLUS_FALSE;
	}

	if (minimumDetailLevel + firstPassDetailLevel > overallMaxDetailLevel)
	{
		printf("First pass detail level to high %d > %d\n", minimumDetailLevel + firstPassDetailLevel, overallMaxDetailLevel);

		return GLUS_FALSE;
	}

	if (powf(2.0f, overallMaxDetailLevel - (minimumDetailLevel + firstPassDetailLevel)) > 32.0f)
	{
		printf("Tessellation level to high %d > 32\n", (GLUSint) powf(2.0f, overallMaxDetailLevel - (minimumDetailLevel + firstPassDetailLevel)));

		return GLUS_FALSE;
	}

	detailStep = powf(2.0f, overallMaxDetailLevel - minimumDetailLevel);

	sNumPoints = (GLUSuint) ceilf(sMapExtend / detailStep) - 1;

	tNumPoints = (GLUSuint) ceilf(tMapExtend / detailStep) - 1;

	map = (GLUSfloat*) malloc(sNumPoints * tNumPoints * 2 * sizeof(GLUSfloat));

	indices = (GLUSuint*) malloc(sNumPoints * tNumPoints * sizeof(GLUSuint));

	for (t = 0; t < tNumPoints; t++)
	{
		for (s = 0; s < sNumPoints; s++)
		{
			map[t * sNumPoints * 2 + s * 2 + 0] = 0.5f + detailStep / 2.0f + (GLUSfloat) s * detailStep;
			map[t * sNumPoints * 2 + s * 2 + 1] = 0.5f + detailStep / 2.0f + (GLUSfloat) t * detailStep;

			indices[t * sNumPoints + s + 0] = (t + 0) * sNumPoints + s + 0;
		}
	}

	//
	// Transfering vertices and indices into GPU
	//

	// Pass one

	// Generating vertex array object ...
	glGenVertexArrays(1, &vertexArrayObjectPassOne);

	// ... and binding to it.
	glBindVertexArray(vertexArrayObjectPassOne);

	// Generating the vertices buffer ...
	glGenBuffers(1, &verticesBufferPassOne);

	// ... and binding to it.
	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferPassOne);

	// Transfering the vertices.
	glBufferData(GL_ARRAY_BUFFER, sNumPoints * tNumPoints * 2 * sizeof(GLfloat), map, GL_STATIC_DRAW);

	// Generating the indices buffer ...
	glGenBuffers(1, &indicesBufferPassOne);

	// ... and binding to it.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferPassOne);

	// Transfering the indices.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sNumPoints * tNumPoints * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// First 0 is the location = 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable location = 0
	glEnableVertexAttribArray(0);

	// Pass two.

	// Generating vertex array object ...
	glGenVertexArrays(1, &vertexArrayObjectPassTwo);

	// ... and binding to it.
	glBindVertexArray(vertexArrayObjectPassTwo);

	// Generating the vertices buffer ...
	glGenBuffers(1, &verticesBufferPassTwo);

	// ... and binding to it.
	glBindBuffer(GL_ARRAY_BUFFER, verticesBufferPassTwo);

	// Reserving space for the incoming vertices.
	glBufferData(GL_ARRAY_BUFFER, sNumPoints * tNumPoints * (GLUSuint) pow(4, firstPassDetailLevel + 1) * 2 * sizeof(GLfloat), 0, GL_STATIC_DRAW);

	// First 0 is the location = 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable location = 0
	glEnableVertexAttribArray(0);

	//

	free(map);
	map = 0;

	free(indices);
	indices = 0;

	if (!glusLoadTgaImage(colorMap, &image))
	{
		printf("Could not load color picture '%s'!\n", colorMap);

		return GLUS_FALSE;
	}

	glGenTextures(1, &colorMapTexture);

	glBindTexture(GL_TEXTURE_2D, colorMapTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, image.format, image.width, image.height, 0, image.format, GL_UNSIGNED_BYTE, image.data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glusDestroyTgaImage(&image);

	//
	// Creating the shader program.
	//

	// Pass one.

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example13/VertexPassOne.vs", &vertexSource);

	// Load the source of the geometry shader.
	glusLoadTextFile("../src/Example13/GeometryPassOne.gs", &geometrySource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example13/FragmentPassOne.fs", &fragmentSource);

	// Compile and ...
	glusCompileProgram(&shaderProgramPassOne, (const GLUSchar**) &vertexSource.text, 0, 0, (const GLUSchar**) &geometrySource.text, (const GLUSchar**) &fragmentSource.text);

	// ... add the transform variable ...
	glTransformFeedbackVaryings(shaderProgramPassOne.program, 1, (const GLUSchar**) &transformVaryings, GL_SEPARATE_ATTRIBS);

	// ... and link the program
	if (!glusLinkProgram(&shaderProgramPassOne))
	{
		printf("Could not build program one\n");

		return GLUS_FALSE;
	}

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&geometrySource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	halfDetailStepLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "halfDetailStep");

	firstPassDetailLevelLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "firstPassDetailLevel");

	positionTextureLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "positionTexture");

	fovRadiusLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "fovRadius");

	leftNormalTextureLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "leftNormalTexture");

	rightNormalTextureLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "rightNormalTexture");

	backNormalTextureLocationPassOne = glGetUniformLocation(shaderProgramPassOne.program, "backNormalTexture");

	// Pass two.

	// Load the source of the vertex shader.
	glusLoadTextFile("../src/Example13/VertexPassTwo.vs", &vertexSource);

	// Load the source of the control shader.
	glusLoadTextFile("../src/Example13/ControlPassTwo.tcs", &controlSource);

	// Load the source of the evaluation shader.
	glusLoadTextFile("../src/Example13/EvaluationPassTwo.tes", &evaluationSource);

	// Load the source of the geometry shader.
	glusLoadTextFile("../src/Example13/GeometryPassTwo.gs", &geometrySource);

	// Load the source of the fragment shader.
	glusLoadTextFile("../src/Example13/FragmentPassTwo.fs", &fragmentSource);

	// Build and ...
	if (!glusBuildProgram(&shaderProgramPassTwo, (const GLUSchar**) &vertexSource.text, (const GLUSchar**) &controlSource.text, (const GLUSchar**) &evaluationSource.text, (const GLUSchar**) &geometrySource.text, (const GLUSchar**) &fragmentSource.text))
	{
		printf("Could not build program two\n");

		return GLUS_FALSE;
	}

	// Destroy the text resource
	glusDestroyTextFile(&vertexSource);

	// Destroy the text resource
	glusDestroyTextFile(&controlSource);

	// Destroy the text resource
	glusDestroyTextFile(&evaluationSource);

	// Destroy the text resource
	glusDestroyTextFile(&geometrySource);

	// Destroy the text resource
	glusDestroyTextFile(&fragmentSource);

	// Get the location of the matrix.
	tmvpLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "TMVP");

	positionTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "positionTexture");

	maxTessellationLevelLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "maxTessellationLevel");

	quadrantStepLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "quadrantStep");

	leftNormalTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "leftNormalTexture");

	rightNormalTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "rightNormalTexture");

	backNormalTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "backNormalTexture");

	normalMapTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "normalMapTexture");

	heightMapTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "heightMapTexture");

	colorMapTextureLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "colorMapTexture");

	lightDirLocationPassTwo = glGetUniformLocation(shaderProgramPassTwo.program, "lightDir");

	//
	// One time GL settings.
	//

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glGenQueries(1, &transformFeedbackQuery);

	// Matrix calculations

	glusLoadIdentityf(textureToWorld);

	glusLoadIdentityf(textureToWorldNormal);

	glusScalef(textureToWorld, horizontalPixelSpacing * metersToVirtualWorldScale, verticalPixelRange * metersToVirtualWorldScale, horizontalPixelSpacing
			* metersToVirtualWorldScale);

	glusScalef(textureToWorld, 1.0f, 1.0f, -1.0f);

	glusScalef(textureToWorldNormal, 1.0f, 1.0f, -1.0f);

	glusTranslatef(textureToWorld, -sMapExtend / 2.0f, 0.0f, -tMapExtend / 2.0f);

	glusCopyMatrixf(worldToTexture, textureToWorld, GLUS_FALSE);
	glusInverseMatrixf(worldToTexture);

	glusCopyMatrixf(worldToTextureNormal, textureToWorldNormal, GLUS_FALSE);
	glusInverseMatrixf(worldToTextureNormal);

	// Pass One

	glUseProgram(shaderProgramPassOne.program);

	glUniform1f(halfDetailStepLocationPassOne, detailStep / 2.0f);

	glUniform1ui(firstPassDetailLevelLocationPassOne, firstPassDetailLevel);

	glUniform1f(fovRadiusLocationPassOne, fovRadius / horizontalPixelSpacing*metersToVirtualWorldScale);

	// Pass Two

	glUseProgram(shaderProgramPassTwo.program);

	glUniform1ui(maxTessellationLevelLocationPassTwo, overallMaxDetailLevel - (minimumDetailLevel + firstPassDetailLevel));

	glUniform1i(quadrantStepLocationPassTwo, quadrantStep);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(heightMapTextureLocationPassTwo, 0);
	glBindTexture(GL_TEXTURE_RECTANGLE, heightMapTexture);

	glActiveTexture(GL_TEXTURE1);
	glUniform1i(colorMapTextureLocationPassTwo, 1);
	glBindTexture(GL_TEXTURE_2D, colorMapTexture);

	glActiveTexture(GL_TEXTURE2);
	glUniform1i(normalMapTextureLocationPassTwo, 2);
	glBindTexture(GL_TEXTURE_RECTANGLE, normalMapTexture);

	glActiveTexture(GL_TEXTURE0);

	glUniform3fv(lightDirLocationPassTwo, 3, lightDir);

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
	GLUSfloat TMVP[16];

	glViewport(0, 0, width, height);

	glusLookAtf(modelView, activeView->cameraPosition[0], activeView->cameraPosition[1], activeView->cameraPosition[2], activeView->cameraPosition[0]
			+ activeView->cameraDirection[0], activeView->cameraPosition[1] + activeView->cameraDirection[1], activeView->cameraPosition[2]
			+ activeView->cameraDirection[2], activeView->cameraUp[0], activeView->cameraUp[1], activeView->cameraUp[2]);

	glusPerspectivef(projection, activeView->fov, (GLfloat) width / (GLfloat) height, 1.0f, 1000000.0f);

	glUseProgram(shaderProgramPassTwo.program);

	glusLoadIdentityf(TMVP);
	glusMultMatrixf(TMVP, TMVP, projection);
	glusMultMatrixf(TMVP, TMVP, modelView);
	glusMultMatrixf(TMVP, TMVP, textureToWorld);

	glUniformMatrix4fv(tmvpLocationPassTwo, 1, GL_FALSE, TMVP);

	g_width = (GLUSfloat) width;
	g_height = (GLUSfloat) height;
}

GLUSvoid key(GLUSboolean pressed, GLUSuint key)
{
	static GLUSboolean filled = GLUS_TRUE;

	static GLUSboolean topViewActive = GLUS_TRUE;

	if (pressed)
	{
		// w
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

		// a
		if (key == 'a')
		{
			animationOn = !animationOn;
		}

		// space
		if (key == ' ')
		{
			topViewActive = !topViewActive;

			if (topViewActive)
			{
				activeView = &personView;
			}
			else
			{
				activeView = &topView;
			}

			glusPerspectivef(projection, activeView->fov, g_width / g_height, 1.0f, 1000000.0f);
		}
	}
}

/**
 * Function to render and display content.
 *
 * @return true, to continue rendering. False to quit the application.
 */
GLUSboolean update(GLUSfloat time)
{
	static GLUSfloat angle = 0.0f;

	GLUSuint primitivesWritten;

	// Field of view

	GLUSfloat rotationMatrix[16];

	GLUSfloat leftNormal[3];

	GLUSfloat rightNormal[3];

	GLUSfloat backNormal[3];

	GLUSfloat positionTexture[4];

	GLUSfloat directionTexture[3];

	GLUSfloat leftNormalTexture[3];

	GLUSfloat rightNormalTexture[3];

	GLUSfloat backNormalTexture[3];

	GLUSfloat flatPosition[4];

	// 

	GLUSfloat TMVP[16];

	// Animation update

	personView.cameraPosition[0] = -cosf(2.0f*PIf*angle/turnDuration) * RADIUS * metersToVirtualWorldScale;
	personView.cameraPosition[2] = -sinf(2.0f*PIf*angle/turnDuration) * RADIUS * metersToVirtualWorldScale;

	personView.cameraDirection[0] = sinf(2.0f*PIf*angle/turnDuration);
	personView.cameraDirection[2] = -cosf(2.0f*PIf*angle/turnDuration);

	if (animationOn)
	{
		angle += time;
	}

	glusLookAtf(modelView, activeView->cameraPosition[0], activeView->cameraPosition[1], activeView->cameraPosition[2], activeView->cameraPosition[0]
			+ activeView->cameraDirection[0], activeView->cameraPosition[1] + activeView->cameraDirection[1], activeView->cameraPosition[2]
			+ activeView->cameraDirection[2], activeView->cameraUp[0], activeView->cameraUp[1], activeView->cameraUp[2]);

	glUseProgram(shaderProgramPassTwo.program);

	glusLoadIdentityf(TMVP);
	glusMultMatrixf(TMVP, TMVP, projection);
	glusMultMatrixf(TMVP, TMVP, modelView);
	glusMultMatrixf(TMVP, TMVP, textureToWorld);

	glUniformMatrix4fv(tmvpLocationPassTwo, 1, GL_FALSE, TMVP);

	// Position

	//
	//

	flatPosition[0] = personView.cameraPosition[0];
	flatPosition[1] = 0.0f;
	flatPosition[2] = personView.cameraPosition[2];
	flatPosition[3] = personView.cameraPosition[3];

	glusMultPointf(positionTexture, worldToTexture, flatPosition);

	// Direction

	glusMultVectorf(directionTexture, worldToTexture, personView.cameraDirection);

	// Left normal of field ov view

	glusLoadIdentityf(rotationMatrix);

	glusRotatef(rotationMatrix, personView.fov * (g_width / g_height) / 2.0f + 90.0f, 0.0f, 1.0f, 0.0f);

	glusMultVectorf(leftNormal, rotationMatrix, personView.cameraDirection);

	glusMultVectorf(leftNormalTexture, worldToTextureNormal, leftNormal);

	// Right normal of field ov view

	glusLoadIdentityf(rotationMatrix);

	glusRotatef(rotationMatrix, -personView.fov * (g_width / g_height) / 2.0f - 90.0f, 0.0f, 1.0f, 0.0f);

	glusMultVectorf(rightNormal, rotationMatrix, personView.cameraDirection);

	glusMultVectorf(rightNormalTexture, worldToTextureNormal, rightNormal);

	// Back normal of field ov views

	glusLoadIdentityf(rotationMatrix);

	glusRotatef(rotationMatrix, 180.0f, 0.0f, 1.0f, 0.0f);

	glusMultVectorf(backNormal, rotationMatrix, personView.cameraDirection);

	glusMultVectorf(backNormalTexture, worldToTextureNormal, backNormal);

	// OpenGL stuff

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Pass one.

	glEnable(GL_RASTERIZER_DISCARD);

	glUseProgram(shaderProgramPassOne.program);

	glUniform4fv(positionTextureLocationPassOne, 4, positionTexture);

	glUniform3fv(leftNormalTextureLocationPassOne, 3, leftNormalTexture);

	glUniform3fv(rightNormalTextureLocationPassOne, 3, rightNormalTexture);

	glUniform3fv(backNormalTextureLocationPassOne, 3, backNormalTexture);

	glBindVertexArray(vertexArrayObjectPassOne);

	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, verticesBufferPassTwo);

	glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, transformFeedbackQuery);

	glBeginTransformFeedback(GL_POINTS);

	glDrawElements(GL_POINTS, sNumPoints * tNumPoints, GL_UNSIGNED_INT, 0);

	glEndTransformFeedback();

	glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

	glDisable(GL_RASTERIZER_DISCARD);

	// Pass two

	glUseProgram(shaderProgramPassTwo.program);

	glUniform4fv(positionTextureLocationPassTwo, 4, positionTexture);

	glUniform3fv(leftNormalTextureLocationPassTwo, 3, leftNormalTexture);

	glUniform3fv(rightNormalTextureLocationPassTwo, 3, rightNormalTexture);

	glUniform3fv(backNormalTextureLocationPassTwo, 3, backNormalTexture);

	glBindVertexArray(vertexArrayObjectPassTwo);

	glGetQueryObjectuiv(transformFeedbackQuery, GL_QUERY_RESULT, &primitivesWritten);

	glDrawArrays(GL_PATCHES, 0, primitivesWritten);

	return GLUS_TRUE;
}

/**
 * Function to clean up everything.
 */
GLUSvoid terminate(GLUSvoid)
{
	// Pass one.

	if (verticesBufferPassOne)
	{
		glDeleteBuffers(1, &verticesBufferPassOne);

		verticesBufferPassOne = 0;
	}

	if (indicesBufferPassOne)
	{
		glDeleteBuffers(1, &indicesBufferPassOne);

		indicesBufferPassOne = 0;
	}

	if (vertexArrayObjectPassOne)
	{
		glDeleteVertexArrays(1, &vertexArrayObjectPassOne);

		vertexArrayObjectPassOne = 0;
	}

	glusDestroyProgram(&shaderProgramPassOne);

	// Pass two.

	if (verticesBufferPassTwo)
	{
		glDeleteBuffers(1, &verticesBufferPassTwo);

		verticesBufferPassTwo = 0;
	}

	if (vertexArrayObjectPassTwo)
	{
		glDeleteVertexArrays(1, &vertexArrayObjectPassTwo);

		vertexArrayObjectPassTwo = 0;
	}

	if (normalMapTexture)
	{
		glDeleteTextures(1, &normalMapTexture);

		normalMapTexture = 0;
	}

	if (heightMapTexture)
	{
		glDeleteTextures(1, &heightMapTexture);

		heightMapTexture = 0;
	}

	if (colorMapTexture)
	{
		glDeleteTextures(1, &colorMapTexture);

		colorMapTexture = 0;
	}

	glusDestroyProgram(&shaderProgramPassTwo);

	glDeleteQueries(1, &transformFeedbackQuery);

	transformFeedbackQuery = 0;
}

/**
 * Main entry point.
 */
int main(int argc, char* argv[])
{
	glusInitFunc(init);

	glusReshapeFunc(reshape);

	glusKeyFunc(key);

	glusUpdateFunc(update);

	glusTerminateFunc(terminate);

	glusPrepareContext(4, 1, GLUS_FORWARD_COMPATIBLE_BIT);

	// Create the output window.
	if (!glusCreateWindow("GLUS Example Window", 640, 480, GLUS_FALSE))
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
