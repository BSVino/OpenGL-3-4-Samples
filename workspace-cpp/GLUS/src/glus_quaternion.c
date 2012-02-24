#include "GL/glus.h"

#include <math.h>

GLUSvoid GLUSAPIENTRY glusQuatIdentityf(GLUSfloat quaternion[4])
{
    quaternion[0] = 0.0f;
    quaternion[1] = 0.0f;
    quaternion[2] = 0.0f;
    quaternion[3] = 1.0f;
}

GLUSvoid GLUSAPIENTRY glusQuatCopyf(GLUSfloat result[4], const GLUSfloat quaternion[4])
{
    result[0] = quaternion[0];
    result[1] = quaternion[1];
    result[2] = quaternion[2];
    result[3] = quaternion[3];
}

GLUSfloat GLUSAPIENTRY glusQuatNormf(const GLUSfloat quaternion[4])
{
    GLUSfloat x = quaternion[0];
    GLUSfloat y = quaternion[1];
    GLUSfloat z = quaternion[2];
    GLUSfloat w = quaternion[3];

    return sqrtf(x * x + y * y + z * z + w * w);
}

GLUSboolean GLUSAPIENTRY glusQuatNormalizef(GLUSfloat quaternion[4])
{
    GLUSuint i;

    GLUSfloat norm = glusQuatNormf(quaternion);

    GLUSfloat inverseNorm;

    if (norm == 0.0f)
    {
        return GLUS_FALSE;
    }

    inverseNorm = 1.0f / norm;

    for (i = 0; i < 4; i++)
    {
        quaternion[i] *= inverseNorm;
    }

    return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusQuatAddQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4])
{
    GLUSuint i;

    for (i = 0; i < 4; i++)
    {
        result[i] = quaternion0[i] + quaternion1[i];
    }
}

GLUSvoid GLUSAPIENTRY glusQuatSubQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4])
{
    GLUSuint i;

    for (i = 0; i < 4; i++)
    {
        result[i] = quaternion0[i] - quaternion1[i];
    }
}

GLUSvoid GLUSAPIENTRY glusQuatMultiplyQuatf(GLUSfloat result[4], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4])
{
    GLUSfloat temp[4];

    temp[0] = quaternion0[3] * quaternion1[0] + quaternion0[0] * quaternion1[3] + quaternion0[1] * quaternion1[2] - quaternion0[2] * quaternion1[1];
    temp[1] = quaternion0[3] * quaternion1[1] - quaternion0[0] * quaternion1[2] + quaternion0[1] * quaternion1[3] + quaternion0[2] * quaternion1[0];
    temp[2] = quaternion0[3] * quaternion1[2] + quaternion0[0] * quaternion1[1] - quaternion0[1] * quaternion1[0] + quaternion0[2] * quaternion1[3];
    temp[3] = quaternion0[3] * quaternion1[3] - quaternion0[0] * quaternion1[0] - quaternion0[1] * quaternion1[1] - quaternion0[2] * quaternion1[2];

    glusQuatCopyf(result, temp);
}

GLUSvoid GLUSAPIENTRY glusQuatConjugatef(GLUSfloat quaternion[4])
{
    quaternion[0] = -quaternion[0];
    quaternion[1] = -quaternion[1];
    quaternion[2] = -quaternion[2];
    // w remains the same
}

