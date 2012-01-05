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

//

GLUSboolean glusIsRowZerof(const GLUSfloat matrix[16], GLUSuint row)
{
	GLUSuint column;

	for (column = 0; column < 4; column++)
	{
		if (matrix[column * 4 + row] != 0.0f)
			return GLUS_FALSE;
	}

	return GLUS_TRUE;
}

GLUSboolean glusIsColumnZerof(const GLUSfloat matrix[16], GLUSuint column)
{
	GLUSuint row;

	for (row = 0; row < 4; row++)
	{
		if (matrix[column * 4 + row] != 0.0f)
			return GLUS_FALSE;
	}

	return GLUS_TRUE;
}

GLUSvoid glusDevideRowByf(GLUSfloat result[16], GLUSfloat matrix[16], GLUSuint row, GLUSfloat value)
{
	GLUSuint column;

	for (column = 0; column < 4; column++)
	{
		matrix[column * 4 + row] /= value;
		result[column * 4 + row] /= value;
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

GLUSvoid GLUSAPIENTRY glusLoadIdentityf(GLUSfloat matrix[16])
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

GLUSvoid GLUSAPIENTRY glusCopyMatrixf(GLUSfloat matrix[16], const GLUSfloat source[16], const GLUSboolean rotationOnly)
{
	GLUSuint i;

	for (i = 0; i < 16; i++)
	{
		matrix[i] = source[i];
	}

	if (rotationOnly)
	{
		matrix[3] = 0.0f;
		matrix[7] = 0.0f;
		matrix[11] = 0.0f;

		matrix[12] = 0.0f;
		matrix[13] = 0.0f;
		matrix[14] = 0.0f;
		matrix[15] = 1.0f;
	}
}

GLUSvoid GLUSAPIENTRY glusMultMatrixf(GLUSfloat matrix[16], const GLUSfloat matrix0[16], const GLUSfloat matrix1[16])
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

GLUSboolean GLUSAPIENTRY glusInverseMatrixf(GLUSfloat matrix[16])
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

	glusLoadIdentityf(matrix);

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
				return GLUS_FALSE;

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
			return GLUS_FALSE;

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

GLUSvoid GLUSAPIENTRY glusInverseRigidBodyMatrixf(GLUSfloat matrix[16])
{
    GLUSfloat inverseRotation[16];
    GLUSfloat inverseTranslation[16];

    glusCopyMatrixf(inverseRotation, matrix, GLUS_TRUE);
    glusTransposef(inverseRotation);

    glusLoadIdentityf(inverseTranslation);
    inverseTranslation[12] = -matrix[12];
    inverseTranslation[13] = -matrix[13];
    inverseTranslation[14] = -matrix[14];

    glusMultMatrixf(matrix, inverseRotation, inverseTranslation);
}

GLUSboolean GLUSAPIENTRY glusInverseRotationMatrixf(GLUSfloat matrix[16])
{
    GLUSuint i, j;
    GLUSuint index[4];
    GLUSuint counter;
    GLUSuint column;
    GLUSuint row;

    GLUSfloat detRot;
    GLUSfloat subDet;
    GLUSfloat temp[16];

    detRot = matrix[0] * matrix[5] * matrix[10] + matrix[4] * matrix[9] * matrix[2] + matrix[8] * matrix[1] * matrix[6] - matrix[8] * matrix[5] * matrix[2] - matrix[4] * matrix[1] * matrix[10] - matrix[0] * matrix[9] * matrix[6];

    if (detRot == 0.0f)
    {
        return GLUS_FALSE;
    }

    glusCopyMatrixf(temp, matrix, GLUS_TRUE);

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

        subDet = 0.0f;
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

        matrix[i] = 1.0f / detRot * powf(-1.0f, (GLfloat) (column + row)) * (temp[index[0]] * temp[index[3]] - temp[index[2]] * temp[index[1]]);
    }

    return GLUS_TRUE;
}

GLUSvoid GLUSAPIENTRY glusTransposef(GLUSfloat matrix[16])
{
	GLUSuint column;
	GLUSuint row;

	GLUSfloat temp[16];

	glusCopyMatrixf(temp, matrix, GLUS_FALSE);

	for (column = 0; column < 4; column++)
	{
		for (row = 0; row < 4; row++)
		{
			matrix[row * 4 + column] = temp[column * 4 + row];
		}
	}
}

GLUSvoid GLUSAPIENTRY glusTranslatef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
	GLUSfloat temp[16];

	glusLoadIdentityf(temp);

	temp[12] = x;
	temp[13] = y;
	temp[14] = z;

	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusRotatef(GLUSfloat matrix[16], const GLUSfloat angle, const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
	GLUSfloat temp[16];

	GLUSfloat s = sinf(2.0f * PIf * angle / 360.0f);
	GLUSfloat c = cosf(2.0f * PIf * angle / 360.0f);

	GLUSfloat vector[3] = { x, y, z };

	GLUSfloat xn, yn, zn;

	glusNormalizef(vector);

	xn = vector[0];
	yn = vector[1];
	zn = vector[2];

	glusLoadIdentityf(temp);

	temp[0] = xn * xn * (1 - c) + c;
	temp[1] = xn * yn * (1 - c) + zn * s;
	temp[2] = xn * zn * (1 - c) - yn * s;

	temp[4] = xn * yn * (1 - c) - zn * s;
	temp[5] = yn * yn * (1 - c) + c;
	temp[6] = yn * zn * (1 - c) + xn * s;

	temp[8] = xn * zn * (1 - c) + yn * s;
	temp[9] = yn * zn * (1 - c) - xn * s;
	temp[10] = zn * zn * (1 - c) + c;

	glusMultMatrixf(matrix, matrix, temp);
}

GLUSAPI GLUSvoid GLUSAPIENTRY glusRotateRyRzRxf(GLfloat matrix[16], const GLfloat anglex, const GLfloat angley, const GLfloat anglez)
{
    GLUSfloat temp[16];

    GLUSfloat ry = 2.0f * PIf * angley / 360.0f;
    GLUSfloat rz = 2.0f * PIf * anglez / 360.0f;
    GLUSfloat rx = 2.0f * PIf * anglex / 360.0f;
    GLUSfloat sx = sinf(rx);
    GLUSfloat cx = cosf(rx);
    GLUSfloat sy = sinf(ry);
    GLUSfloat cy = cosf(ry);
    GLUSfloat sz = sinf(rz);
    GLUSfloat cz = cosf(rz);

    glusLoadIdentityf(temp);

    temp[0] = cy * cz;
    temp[1] = -sz;
    temp[2] = cz * sy;

    temp[4] = sy * sx + cy * cx * sz;
    temp[5] = cz * cx;
    temp[6] = cx * sy * sz - cy * sx;

    temp[8] = cy * sz * sx - cx * sy;
    temp[9] = cz * sx;
    temp[10] = cy * cx + sy * sz * sx;

    glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusRotateRzRyRxf(GLfloat matrix[16], const GLUSfloat anglex, const GLUSfloat angley, const GLUSfloat anglez)
{
    GLUSfloat temp[16];

    GLUSfloat rz = 2.0f * PIf * anglez / 360.0f;
    GLUSfloat ry = 2.0f * PIf * angley / 360.0f;
    GLUSfloat rx = 2.0f * PIf * anglex / 360.0f;
    GLUSfloat sx = sinf(rx);
    GLUSfloat cx = cosf(rx);
    GLUSfloat sy = sinf(ry);
    GLUSfloat cy = cosf(ry);
    GLUSfloat sz = sinf(rz);
    GLUSfloat cz = cosf(rz);

    glusLoadIdentityf(temp);

    temp[0] = cz * cy;
    temp[1] = -cy * sz;
    temp[2] = sy;

    temp[4] = cx * sz + cz * sy * sx;
    temp[5] = cz * cx - sz * sy * sx;
    temp[6] = -cy * sx;

    temp[8] = sz * sx - cz * cx * sy;
    temp[9] = cz * sx + cx * sz * sy;
    temp[10] = cy * cx;

    glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusRotateRzRxRyf(GLfloat matrix[16], const GLfloat anglex, const GLfloat angley, const GLfloat anglez)
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

    glusLoadIdentityf(temp);

    temp[0] = cz * cy + sz * sx * sy;
    temp[1] = cz * sx * sy - cy * sz;
    temp[2] = cx * sy;

    temp[4] = cx * sz;
    temp[5] = cz * cx;
    temp[6] = -sx;

    temp[8] = cy * sz * sx - cz * sy;
    temp[9] = sz * sy + cz * cy * sx;
    temp[10] = cx * cy;

    glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusScalef(GLUSfloat matrix[16], const GLUSfloat x, const GLUSfloat y, const GLUSfloat z)
{
	GLUSfloat temp[16];

	glusLoadIdentityf(temp);

	temp[0] = x;
	temp[5] = y;
	temp[10] = z;

	glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusShearf(GLUSfloat matrix[16], const GLUSfloat shxy, const GLUSfloat shxz, const GLUSfloat shyx, const GLUSfloat shyz, const GLUSfloat shzx, const GLUSfloat shzy)
{
    GLUSfloat temp[16];

    glusLoadIdentityf(temp);

    temp[4] = shxy;
    temp[8] = shxz;

    temp[1] = shyx;
    temp[9] = shyz;

    temp[2] = shzx;
    temp[6] = shzy;

    glusMultMatrixf(matrix, matrix, temp);
}

GLUSvoid GLUSAPIENTRY glusMultVectorf(GLUSfloat result[3], const GLUSfloat matrix[16], const GLUSfloat vector[3])
{
	GLUSuint i;

	GLUSfloat temp[3];

	for (i = 0; i < 3; i++)
		temp[i] = matrix[i] * vector[0] + matrix[4 + i] * vector[1] + matrix[8 + i] * vector[2];

	for (i = 0; i < 3; i++)
		result[i] = temp[i];
}

GLUSvoid GLUSAPIENTRY glusMultPointf(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat point[4])
{
	GLUSuint i;

	GLUSfloat temp[4];

	for (i = 0; i < 4; i++)
		temp[i] = matrix[i] * point[0] + matrix[4 + i] * point[1] + matrix[8 + i] * point[2] + matrix[12 + i] * point[3];

	for (i = 0; i < 4; i++)
		result[i] = temp[i];

	if (temp[3] != 0.0f)
	{
		for (i = 0; i < 4; i++)
			result[i] /= temp[3];
	}
}

GLUSvoid GLUSAPIENTRY glusMultPlanef(GLUSfloat result[4], const GLUSfloat matrix[16], const GLUSfloat plane[4])
{
	GLUSuint i;

	GLUSfloat temp[4];

	for (i = 0; i < 4; i++)
		temp[i] = matrix[i] * plane[0] + matrix[4 + i] * plane[1] + matrix[8 + i] * plane[2] + matrix[12 + i] * plane[3];

	for (i = 0; i < 4; i++)
		result[i] = temp[i];
}
