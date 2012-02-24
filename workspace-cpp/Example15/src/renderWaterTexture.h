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

GLUSuint initWaterTexture(GLUSfloat waterPlaneLength);

GLUSvoid reshapeWaterTexture(GLUSuint width, GLUSuint height);

GLUSboolean renderWaterTexture(GLUSfloat time);

GLUSvoid terminateWaterTexture(GLUSvoid);
