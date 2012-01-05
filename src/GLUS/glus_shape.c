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

#include <stdlib.h>
#include <string.h>
#include <math.h>

GLUSvoid glusCreateAllAtributes(GLUSshape* shape)
{
    GLUSuint i;

    GLUSuint stride = 4 + 3 + 3 + 2;

    shape->allAttributes = (GLUSfloat*) malloc(stride * shape->numberVertices * sizeof(GLUSfloat));

    for (i = 0; i < shape->numberVertices; i++)
    {
        shape->allAttributes[i * stride + 0] = shape->vertices[i * 4 + 0];
        shape->allAttributes[i * stride + 1] = shape->vertices[i * 4 + 1];
        shape->allAttributes[i * stride + 2] = shape->vertices[i * 4 + 2];
        shape->allAttributes[i * stride + 3] = shape->vertices[i * 4 + 3];

        shape->allAttributes[i * stride + 4] = shape->normals[i * 3 + 0];
        shape->allAttributes[i * stride + 5] = shape->normals[i * 3 + 1];
        shape->allAttributes[i * stride + 6] = shape->normals[i * 3 + 2];

        shape->allAttributes[i * stride + 7] = shape->tangents[i * 3 + 0];
        shape->allAttributes[i * stride + 8] = shape->tangents[i * 3 + 1];
        shape->allAttributes[i * stride + 9] = shape->tangents[i * 3 + 2];

        shape->allAttributes[i * stride + 10] = shape->texCoords[i * 2 + 0];
        shape->allAttributes[i * stride + 11] = shape->texCoords[i * 2 + 1];
    }
}

GLUSvoid GLUSAPIENTRY glusCreatePlanef(GLUSshape* shape, const GLUSfloat halfExtend)
{
	GLUSuint i;

	GLUSuint numberVertices = 4;
	GLUSuint numberIndices = 6;

	GLUSfloat xy_vertices[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };

	GLUSfloat xy_normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

	GLUSfloat xy_tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

	GLUSfloat xy_texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	GLUSuint xy_indices[] = { 0, 1, 2, 1, 3, 2 };

	if (!shape)
	{
		return;
	}

	shape->vertices = 0;
	shape->normals = 0;
	shape->tangents = 0;
	shape->texCoords = 0;
	shape->indices = 0;
	shape->numberVertices = 0;
	shape->numberIndices = 0;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLUSfloat*) malloc(4 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->vertices, xy_vertices, sizeof(xy_vertices));
	for (i = 0; i < numberVertices; i++)
	{
		shape->vertices[i * 4 + 0] *= halfExtend;
		shape->vertices[i * 4 + 1] *= halfExtend;
	}

	shape->normals = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->normals, xy_normals, sizeof(xy_normals));

	shape->tangents = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->tangents, xy_tangents, sizeof(xy_tangents));

	shape->texCoords = (GLUSfloat*) malloc(2 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->texCoords, xy_texCoords, sizeof(xy_texCoords));

	shape->indices = (GLUSuint*) malloc(numberIndices * sizeof(GLUSuint));
	memcpy(shape->indices, xy_indices, sizeof(xy_indices));

	glusCreateAllAtributes(shape);
}

GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateRectangularPlanef(GLUSshape* shape, const GLUSfloat horizontalExtend, const GLUSfloat verticalExtend)
{
	GLUSuint i;

	GLUSuint numberVertices = 4;
	GLUSuint numberIndices = 6;

	GLUSfloat xy_vertices[] = { -1.0f, -1.0f, 0.0f, +1.0f, +1.0f, -1.0f, 0.0f, +1.0f, -1.0f, +1.0f, 0.0f, +1.0f, +1.0f, +1.0f, 0.0f, +1.0f };

	GLUSfloat xy_normals[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f };

	GLUSfloat xy_tangents[] = { 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };

	GLUSfloat xy_texCoords[] = { 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };

	GLUSuint xy_indices[] = { 0, 1, 2, 1, 3, 2 };

	if (!shape)
	{
		return;
	}

	shape->vertices = 0;
	shape->normals = 0;
	shape->tangents = 0;
	shape->texCoords = 0;
	shape->indices = 0;
	shape->numberVertices = 0;
	shape->numberIndices = 0;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLUSfloat*) malloc(4 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->vertices, xy_vertices, sizeof(xy_vertices));
	for (i = 0; i < numberVertices; i++)
	{
		shape->vertices[i * 4 + 0] *= horizontalExtend;
		shape->vertices[i * 4 + 1] *= verticalExtend;
	}

	shape->normals = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->normals, xy_normals, sizeof(xy_normals));

	shape->tangents = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->tangents, xy_tangents, sizeof(xy_tangents));

	shape->texCoords = (GLUSfloat*) malloc(2 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->texCoords, xy_texCoords, sizeof(xy_texCoords));

	shape->indices = (GLUSuint*) malloc(numberIndices * sizeof(GLUSuint));
	memcpy(shape->indices, xy_indices, sizeof(xy_indices));

	glusCreateAllAtributes(shape);
}