GLUSboolean GLUSAPIENTRY glusQuatInversef(GLUSfloat quaternion[4])
{
    GLUSuint i;

    GLUSfloat norm = glusQuatNormf(quaternion);

    GLUSfloat inverseNorm;

    if (norm == 0.0f)
    {
        return GLUS_FALSE;
    }

    inverseNorm = 1.0f / norm;

    glusQuatConjugatef(quaternion);

    for (i = 0; i < 4; i++)
    {
        quaternion[i] *= inverseNorm * inverseNorm;
    }

    return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusQuatInverseUnitf(GLUSfloat quaternion[4])
{
    glusQuatConjugatef(quaternion);
}

GLUSvoid GLUSAPIENTRY glusQuatRotatef(GLUSfloat quaternion[4], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
    GLUSfloat axis[4];

    GLUSfloat halfAngleRadian = glusDegToRadf(angle) * 0.5;

    GLUSfloat s = sinf(halfAngleRadian);

    GLUSfloat normalized[3] = { x, y, z };

    glusVectorNormalizef(normalized);

    axis[0] = normalized[0] * s;
    axis[1] = normalized[1] * s;
    axis[2] = normalized[2] * s;
    axis[3] = cosf(halfAngleRadian);

    glusQuatMultiplyQuatf(quaternion, quaternion, axis);
}

GLUSvoid GLUSAPIENTRY glusQuatRotateRxf(GLUSfloat quaternion[4], const GLUSfloat angle)
{
    GLUSfloat xAxis[4];

    GLUSfloat halfAngleRadian = glusDegToRadf(angle) * 0.5;

    xAxis[0] = sinf(halfAngleRadian);
    xAxis[1] = 0.0f;
    xAxis[2] = 0.0f;
    xAxis[3] = cosf(halfAngleRadian);

    glusQuatMultiplyQuatf(quaternion, quaternion, xAxis);
}

GLUSvoid GLUSAPIENTRY glusQuatRotateRyf(GLUSfloat quaternion[4], const GLUSfloat angle)
{
    GLUSfloat yAxis[4];

    GLUSfloat halfAngleRadian = glusDegToRadf(angle) * 0.5;

    yAxis[0] = 0.0f;
    yAxis[1] = sinf(halfAngleRadian);
    yAxis[2] = 0.0f;
    yAxis[3] = cosf(halfAngleRadian);

    glusQuatMultiplyQuatf(quaternion, quaternion, yAxis);
}

GLUSvoid GLUSAPIENTRY glusQuatRotateRzf(GLUSfloat quaternion[4], const GLUSfloat angle)
{
    GLUSfloat zAxis[4];

    GLUSfloat halfAngleRadian = glusDegToRadf(angle) * 0.5;

    zAxis[0] = 0.0f;
    zAxis[1] = 0.0f;
    zAxis[2] = sinf(halfAngleRadian);
    zAxis[3] = cosf(halfAngleRadian);

    glusQuatMultiplyQuatf(quaternion, quaternion, zAxis);
}

GLUSvoid GLUSAPIENTRY glusQuatRotateRzRxRyf(GLUSfloat quaternion[4], const GLUSfloat anglez, const GLUSfloat anglex, const GLUSfloat angley)
{
    glusQuatRotateRzf(quaternion, anglez);
    glusQuatRotateRxf(quaternion, anglex);
    glusQuatRotateRyf(quaternion, angley);
}

GLUSvoid GLUSAPIENTRY glusQuatGetMatrixf(GLUSfloat matrix[16], const GLUSfloat quaternion[4])
{
    GLUSfloat x = quaternion[0];
    GLUSfloat y = quaternion[1];
    GLUSfloat z = quaternion[2];
    GLUSfloat w = quaternion[3];

    matrix[0] = 1.0f - 2.0f * y * y - 2.0f * z * z;
    matrix[1] = 2.0f * x * y + 2.0f * w * z;
    matrix[2] = 2.0f * x * z - 2.0f * w * y;
    matrix[3] = 0.0f;

    matrix[4] = 2.0f * x * y - 2.0f * w * z;
    matrix[5] = 1.0f - 2.0f * x * x - 2.0f * z * z;
    matrix[6] = 2.0f * y * z + 2.0f * w * x;
    matrix[7] = 0.0f;

    matrix[8] = 2.0f * x * z + 2.0f * w * y;
    matrix[9] = 2.0f * y * z - 2.0f * w * x;
    matrix[10] = 1.0f - 2.0f * x * x - 2.0f * y * y;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

/**
 * angles[0] = Rz
 * angles[1] = Rx
 * angles[2] = Ry
 */
GLUSvoid GLUSAPIENTRY glusQuatGetEulerf(GLUSfloat angles[3], const GLUSfloat quaternion[4])
{
    GLUSfloat x = quaternion[0];
    GLUSfloat y = quaternion[1];
    GLUSfloat z = quaternion[2];
    GLUSfloat w = quaternion[3];

    GLUSfloat f11 = 1.0f - 2.0f * x * x - 2.0f * z * z;
    GLUSfloat f21 = 2.0f * y * z + 2.0f * w * x;

    if (f11 != 0.0f)
    {
        GLUSfloat f20 = 2.0f * x * z - 2.0f * w * y;
        GLUSfloat f22 = 1.0f - 2.0f * x * x - 2.0f * y * y;
        GLUSfloat f01 = 2.0f * x * y - 2.0f * w * z;

        angles[0] = glusRadToDegf(atan2(-f01, f11));
        angles[1] = glusRadToDegf(asinf(f21));
        angles[2] = glusRadToDegf(atan2(-f20, f22));
    }
    else
    {
        GLUSfloat f10 = 2.0f * x * y + 2.0f * w * z;
        GLUSfloat f00 = 1.0f - 2.0f * y * y - 2.0f * z * z;

        angles[0] = glusRadToDegf(atan2(f10, f00));
        angles[1] = glusRadToDegf(asinf(f21));
        angles[2] = 0.0f;
    }
}

GLUSvoid GLUSAPIENTRY glusQuatSlerpf(GLUSfloat result[16], const GLUSfloat quaternion0[4], const GLUSfloat quaternion1[4], const GLUSfloat t)
{
    GLUSuint i;

    GLUSfloat cosAlpha = quaternion0[0] * quaternion1[0] + quaternion0[1] * quaternion1[1] + quaternion0[2] * quaternion1[2] + quaternion0[3] * quaternion1[3];

    GLUSfloat sinAlpha = sqrtf(1.0f - cosAlpha * cosAlpha);

    GLUSfloat alpha = acosf(cosAlpha);

    GLUSfloat a = sinf(alpha * (1.0f - t)) / sinAlpha;

    GLUSfloat b = sinf(alpha * t) / sinAlpha;

    for (i = 0; i < 4; i++)
    {
        result[i] = a * quaternion0[i] + b * quaternion1[i];
    }
}
