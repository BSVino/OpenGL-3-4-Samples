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

#if defined(__APPLE__)
#define GLFW_GL3
#include "GL/glfw.h"
#else
#include <GL/glew.h>
#endif

#if defined(_WIN32) && !defined(APIENTRY) && !defined(__CYGWIN__) && !defined(__SCITECH_SNAP__)
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
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

#define GLUS_ALPHA 0x1906
#define GLUS_RGB 0x1907
#define GLUS_RGBA 0x1908

#define PIf		3.1415926535897932384626433832795f

    typedef struct _GLUStextfile
    {
            GLUSchar* text;

            GLUSint length;

    } GLUStextfile;

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

            GLUSfloat* texCoords;

            GLUSfloat* allAttributes;

            GLUSuint* indices;

            GLUSuint numberVertices;

            GLUSuint numberIndices;

    } GLUSshape;

#define GLUS_TRUE	1
#define GLUS_FALSE	0

#define GLUS_BACKWARD_COMPATIBLE_BIT	0x0000
#define GLUS_FORWARD_COMPATIBLE_BIT		0x0002

#define GLUS_VERTEX_SHADER 0x8B31
#define GLUS_FRAGMENT_SHADER 0x8B30
#define GLUS_TESS_EVALUATION_SHADER 0x8E87
#define GLUS_TESS_CONTROL_SHADER 0x8E88
#define GLUS_GEOMETRY_SHADER 0x8DD9

    GLUSAPI GLUSvoid GLUSAPIENTRY glusInitFunc(GLUSboolean(*glusNewInit)(GLUSvoid));

    GLUSAPI GLUSvoid GLUSAPIENTRY glusReshapeFunc(GLUSvoid(*glusNewReshape)(const GLUSuint width, const GLUSuint height));

    GLUSAPI GLUSvoid GLUSAPIENTRY glusUpdateFunc(GLUSboolean(*glusNewUpdate)(const GLUSfloat time));

    GLUSAPI GLUSvoid GLUSAPIENTRY glusTerminateFunc(GLUSvoid(*glusNewTerminate)(GLUSvoid));

    GLUSAPI GLUSvoid GLUSAPIENTRY glusPrepareContext(const GLUSuint major, const GLUSuint minor, const GLUSint flags);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateWindow(const GLUSchar* title, const GLUSuint width, const GLUSuint height, const GLUSboolean fullscreen);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusCreateWindowX(const GLUSchar* title, const GLUSuint width, const GLUSuint height, const GLUSuint depthBits, const GLUSuint stencilBits, const GLUSboolean fullscreen);

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
    GLUSAPI GLUSboolean GLUSAPIENTRY glusLoadTgaImage(const GLUSchar* filename, GLUStgaimage* tgaimage);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyTgaImage(GLUStgaimage* tgaimage);

    //
    GLUSAPI GLUSvoid GLUSAPIENTRY glusAddVectorf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusSubtractVectorf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusMultScalarf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat scalar);
    GLUSAPI GLUSfloat GLUSAPIENTRY glusAmountf(const GLUSfloat vector[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusNormalizef(GLUSfloat vector[3]);
    GLUSAPI GLUSfloat GLUSAPIENTRY glusDotf(const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCrossf(GLUSfloat result[3], const GLUSfloat vector0[3], const GLUSfloat vector1[3]);

    //
    GLUSAPI GLUSvoid GLUSAPIENTRY glusSubtractPointf(GLUSfloat result[3], const GLUSfloat point0[4], const GLUSfloat point1[4]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusAddVectorPointf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusSubtractVectorPointf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3]);
    //
    GLUSAPI GLUSfloat GLUSAPIENTRY glusMaxf(const GLUSfloat value0, const GLUSfloat value1);
    GLUSAPI GLUSfloat GLUSAPIENTRY glusMinf(const GLUSfloat value0, const GLUSfloat value1);
    GLUSAPI GLUSfloat GLUSAPIENTRY glusAbsf(const GLUSfloat value);
    GLUSAPI GLUSfloat GLUSAPIENTRY glusLengthf(const GLUSfloat v0, const GLUSfloat v1, const GLUSfloat v2);

    //
    GLUSAPI GLUSvoid GLUSAPIENTRY glusOrthof(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusFrustumf(GLUSfloat result[16], const GLUSfloat left, const GLUSfloat right, const GLUSfloat bottom, const GLUSfloat top, const GLUSfloat nearVal, const GLUSfloat farVal);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusPerspectivef(GLUSfloat result[16], const GLUSfloat fovy, const GLUSfloat aspect, const GLUSfloat zNear, const GLUSfloat zFar);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusLookAtf(GLUSfloat result[16], const GLUSfloat eyeX, const GLUSfloat eyeY, const GLUSfloat eyeZ, const GLUSfloat centerX, const GLUSfloat centerY, const GLUSfloat centerZ, const GLUSfloat upX, const GLUSfloat upY, const GLUSfloat upZ);

    //
    GLUSAPI GLUSvoid GLUSAPIENTRY glusLoadIdentityf(GLUSfloat matrix[16]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCopyMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusMultMatrixf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16]);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusInverseMatrixf(GLUSfloat matrix[16]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusInverseRigidBodyMatrixf(GLUSfloat matrix[16]);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusInverseRotationMatrixf(GLUSfloat matrix[16]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusTransposef(GLUSfloat matrix[16]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusTranslatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusRotatef(GLUSfloat matrix[16], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRyRzRxf(GLfloat matrix[16], const GLfloat anglex, const GLfloat angley, const GLfloat anglez);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRzRyRxf(GLUSfloat matrix[16], const GLUSfloat anglex, const GLUSfloat angley, const GLUSfloat anglez);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRzRxRyf(GLfloat matrix[16], const GLfloat anglex, const GLfloat angley, const GLfloat anglez);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusScalef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusShearf(GLUSfloat matrix[16], const GLUSfloat shxy, const GLUSfloat shxz, const GLUSfloat shyx, const GLUSfloat shyz, const GLUSfloat shzx, const GLUSfloat shzy);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusMultVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusMultPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat point[4]);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusMultPlanef(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat plane[4]);
    //
    GLUSAPI GLUSboolean GLUSAPIENTRY glusBuildProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusCompileProgram(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource);
    GLUSAPI GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram);

    //
    GLUSAPI GLUSvoid GLUSAPIENTRY glusCreatePlanef(GLUSshape* shape, const GLUSfloat halfExtend);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateRectangularPlanef(GLUSshape* shape, const GLUSfloat horizontalExtend, const GLUSfloat verticalExtend);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateCubef(GLUSshape* shape, const GLUSfloat halfExtend);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateSpheref(GLUSshape* shape, const GLUSfloat radius, const GLUSuint numberSlices);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateTorusf(GLUSshape* shape, const GLUSfloat innerRadius, const GLUSfloat outerRadius, const GLUSuint numSides, const GLUSuint numFaces);

    GLUSAPI GLUSvoid GLUSAPIENTRY glusDestroyShapef(GLUSshape* shape);

#ifdef __cplusplus
}
#endif

#endif /*__glus_h_*/