GLUSvoid GLUSAPIENTRY glusCreateCubef(GLUSshape* shape, const GLUSfloat halfExtend)
{
	GLUSuint i;

	GLUSuint numberVertices = 24;
	GLUSuint numberIndices = 36;

	GLUSfloat
			cubeVertices[] =
					{ -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, -1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f, -1.0f, +1.0f, +1.0f, -1.0f, -1.0f, +1.0f, +1.0f, -1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, +1.0f, -1.0f, +1.0f };

	GLUSfloat
			cubeNormals[] =
					{ 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f };

	GLUSfloat cubeTangents[] = { -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	+1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,

	-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	+1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f,

	0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f, 0.0f, 0.0f, +1.0f,

	0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f };

	GLUSfloat
			cubeTexCoords[] =
					{ 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, };

	GLUSuint cubeIndices[] =
			{ 0, 2, 1, 0, 3, 2, 4, 5, 6, 4, 6, 7, 8, 9, 10, 8, 10, 11, 12, 15, 14, 12, 14, 13, 16, 17, 18, 16, 18, 19, 20, 23, 22, 20, 22, 21 };

	if (!shape)
	{
		return;
	}

	shape->vertices = 0;
	shape->normals = 0;
	shape->tangents = 0;
	shape->texCoords = 0;
	shape->indices = 0;
	shape->numberVertices = 0;
	shape->numberIndices = 0;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLUSfloat*) malloc(4 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->vertices, cubeVertices, sizeof(cubeVertices));
	for (i = 0; i < numberVertices; i++)
	{
		shape->vertices[i * 4 + 0] *= halfExtend;
		shape->vertices[i * 4 + 1] *= halfExtend;
		shape->vertices[i * 4 + 2] *= halfExtend;
	}

	shape->normals = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->normals, cubeNormals, sizeof(cubeNormals));

	shape->tangents = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->tangents, cubeTangents, sizeof(cubeTangents));

	shape->texCoords = (GLUSfloat*) malloc(2 * numberVertices * sizeof(GLUSfloat));
	memcpy(shape->texCoords, cubeTexCoords, sizeof(cubeTexCoords));

	shape->indices = (GLUSuint*) malloc(numberIndices * sizeof(GLUSuint));
	memcpy(shape->indices, cubeIndices, sizeof(cubeIndices));

	glusCreateAllAtributes(shape);
}

