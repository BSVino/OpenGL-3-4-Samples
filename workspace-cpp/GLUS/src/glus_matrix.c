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

#include "GL/glus.h"

#include <math.h>

GLUSboolean glusIsRowZerof(const GLUSfloat matrix[16], GLUSuint row)
{
    GLUSuint column;

    for (column = 0; column < 4; column++)
    {
        if (matrix[column * 4 + row] != 0.0f)
        {
            return GLUS_FALSE;
        }
    }

    return GLUS_TRUE;
}

GLUSboolean glusIsColumnZerof(const GLUSfloat matrix[16], GLUSuint column)
{
    GLUSuint row;

    for (row = 0; row < 4; row++)
    {
        if (matrix[column * 4 + row] != 0.0f)
        {
            return GLUS_FALSE;
        }
    }

    return GLUS_TRUE;
}

GLUSvoid glusDevideRowByf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint row, GLUSfloat value)
{
    GLUSuint column;
    GLUSfloat inverseValue = 1.0f / value;

    for (column = 0; column < 4; column++)
    {
        matrix[column * 4 + row] *= inverseValue;
        result[column * 4 + row] *= inverseValue;
    }
}

GLUSvoid glusSwapRowf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint rowOne, GLUSuint rowTwo)
{
    GLUSuint column;

    GLUSfloat temp;

    for (column = 0; column < 4; column++)
    {
        temp = matrix[column * 4 + rowOne];
        matrix[column * 4 + rowOne] = matrix[column * 4 + rowTwo];
        matrix[column * 4 + rowTwo] = temp;

        temp = result[column * 4 + rowOne];
        result[column * 4 + rowOne] = result[column * 4 + rowTwo];
        result[column * 4 + rowTwo] = temp;
    }
}

GLUSvoid glusAddRowf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint rowOne, GLUSuint rowTwo, GLUSfloat factor)
{
    GLUSuint column;

    for (column = 0; column < 4; column++)
    {
        matrix[column * 4 + rowOne] += matrix[column * 4 + rowTwo] * factor;
        result[column * 4 + rowOne] += result[column * 4 + rowTwo] * factor;
    }
}

//

GLUSvoid GLUSAPIENTRY glusMatrixIdentityf(GLUSfloat matrix[16])
{
    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;

    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;

    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}

GLUSvoid GLUSAPIENTRY glusMatrixCopyf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly)
{
    GLUSuint i;

    if (rotationOnly)
    {
        for (i = 0; i < 12; i++)
        {
            if ((i + 1) % 4 == 0)
            {
                continue;
            }

            matrix[i] = source[i];
        }

        matrix[3] = 0.0f;
        matrix[7] = 0.0f;
        matrix[11] = 0.0f;

        matrix[12] = 0.0f;
        matrix[13] = 0.0f;
        matrix[14] = 0.0f;
        matrix[15] = 1.0f;
    }
    else
    {
        for (i = 0; i < 16; i++)
        {
            matrix[i] = source[i];
        }
    }
}

GLUSvoid GLUSAPIENTRY glusMatrixExtract3x3f(GLUSfloat matrix[9], const GLUSfloat source[16])
{
    matrix[0] = source[0];
    matrix[1] = source[1];
    matrix[2] = source[2];

    matrix[3] = source[4];
    matrix[4] = source[5];
    matrix[5] = source[6];

    matrix[6] = source[8];
    matrix[7] = source[9];
    matrix[8] = source[10];
}

GLUSvoid GLUSAPIENTRY glusMatrixExtract2x2f(GLUSfloat matrix[4], const GLUSfloat source[16])
{
    matrix[0] = source[0];
    matrix[1] = source[1];

    matrix[2] = source[4];
    matrix[3] = source[5];
}

