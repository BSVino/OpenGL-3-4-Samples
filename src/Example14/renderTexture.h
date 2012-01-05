/**
 * OpenGL 4.1 with GLEW - Example 14
 *
 * @author	Norbert Nopper norbert@nopper.tv
 * @version	1.0
 *
 * Homepage: http://nopper.tv
 *
 * Copyright Norbert Nopper
 */

typedef struct _WaveParameters
{
	GLfloat speed;
	GLfloat amplitude;
	GLfloat wavelength;
	GLfloat steepness;
} WaveParameters;

typedef struct _WaveDirections
{
	GLfloat x;
	GLfloat z;
} WaveDirections;

GLUSuint initTexture(GLUSfloat);

GLUSvoid reshapeTexture(GLUSuint width, GLUSuint height);

GLUSboolean updateTexture(GLUSfloat time);

GLUSvoid terminateTexture(GLUSvoid);