GLUSvoid GLUSAPIENTRY glusCreateSpheref(GLUSshape* shape, const GLUSfloat radius, const GLUSuint numberSlices)
{
	GLUSuint i, j;

	GLUSuint numberParallels = numberSlices;
	GLUSuint numberVertices = (numberParallels + 1) * (numberSlices + 1);
	GLUSuint numberIndices = numberParallels * numberSlices * 6;

	GLUSfloat angleStep = (2.0f * PIf) / ((GLUSfloat) numberSlices);

	GLUSuint* indexBuf;

	GLUSfloat helpVector[3] = { 0.0f, 1.0f, 0.0f };

	if (!shape)
	{
		return;
	}

	shape->vertices = 0;
	shape->normals = 0;
	shape->tangents = 0;
	shape->texCoords = 0;
	shape->indices = 0;
	shape->numberVertices = 0;
	shape->numberIndices = 0;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLUSfloat*) malloc(4 * numberVertices * sizeof(GLUSfloat));

	shape->normals = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));

	shape->tangents = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));

	shape->texCoords = (GLUSfloat*) malloc(2 * numberVertices * sizeof(GLUSfloat));

	shape->indices = (GLUSuint*) malloc(numberIndices * sizeof(GLUSuint));

	for (i = 0; i < numberParallels + 1; i++)
	{
		for (j = 0; j < numberSlices + 1; j++)
		{
			GLUSuint vertexIndex = (i * (numberSlices + 1) + j) * 4;
			GLUSuint normalIndex = (i * (numberSlices + 1) + j) * 3;
			GLUSuint tangentIndex = (i * (numberSlices + 1) + j) * 3;
			GLUSuint texCoordsIndex = (i * (numberSlices + 1) + j) * 2;

			shape->vertices[vertexIndex + 0] = radius * sinf(angleStep * (GLUSfloat) i) * sinf(angleStep * (GLUSfloat) j);
			shape->vertices[vertexIndex + 1] = radius * cosf(angleStep * (GLUSfloat) i);
			shape->vertices[vertexIndex + 2] = radius * sinf(angleStep * (GLUSfloat) i) * cosf(angleStep * (GLUSfloat) j);
			shape->vertices[vertexIndex + 3] = 1.0f;

			shape->normals[normalIndex + 0] = shape->vertices[vertexIndex + 0] / radius;
			shape->normals[normalIndex + 1] = shape->vertices[vertexIndex + 1] / radius;
			shape->normals[normalIndex + 2] = shape->vertices[vertexIndex + 2] / radius;

			glusCrossf(&shape->tangents[tangentIndex], &shape->normals[normalIndex], helpVector);

			if (glusAmountf(&shape->tangents[tangentIndex]) == 0.0f)
			{
				shape->tangents[tangentIndex + 0] = 1.0f;
				shape->tangents[tangentIndex + 1] = 0.0f;
				shape->tangents[tangentIndex + 2] = 0.0f;
			}

			shape->texCoords[texCoordsIndex + 0] = (GLUSfloat) j / (GLUSfloat) numberSlices;
			shape->texCoords[texCoordsIndex + 1] = (1.0f - (GLUSfloat) i) / (GLUSfloat)(numberParallels - 1);
		}
	}

	indexBuf = shape->indices;
	for (i = 0; i < numberParallels; i++)
	{
		for (j = 0; j < numberSlices; j++)
		{
			*indexBuf++ = i * (numberSlices + 1) + j;
			*indexBuf++ = (i + 1) * (numberSlices + 1) + j;
			*indexBuf++ = (i + 1) * (numberSlices + 1) + (j + 1);

			*indexBuf++ = i * (numberSlices + 1) + j;
			*indexBuf++ = (i + 1) * (numberSlices + 1) + (j + 1);
			*indexBuf++ = i * (numberSlices + 1) + (j + 1);
		}
	}

	glusCreateAllAtributes(shape);
}

/**
 * @author Pablo Alonso-Villaverde Roza
 */
