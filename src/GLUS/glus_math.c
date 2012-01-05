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

GLUSfloat GLUSAPIENTRY glusMaxf(const GLUSfloat value0, const GLUSfloat value1)
{
	return value0 >= value1 ? value0 : value1;
}

GLUSfloat GLUSAPIENTRY glusMinf(const GLUSfloat value0, const GLUSfloat value1)
{
	return value0 <= value1 ? value0 : value1;
}

GLUSfloat GLUSAPIENTRY glusAbsf(const GLUSfloat value)
{
	return value >= 0.0 ? value : -value;
}

GLUSfloat GLUSAPIENTRY glusLengthf(const GLUSfloat v0, const GLUSfloat v1, const GLUSfloat v2)
{
	return sqrtf(v0*v0 + v1*v1 + v2*v2);
}
