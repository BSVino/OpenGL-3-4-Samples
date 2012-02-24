/**
 * GLUS - OpenGL 3 Utilities. Copyright (C) 2010 Norbert Nopper
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __glus_h_
#define __glus_h_

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef __APPLE__
#define GLEW_STATIC
#include <GL/glew.h>
#else
#define GLFW_NO_GLU
#define GLFW_INCLUDE_GL3
#include "GL/glfw.h"
#endif

#ifndef GLUSAPIENTRY
#define GLUSAPIENTRY
#endif
#ifndef GLUSAPIENTRYP
#define GLUSAPIENTRYP GLUSAPIENTRY *
#endif
#ifndef GLUSAPI
#define GLUSAPI extern
#endif

typedef unsigned int GLUSenum;
typedef unsigned char GLUSboolean;
typedef unsigned int GLUSbitfield;
typedef signed char GLUSbyte;
typedef short GLUSshort;
typedef int GLUSint;
typedef int GLUSsizei;
typedef unsigned char GLUSubyte;
typedef unsigned short GLUSushort;
typedef unsigned int GLUSuint;
typedef float GLUSfloat;
typedef float GLUSclampf;
typedef double GLUSdouble;
typedef double GLUSclampd;
typedef char GLUSchar;

#ifdef __cplusplus
#define GLUSvoid void
#else
typedef void GLUSvoid;
#endif

#define GLUS_OK 0
#define GLUS_TRUE   1
#define GLUS_FALSE  0
#define GLUS_BACKWARD_COMPATIBLE_BIT    0x0000
#define GLUS_FORWARD_COMPATIBLE_BIT     0x0002
#define GLUS_VERTEX_SHADER              0x8B31
#define GLUS_FRAGMENT_SHADER            0x8B30
#define GLUS_TESS_EVALUATION_SHADER     0x8E87
#define GLUS_TESS_CONTROL_SHADER        0x8E88
#define GLUS_GEOMETRY_SHADER            0x8DD9

#define GLUS_ALPHA  0x1906
#define GLUS_RGB    0x1907
#define GLUS_RGBA   0x1908

#define PIf		3.1415926535897932384626433832795f

typedef struct _GLUStextfile
{
    GLUSchar* text;

    GLUSuint length;

} GLUStextfile;

typedef struct _GLUSbinaryfile
{
    unsigned char* binary;

    GLUSuint length;

} GLUSbinaryfile;

typedef struct _GLUStgaimage
{
    GLUSushort width;

    GLUSushort height;

    GLUSubyte* data;

    GLUSenum format;

} GLUStgaimage;

typedef struct _GLUSshaderprogram
{
    GLUSuint program;

    GLUSuint vertex;

    GLUSuint control;

    GLUSint evaluation;

    GLUSuint geometry;

    GLUSuint fragment;

} GLUSshaderprogram;

typedef struct _GLUSshape
{
    GLUSfloat* vertices;

    GLUSfloat* normals;

    GLUSfloat* tangents;

    GLUSfloat* bitangents;

    GLUSfloat* texCoords;

    GLUSfloat* allAttributes;

    GLUSuint* indices;

    GLUSuint numberVertices;

    GLUSuint numberIndices;

} GLUSshape;
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusInitFunc(GLUSboolean(*glusNewInit)(GLUSvoid));
GLUSAPI GLUSvoid GLUSAPIENTRY glusReshapeFunc(GLUSvoid(*glusNewReshape)(const GLUSuint width, const GLUSuint height));
GLUSAPI GLUSvoid GLUSAPIENTRY glusUpdateFunc(GLUSboolean(*glusNewUpdate)(const GLUSfloat time));
GLUSAPI GLUSvoid GLUSAPIENTRY glusTerminateFunc(GLUSvoid(*glusNewTerminate)(GLUSvoid));
GLUSAPI GLUSvoid GLUSAPIENTRY glusPrepareContext(const GLUSuint major, const GLUSuint minor, const GLUSint flags);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPrepareMSAA(const GLUSuint numberSamples);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateWindow(const GLUSchar* title, const GLUSuint width, const GLUSuint height, const GLUSuint depthBits, const GLUSuint stencilBits, const GLUSboolean fullscreen);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyWindow(GLUSvoid);
GLUSAPI GLUSboolean GLUSAPIENTRY glusRun(GLUSvoid);
GLUSAPI GLUSboolean GLUSAPIENTRY glusIsSupported(const GLUSuint major, const GLUSuint minor);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusKeyFunc(GLUSvoid(*glusNewKey)(const GLUSboolean pressed, const GLUSuint key));
GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseFunc(GLUSvoid(*glusNewMouse)(const GLUSboolean pressed, const GLUSuint button, const GLUSuint xPos, const GLUSuint yPos));
GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseWheelFunc(GLUSvoid(*glusNewMouseWheel)(const GLUSuint buttons, const GLUSint ticks, const GLUSuint xPos, const GLUSuint yPos));
GLUSAPI GLUSvoid GLUSAPIENTRY glusMouseMoveFunc(GLUSvoid(*glusNewMouseMove)(const GLUSuint buttons, const GLUSuint xPos, const GLUSuint yPos));
//
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTextFile(const GLUSchar* filename, GLUStextfile* textfile);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTextFile(GLUStextfile* textfile);
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadBinaryFile(const GLUSchar* filename, GLUSbinaryfile* binaryfile);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyBinaryFile(GLUSbinaryfile* binaryfile);
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorCopyf(GLUSfloat result[3], const GLUSfloat vector[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorAddVectorf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorSubtractVectorf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorMultiplyScalarf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat scalar);
GLUSAPI GLUSfloat GLUSAPIENTRY glusVectorLengthf(const GLUSfloat vector[3]);
GLUSAPI GLUSboolean GLUSAPIENTRY glusVectorNormalizef(GLUSfloat vector[3]);
GLUSAPI GLUSfloat GLUSAPIENTRY glusVectorDotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorCrossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusVectorGetPointf(GLUSfloat result[4], const GLUSfloat vector[3]);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusPointCopyf(GLUSfloat result[4], const GLUSfloat point[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPointSubtractPointf(GLUSfloat result[3], const GLUSfloat point0[4], const GLUSfloat point1[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPointAddVectorf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPointSubtractVectorf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPointGetVectorf(GLUSfloat result[3], const GLUSfloat point[4]);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusPlaneCopyf(GLUSfloat result[4], const GLUSfloat plane[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPlaneCreatef(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat normal[3]);
//
GLUSAPI GLUSfloat GLUSAPIENTRY glusMaxf(const GLUSfloat value0, const GLUSfloat value1);
GLUSAPI GLUSfloat GLUSAPIENTRY glusMinf(const GLUSfloat value0, const GLUSfloat value1);
GLUSAPI GLUSfloat GLUSAPIENTRY glusRadToDegf(const GLUSfloat radians);
GLUSAPI GLUSfloat GLUSAPIENTRY glusDegToRadf(const GLUSfloat degrees);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusOrthof(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);
GLUSAPI GLUSvoid GLUSAPIENTRY glusFrustumf(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);
GLUSAPI GLUSvoid GLUSAPIENTRY glusPerspectivef(GLUSfloat result[16], const GLUSfloat fovy, const GLUSfloat aspect, const GLUSfloat zNear, const GLUSfloat zFar);
GLUSAPI GLUSvoid GLUSAPIENTRY glusLookAtf(GLUSfloat result[16], const GLUSfloat eyeX, const GLUSfloat eyeY, const GLUSfloat eyeZ, const GLUSfloat centerX, const GLUSfloat centerY, const GLUSfloat centerZ, const GLUSfloat upX, const GLUSfloat upY, const GLUSfloat upZ);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixIdentityf(GLUSfloat matrix[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixCopyf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixExtract3x3f(GLUSfloat matrix[9], const GLUSfloat source[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixExtract2x2f(GLUSfloat matrix[4], const GLUSfloat source[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixCopyf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixMultiplyf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16]);
GLUSAPI GLUSboolean GLUSAPIENTRY glusMatrixInversef(GLUSfloat matrix[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixInverseRigidBodyf(GLUSfloat matrix[16]);
GLUSAPI GLUSboolean GLUSAPIENTRY glusMatrixInverseRotationf(GLUSfloat matrix[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixTransposef(GLUSfloat matrix[16]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixTranslatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixRotatef(GLUSfloat matrix[16], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixRotateRxf(GLUSfloat matrix[16], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixRotateRyf(GLUSfloat matrix[16], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixRotateRzf(GLUSfloat matrix[16], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixRotateRzRxRyf(GLUSfloat matrix[16], const GLUSfloat anglez, const GLUSfloat anglex, const GLUSfloat angley);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixScalef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixShearf(GLUSfloat matrix[16], const GLUSfloat shxy, const GLUSfloat shxz, const GLUSfloat shyx, const GLUSfloat shyz, const GLUSfloat shzx, const GLUSfloat shzy);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixMultiplyVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixMultiplyPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat point[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixMultiplyPlanef(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat plane[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusMatrixGetEulerf(GLUSfloat angles[3], const GLUSfloat matrix[16]);
//
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatIdentityf(GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatCopyf(GLUSfloat result[4], const GLUSfloat quaternion[4]);
GLUSAPI GLUSfloat GLUSAPIENTRY glusQuatNormf(const GLUSfloat quaternion[4]);
GLUSAPI GLUSboolean GLUSAPIENTRY glusQuatNormalizef(GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatAddQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatSubQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatMultiplyQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatConjugatef(GLUSfloat quaternion[4]);
GLUSAPI GLUSboolean GLUSAPIENTRY glusQuatInversef(GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatInverseUnitf(GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatRotatef(GLUSfloat quaternion[4], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatRotateRxf(GLUSfloat quaternion[4], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatRotateRyf(GLUSfloat quaternion[4], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatRotateRzf(GLUSfloat quaternion[4], const GLUSfloat angle);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatRotateRzRxRyf(GLUSfloat quaternion[4], const GLUSfloat anglez, const GLUSfloat anglex, const GLUSfloat angley);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatGetMatrixf(GLUSfloat matrix[16], const GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatGetEulerf(GLUSfloat angles[3], const GLUSfloat quaternion[4]);
GLUSAPI GLUSvoid GLUSAPIENTRY glusQuatSlerpf(GLUSfloat result[16], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4], const GLUSfloat t);
//
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateProgramFromSource(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);
GLUSAPI GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram);
GLUSAPI GLUSboolean GLUSAPIENTRY glusBuildProgramFromSource(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram);
//
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreatePlanef(GLUSshape* shape, const GLUSfloat halfExtend);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateRectangularPlanef(GLUSshape* shape, const GLUSfloat horizontalExtend, const GLUSfloat verticalExtend);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateSegmentedRectangularPlanef(GLUSshape* shape, const GLUSfloat horizontalExtend, const GLUSfloat verticalExtend, const GLUSuint rows, const GLUSuint columns);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateCubef(GLUSshape* shape, const GLUSfloat halfExtend);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateSpheref(GLUSshape* shape, const GLUSfloat radius, const GLUSuint numberSlices);
GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateTorusf(GLUSshape* shape, const GLUSfloat innerRadius, const GLUSfloat outerRadius, const GLUSuint numSides, const GLUSuint numFaces);
GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyShapef(GLUSshape* shape);
//
GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadObjFile(const GLUSchar* filename, GLUSshape* shape);

#ifdef __cplusplus
}
#endif

#endif /*__glus_h_*/
