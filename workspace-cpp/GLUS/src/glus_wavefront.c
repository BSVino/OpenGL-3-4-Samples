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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GLUSboolean GLUSAPIENTRY glusLoadObjFile(const GLUSchar* filename, GLUSshape* shape)
{
    const GLUSuint MAX_ATTRIBUTES = 4096;
    const GLUSuint MAX_TRIANGLE_ATTRIBUTES = 4096*3;
    const GLUSuint BUFFERSIZE = 128;

    FILE* f;

    GLUSchar buffer[BUFFERSIZE];
    GLUSchar identifier[3];

    GLUSfloat x, y, z;
    GLUSfloat s, t;

    GLUSfloat vertices[4 * MAX_ATTRIBUTES];
    GLUSfloat normals[3 * MAX_ATTRIBUTES];
    GLUSfloat texCoords[2 * MAX_ATTRIBUTES];

    GLUSuint numberVertices = 0;
    GLUSuint numberNormals = 0;
    GLUSuint numberTexCoords = 0;

    GLUSfloat triangleVertices[4 * MAX_TRIANGLE_ATTRIBUTES];
    GLUSfloat triangleNormals[3 * MAX_TRIANGLE_ATTRIBUTES];
    GLUSfloat triangleTexCoords[2 * MAX_TRIANGLE_ATTRIBUTES];

    GLUSuint totalNumberVertices= 0;
    GLUSuint totalNumberNormals= 0;
    GLUSuint totalNumberTexCoords= 0;

    GLUSuint facesEncoding = 0;

    if (!filename || !shape)
    {
        return GLUS_FALSE;
    }

    memset(shape, 0, sizeof(GLUSshape));

    f = fopen(filename, "r");

    if (!f)
    {
        return GLUS_FALSE;
    }

    while (!feof(f))
    {
        fgets(buffer, BUFFERSIZE, f);

        if (strncmp(buffer, "vt", 2) == 0)
        {
            sscanf(buffer, "%s %f %f", identifier, &s, &t);

            texCoords[2 * numberTexCoords + 0] = s;
            texCoords[2 * numberTexCoords + 1] = t;

            numberTexCoords++;

            if (numberTexCoords == MAX_ATTRIBUTES)
            {
                fclose(f);

                return GLUS_FALSE;
            }
        }
        else if (strncmp(buffer, "vn", 2) == 0)
        {
            sscanf(buffer, "%s %f %f %f", identifier, &x, &y, &z);

            normals[3 * numberNormals + 0] = x;
            normals[3 * numberNormals + 1] = y;
            normals[3 * numberNormals + 2] = z;

            numberNormals++;

            if (numberNormals == MAX_ATTRIBUTES)
            {
                fclose(f);

                return GLUS_FALSE;
            }
        }
        else if (strncmp(buffer, "v", 1) == 0)
        {
            sscanf(buffer, "%s %f %f %f", identifier, &x, &y, &z);

            vertices[4 * numberVertices + 0] = x;
            vertices[4 * numberVertices + 1] = y;
            vertices[4 * numberVertices + 2] = z;
            vertices[4 * numberVertices + 3] = 1.0f;

            numberVertices++;

            if (numberVertices == MAX_ATTRIBUTES)
            {
                fclose(f);

                return GLUS_FALSE;
            }
        }
        else if (strncmp(buffer, "f", 1) == 0)
        {
            GLUSchar* token;

            GLUSint vIndex, vtIndex, vnIndex;

            GLUSuint edgeCount = 0;

            token = strtok(buffer, " \t");
            token = strtok(NULL, " \n");

            if (totalNumberVertices == 0)
            {
                if (strstr(token, "//") != NULL)
                {
                    facesEncoding = 2;
                }
                else if (strstr(token, "/") == NULL)
                {
                    facesEncoding = 0;
                }
                else if (strstr(token, "/") != NULL)
                {
                    GLUSchar* c = strstr(token, "/");
                    c++;
                    if (strstr(c, "/") == NULL)
                    {
                        facesEncoding = 1;
                    }
                    else
                    {
                        facesEncoding = 3;
                    }
                }
            }

            while (token != NULL)
            {
                vIndex = -1;
                vtIndex = -1;
                vnIndex = -1;

                switch (facesEncoding)
                {
                    case 0:
                        sscanf(token, "%d", &vIndex);
                        break;
                    case 1:
                        sscanf(token, "%d/%d", &vIndex, &vtIndex);
                        break;
                    case 2:
                        sscanf(token, "%d//%d", &vIndex, &vnIndex);
                        break;
                    case 3:
                        sscanf(token, "%d/%d/%d", &vIndex, &vtIndex, &vnIndex);
                        break;
                }

                vIndex--;
                vtIndex--;
                vnIndex--;

                if (vIndex >= 0)
                {
                    if (edgeCount < 3)
                    {
                        memcpy(&triangleVertices[4*totalNumberVertices], &vertices[4*vIndex], 4*sizeof(GLUSfloat));

                        totalNumberVertices++;
                    }
                    else
                    {
                        memcpy(&triangleVertices[4*(totalNumberVertices)], &triangleVertices[4*(totalNumberVertices-edgeCount)], 4*sizeof(GLUSfloat));
                        memcpy(&triangleVertices[4*(totalNumberVertices+1)], &triangleVertices[4*(totalNumberVertices-1)], 4*sizeof(GLUSfloat));
                        memcpy(&triangleVertices[4*(totalNumberVertices+2)], &vertices[4*vIndex], 4*sizeof(GLUSfloat));

                        totalNumberVertices += 3;
                    }
                }
                if (vnIndex >= 0)
                {
                    if (edgeCount < 3)
                    {
                        memcpy(&triangleNormals[3*totalNumberNormals], &normals[3*vnIndex], 3*sizeof(GLUSfloat));

                        totalNumberNormals++;
                    }
                    else
                    {
                        memcpy(&triangleNormals[3*(totalNumberNormals)], &triangleNormals[3*(totalNumberNormals-edgeCount)], 3*sizeof(GLUSfloat));
                        memcpy(&triangleNormals[3*(totalNumberNormals+1)], &triangleNormals[3*(totalNumberNormals-1)], 3*sizeof(GLUSfloat));
                        memcpy(&triangleNormals[3*(totalNumberNormals+2)], &normals[3*vnIndex], 3*sizeof(GLUSfloat));

                        totalNumberNormals += 3;
                    }
                }
                if (vtIndex >= 0)
                {
                    if (edgeCount < 3)
                    {
                        memcpy(&triangleTexCoords[2*totalNumberTexCoords], &texCoords[2*vtIndex], 2*sizeof(GLUSfloat));

                        totalNumberTexCoords++;
                    }
                    else
                    {
                        memcpy(&triangleTexCoords[2*(totalNumberTexCoords)], &triangleTexCoords[2*(totalNumberTexCoords-edgeCount)], 2*sizeof(GLUSfloat));
                        memcpy(&triangleTexCoords[2*(totalNumberTexCoords+1)], &triangleTexCoords[2*(totalNumberTexCoords-1)], 2*sizeof(GLUSfloat));
                        memcpy(&triangleTexCoords[2*(totalNumberTexCoords+2)], &texCoords[2*vtIndex], 2*sizeof(GLUSfloat));

                        totalNumberTexCoords += 3;
                    }
                }

                edgeCount++;

                token = strtok(NULL, " \n");

                if (token != NULL && totalNumberVertices == MAX_TRIANGLE_ATTRIBUTES)
                {
                    return GL_FALSE;
                }
            }
        }
    }

    fclose(f);

    shape->numberVertices = totalNumberVertices;

    if (totalNumberVertices > 0)
    {
        shape->vertices = (GLUSfloat*)malloc(totalNumberVertices*4*sizeof(GLUSfloat));

        if (shape->vertices == NULL)
        {
            glusDestroyShapef(shape);

            return GLUS_FALSE;
        }

        memcpy(shape->vertices, triangleVertices, totalNumberVertices*4*sizeof(GLUSfloat));
    }
    if (totalNumberNormals > 0)
    {
        shape->normals = (GLUSfloat*)malloc(totalNumberNormals*3*sizeof(GLUSfloat));

        if (shape->normals == NULL)
        {
            glusDestroyShapef(shape);

            return GLUS_FALSE;
        }

        memcpy(shape->normals, triangleNormals, totalNumberNormals*3*sizeof(GLUSfloat));
    }
    if (totalNumberTexCoords > 0)
    {
        shape->texCoords = (GLUSfloat*)malloc(totalNumberTexCoords*2*sizeof(GLUSfloat));

        if (shape->texCoords == NULL)
        {
            glusDestroyShapef(shape);

            return GLUS_FALSE;
        }

        memcpy(shape->texCoords, triangleTexCoords, totalNumberTexCoords*2*sizeof(GLUSfloat));
    }

    return GLUS_TRUE;
}
