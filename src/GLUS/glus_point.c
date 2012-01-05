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

GLUSvoid GLUSAPIENTRY glusSubtractPointf(GLUSfloat result[3], const GLUSfloat point0[4], const GLUSfloat point1[4])
{
	result[0] = point0[0] - point1[0];
	result[1] = point0[1] - point1[1];
	result[2] = point0[2] - point1[2];
}

GLUSvoid GLUSAPIENTRY glusAddVectorPointf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3])
{
	result[0] = point[0] + vector[0];
	result[1] = point[1] + vector[1];
	result[2] = point[2] + vector[2];
	result[3] = point[3];
}

GLUSvoid GLUSAPIENTRY glusSubtractVectorPointf(GLUSfloat result[4], const GLUSfloat point[4], const GLUSfloat vector[3])
{
	result[0] = point[0] - vector[0];
	result[1] = point[1] - vector[1];
	result[2] = point[2] - vector[2];
	result[3] = point[3];
}