GLUSAPI GLUSvoid GLUSAPIENTRY glusCreateTorusf(GLUSshape* shape, const GLUSfloat innerRadius, const GLUSfloat outerRadius, const GLUSuint numSides, const GLUSuint numFaces)
{
	// t, s = parametric values of the equations, in the range [0,1]
	GLUSfloat t = 0;
	GLUSfloat s = 0;

	// incr_t, incr_s are increment values aplied to t and s on each loop iteration	to generate the torus
	GLUSfloat tIncr;
	GLUSfloat sIncr;

	// to store precomputed sin and cos values 
	GLUSfloat cos2PIt, sin2PIt, cos2PIs, sin2PIs;

	GLUSuint numberVertices;
	GLUSuint numberIndices;

	// used later to help us calculating tangents vectors
	GLUSfloat helpVector[3] = { 0.0f, 1.0f, 0.0f };

	// indices for each type of buffer (of vertices, indices, normals...)
	GLUSuint indexVertices, indexIndices, indexNormals, indexTangents, indexTexCoords;

	// loop counters
	GLUSuint sideCount, faceCount;

	// used to generate the indices
	GLUSuint v0, v1, v2, v3;

	GLUSfloat torusRadius = (outerRadius - innerRadius) / 2.0f;
	GLUSfloat centerRadius = outerRadius - torusRadius;

	if ((!shape) || (numSides < 3) || (numFaces < 3))
	{
		return;
	}

	numberVertices = (numFaces + 1) * (numSides + 1);
	numberIndices = numFaces * numSides * 2 * 3; // 2 triangles per face * 3 indices per triangle

	shape->vertices = 0;
	shape->normals = 0;
	shape->tangents = 0;
	shape->texCoords = 0;
	shape->indices = 0;
	shape->numberVertices = 0;
	shape->numberIndices = 0;

	shape->numberVertices = numberVertices;
	shape->numberIndices = numberIndices;

	shape->vertices = (GLUSfloat*) malloc(4 * numberVertices * sizeof(GLUSfloat));
	shape->normals = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	shape->tangents = (GLUSfloat*) malloc(3 * numberVertices * sizeof(GLUSfloat));
	shape->texCoords = (GLUSfloat*) malloc(2 * numberVertices * sizeof(GLUSfloat));
	shape->indices = (GLUSuint*) malloc(numberIndices * sizeof(GLUSuint));

	tIncr = 1.0f / (GLUSfloat) numFaces;
	sIncr = 1.0f / (GLUSfloat) numSides;

	// generate vertices and its attributes
	for (sideCount = 0; sideCount <= numSides; ++sideCount, s += sIncr)
	{
		// precompute some values
		cos2PIs = (GLUSfloat) cos(2.0f * PIf * s);
		sin2PIs = (GLUSfloat) sin(2.0f * PIf * s);

		t = 0.0f;
		for (faceCount = 0; faceCount <= numFaces; ++faceCount, t += tIncr)
		{
			// precompute some values
			cos2PIt = (GLUSfloat) cos(2.0f * PIf * t);
			sin2PIt = (GLUSfloat) sin(2.0f * PIf * t);

			// generate vertex and stores it in the right position
			indexVertices = ((sideCount * (numFaces + 1)) + faceCount) * 4;
			shape->vertices[indexVertices + 0] = (centerRadius + torusRadius * cos2PIt) * cos2PIs;
			shape->vertices[indexVertices + 1] = (centerRadius + torusRadius * cos2PIt) * sin2PIs;
			shape->vertices[indexVertices + 2] = torusRadius * sin2PIt;
			shape->vertices[indexVertices + 3] = 1.0f;

			// generate normal and stores it in the right position
			// NOTE: cos (2PIx) = cos (x) and sin (2PIx) = sin (x) so, we can use this formula
			//       normal = {cos(2PIs)cos(2PIt) , sin(2PIs)cos(2PIt) ,sin(2PIt)}      
			indexNormals = ((sideCount * (numFaces + 1)) + faceCount) * 3;
			shape->normals[indexNormals + 0] = cos2PIs * cos2PIt;
			shape->normals[indexNormals + 1] = sin2PIs * cos2PIt;
			shape->normals[indexNormals + 2] = sin2PIt;

			// tangent vector can be calculated with a cross product between the helper vector, and the normal vector
			// We must take care if both the normal and helper are parallel (cross product = 0, that's not a valid tangent!)			
			indexTangents = ((sideCount * (numFaces + 1)) + faceCount) * 3;
			glusCrossf(&shape->tangents[indexTangents], &shape->normals[indexNormals], helpVector);

			if (glusAmountf(&shape->tangents[indexTangents]) == 0.0f)
			{
				shape->tangents[indexTangents + 0] = 1.0f;
				shape->tangents[indexTangents + 1] = 0.0f;
				shape->tangents[indexTangents + 2] = 0.0f;
			}

			// generate texture coordinates and stores it in the right position
			indexTexCoords = ((sideCount * (numFaces + 1)) + faceCount) * 2;
			shape->texCoords[indexTexCoords + 0] = t;
			shape->texCoords[indexTexCoords + 1] = s;

		}
	}

	// generate indices
	indexIndices = 0;
	for (sideCount = 0; sideCount < numSides; ++sideCount)
	{
		for (faceCount = 0; faceCount < numFaces; ++faceCount)
		{
			// get the number of the vertices for a face of the torus. They must be < numVertices
			v0 = ((sideCount * (numFaces + 1)) + faceCount);
			v1 = (((sideCount + 1) * (numFaces + 1)) + faceCount);
			v2 = (((sideCount + 1) * (numFaces + 1)) + (faceCount + 1));
			v3 = ((sideCount * (numFaces + 1)) + (faceCount + 1));

			// first triangle of the face, counter clock wise winding		
			shape->indices[indexIndices++] = v0;
			shape->indices[indexIndices++] = v1;
			shape->indices[indexIndices++] = v2;

			// second triangle of the face, counter clock wise winding
			shape->indices[indexIndices++] = v0;
			shape->indices[indexIndices++] = v2;
			shape->indices[indexIndices++] = v3;
		}
	}

	glusCreateAllAtributes(shape);
}

GLUSvoid GLUSAPIENTRY glusDestroyShapef(GLUSshape* shape)
{
	if (!shape)
	{
		return;
	}

	if (shape->vertices)
	{
		free(shape->vertices);

		shape->vertices = 0;
	}

	if (shape->normals)
	{
		free(shape->normals);

		shape->normals = 0;
	}

	if (shape->tangents)
	{
		free(shape->tangents);

		shape->tangents = 0;
	}

	if (shape->texCoords)
	{
		free(shape->texCoords);

		shape->texCoords = 0;
	}

    if (shape->allAttributes)
    {
        free(shape->allAttributes);

        shape->allAttributes = 0;
    }

	if (shape->indices)
	{
		free(shape->indices);

		shape->indices = 0;
	}

	shape->numberVertices = 0;
	shape->numberIndices = 0;
}