GLUSvoid GLUSAPIENTRY glusMatrixMultiplyf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16])
{
    GLUSuint i;
    GLUSuint k;

    GLUSfloat temp[16];

    for (i = 0; i < 16; i++)
    {
        temp[i] = 0.0f;

        for (k = 0; k < 4; k++)
        {
            //			  		row   column   		   row column
            temp[i] += matrix0[(i % 4) + (k * 4)] * matrix1[(k) + ((i / 4) * 4)];
        }
    }

    for (i = 0; i < 16; i++)
    {
        matrix[i] = temp[i];
    }
}

GLUSboolean GLUSAPIENTRY glusMatrixInversef(GLUSfloat matrix[16])
{
    GLUSuint i;

    GLUSint column;
    GLUSint row;

    GLUSfloat copy[16];

    //
    // Copy the original matrix as we want to manipulate it
    //
    for (i = 0; i < 16; i++)
    {
        copy[i] = matrix[i];
    }

    glusMatrixIdentityf(matrix);

    //
    // Make triangle form
    //
    for (column = 0; column < 4; column++)
    {
        GLUSuint row;

        for (row = column; row < 4; row++)
        {
            //
            // Is row all zero, then return false
            //
            if (glusIsRowZerof(copy, row))
            {
                return GLUS_FALSE;
            }

            //
            // Divide, if not zero, by copy[column*4+row]
            //
            if (copy[column * 4 + row] != 0.0f)
            {
                glusDevideRowByf(matrix, copy, row, copy[column * 4 + row]);
            }
        }

        //
        // Is column all zero, then return false
        //
        if (glusIsColumnZerof(copy, column))
        {
            return GLUS_FALSE;
        }

        //
        // Is pivot [column*4+column] = 0.0f
        //
        for (row = column + 1; row < 4; row++)
        {
            if (copy[column * 4 + row] == 1.0f)
            {
                //
                // Swap with pivot row = column
                //
                glusSwapRowf(matrix, copy, column, row);

                break;
            }
        }

        for (row = column + 1; row < 4; row++)
        {
            //
            // Subtract, [column*4+row] not zero, current row minus pivot row = column
            //
            if (copy[column * 4 + row] != 0.0f)
            {
                glusAddRowf(matrix, copy, row, column, -1.0f);
            }
        }
    }

    //
    // Make diagonal form
    //
    for (column = 3; column >= 0; column--)
    {
        for (row = column - 1; row >= 0; row--)
        {
            //
            // Subtract, if [column*4+row] not zero, current row minus pivot row = column with factor [column*4+row]
            //
            if (copy[column * 4 + row] != 0.0f)
            {
                glusAddRowf(matrix, copy, row, column, -copy[column * 4 + row]);
            }
        }
    }

    return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusMatrixInverseRigidBodyf(GLUSfloat matrix[16])
{
    GLUSfloat inverseRotation[16];
    GLUSfloat inverseTranslation[16];

    glusMatrixCopyf(inverseRotation, matrix, GLUS_TRUE);
    glusMatrixTransposef(inverseRotation);

    glusMatrixIdentityf(inverseTranslation);
    inverseTranslation[12] = -matrix[12];
    inverseTranslation[13] = -matrix[13];
    inverseTranslation[14] = -matrix[14];

    glusMatrixMultiplyf(matrix, inverseRotation, inverseTranslation);
}

GLUSboolean GLUSAPIENTRY glusMatrixInverseRotationf(GLUSfloat matrix[16])
{
    GLUSuint i, j;
    GLUSuint index[4];
    GLUSuint counter;
    GLUSuint column;
    GLUSuint row;

    GLUSfloat detRot;
    GLUSfloat temp[16];

    detRot = matrix[0] * matrix[5] * matrix[10] + matrix[4] * matrix[9] * matrix[2] + matrix[8] * matrix[1] * matrix[6] - matrix[8] * matrix[5] * matrix[2] - matrix[4] * matrix[1] * matrix[10] - matrix[0] * matrix[9] * matrix[6];

    if (detRot == 0.0f)
    {
        return GLUS_FALSE;
    }

    glusMatrixCopyf(temp, matrix, GLUS_TRUE);

    matrix[3] = 0.0f;
    matrix[7] = 0.0f;
    matrix[11] = 0.0f;

    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;

    for (i = 0; i < 12; i++)
    {
        if ((i + 1) % 4 == 0)
        {
            continue;
        }

        column = i / 4;
        row = i % 4;

        counter = 0;

        for (j = 0; j < 12; j++)
        {
            // also leave out column depending on row and row depending on column.
            if ((j + 1) % 4 == 0 || j / 4 == row || j % 4 == column)
            {
                continue;
            }

            index[counter] = j;

            if (counter == 3)
            {
                break;
            }

            counter++;
        }

        matrix[i] = 1.0f / detRot * powf(-1.0f, (GLUSfloat) (column + row)) * (temp[index[0]] * temp[index[3]] - temp[index[2]] * temp[index[1]]);
    }

    return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusMatrixTransposef(GLUSfloat matrix[16])
{
    GLUSuint column;
    GLUSuint row;

    GLUSfloat temp[16];

    glusMatrixCopyf(temp, matrix, GLUS_FALSE);

    for (column = 0; column < 4; column++)
    {
        for (row = 0; row < 4; row++)
        {
            matrix[row * 4 + column] = temp[column * 4 + row];
        }
    }
}

GLUSvoid GLUSAPIENTRY glusMatrixTranslatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
    GLUSfloat temp[16];

    glusMatrixIdentityf(temp);

    temp[12] = x;
    temp[13] = y;
    temp[14] = z;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixRotatef(GLUSfloat matrix[16], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
    GLUSfloat temp[16];

    GLUSfloat s = sinf(2.0f * PIf * angle / 360.0f);
    GLUSfloat c = cosf(2.0f * PIf * angle / 360.0f);

    GLUSfloat vector[3] = { x, y, z };

    GLUSfloat xn, yn, zn;

    glusVectorNormalizef(vector);

    xn = vector[0];
    yn = vector[1];
    zn = vector[2];

    glusMatrixIdentityf(temp);

    temp[0] = xn * xn * (1 - c) + c;
    temp[1] = xn * yn * (1 - c) + zn * s;
    temp[2] = xn * zn * (1 - c) - yn * s;

    temp[4] = xn * yn * (1 - c) - zn * s;
    temp[5] = yn * yn * (1 - c) + c;
    temp[6] = yn * zn * (1 - c) + xn * s;

    temp[8] = xn * zn * (1 - c) + yn * s;
    temp[9] = yn * zn * (1 - c) - xn * s;
    temp[10] = zn * zn * (1 - c) + c;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixRotateRxf(GLUSfloat matrix[16], const GLUSfloat angle)
{
    GLUSfloat temp[16];

    GLUSfloat s = sinf(2.0f * PIf * angle / 360.0f);
    GLUSfloat c = cosf(2.0f * PIf * angle / 360.0f);

    glusMatrixIdentityf(temp);

    temp[5] = c;
    temp[6] = s;

    temp[9] = -s;
    temp[10] = c;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixRotateRyf(GLUSfloat matrix[16], const GLUSfloat angle)
{
    GLUSfloat temp[16];

    GLUSfloat s = sinf(2.0f * PIf * angle / 360.0f);
    GLUSfloat c = cosf(2.0f * PIf * angle / 360.0f);

    glusMatrixIdentityf(temp);

    temp[0] = c;
    temp[2] = -s;

    temp[8] = s;
    temp[10] = c;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixRotateRzf(GLUSfloat matrix[16], const GLUSfloat angle)
{
    GLUSfloat temp[16];

    GLUSfloat s = sinf(2.0f * PIf * angle / 360.0f);
    GLUSfloat c = cosf(2.0f * PIf * angle / 360.0f);

    glusMatrixIdentityf(temp);

    temp[0] = c;
    temp[1] = s;

    temp[4] = -s;
    temp[5] = c;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixRotateRzRxRyf(GLUSfloat matrix[16], const GLUSfloat anglez, const GLUSfloat anglex, const GLUSfloat angley)
{
    GLUSfloat temp[16];

    GLUSfloat rz = 2.0f * PIf * anglez / 360.0f;
    GLUSfloat rx = 2.0f * PIf * anglex / 360.0f;
    GLUSfloat ry = 2.0f * PIf * angley / 360.0f;
    GLUSfloat sy = sinf(ry);
    GLUSfloat cy = cosf(ry);
    GLUSfloat sx = sinf(rx);
    GLUSfloat cx = cosf(rx);
    GLUSfloat sz = sinf(rz);
    GLUSfloat cz = cosf(rz);

    glusMatrixIdentityf(temp);

    temp[0] = cy * cz - sx * sy * sz;
    temp[1] = cz * sx * sy + cy * sz;
    temp[2] = -cx * sy;

    temp[4] = -cx * sz;
    temp[5] = cx * cz;
    temp[6] = sx;

    temp[8] = cz * sy + cy * sx * sz;
    temp[9] = -cy * cz * sx + sy * sz;
    temp[10] = cx * cy;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixScalef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
    GLUSfloat temp[16];

    glusMatrixIdentityf(temp);

    temp[0] = x;
    temp[5] = y;
    temp[10] = z;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixShearf(GLUSfloat matrix[16], const GLUSfloat shxy, const GLUSfloat shxz, const GLUSfloat shyx, const GLUSfloat shyz, const GLUSfloat shzx, const GLUSfloat shzy)
{
    GLUSfloat temp[16];

    glusMatrixIdentityf(temp);

    temp[4] = shxy;
    temp[8] = shxz;

    temp[1] = shyx;
    temp[9] = shyz;

    temp[2] = shzx;
    temp[6] = shzy;

    glusMatrixMultiplyf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMatrixMultiplyVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3])
{
    GLUSuint i;

    GLUSfloat temp[3];

    for (i = 0; i < 3; i++)
    {
        temp[i] = matrix[i] * vector[0] + matrix[4 + i] * vector[1] + matrix[8 + i] * vector[2];
    }

    for (i = 0; i < 3; i++)
    {
        result[i] = temp[i];
    }
}

GLUSvoid GLUSAPIENTRY glusMatrixMultiplyPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat point[4])
{
    GLUSuint i;

    GLUSfloat temp[4];

    GLUSfloat inverseW;

    for (i = 0; i < 4; i++)
    {
        temp[i] = matrix[i] * point[0] + matrix[4 + i] * point[1] + matrix[8 + i] * point[2] + matrix[12 + i] * point[3];
    }

    for (i = 0; i < 4; i++)
    {
        result[i] = temp[i];
    }

    if (result[3] != 0.0f && result[3] != 1.0f)
    {
        inverseW = 1.0f / temp[3];

        for (i = 0; i < 4; i++)
        {
            result[i] *= inverseW;
        }
    }
}

GLUSvoid GLUSAPIENTRY glusMatrixMultiplyPlanef(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat plane[4])
{
    GLUSuint i;

    GLUSfloat temp[4];

    for (i = 0; i < 4; i++)
    {
        temp[i] = matrix[i] * plane[0] + matrix[4 + i] * plane[1] + matrix[8 + i] * plane[2] + matrix[12 + i] * plane[3];
    }

    for (i = 0; i < 4; i++)
    {
        result[i] = temp[i];
    }
}

/**
 * angles[0] = Rz
 * angles[1] = Rx
 * angles[2] = Ry
 */
GLUSvoid GLUSAPIENTRY glusMatrixGetEulerf(GLUSfloat angles[3], const GLUSfloat matrix[16])
{
    if (matrix[5] != 0.0f)
    {
        angles[0] = glusRadToDegf(atan2(-matrix[4], matrix[5]));
        angles[1] = glusRadToDegf(asinf(matrix[6]));
        angles[2] = glusRadToDegf(atan2(-matrix[2], matrix[10]));
    }
    else
    {
        angles[0] = glusRadToDegf(atan2f(matrix[1], matrix[0]));
        angles[1] = glusRadToDegf(asinf(matrix[6]));
        angles[2] = 0.0f;
    }
}
