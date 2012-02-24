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

GLUSboolean GLUSAPIENTRY glusCreateProgramFromSource(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
{
    GLUSint compiled;

    GLUSint logLength, charsWritten;

    char* log;

    if (!shaderProgram || !vertexSource || !fragmentSource)
    {
        return GLUS_FALSE;
    }

    shaderProgram->program = 0;
    shaderProgram->vertex = 0;
    shaderProgram->control = 0;
    shaderProgram->evaluation = 0;
    shaderProgram->geometry = 0;
    shaderProgram->fragment = 0;

    // http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
    shaderProgram->vertex = glCreateShader(GLUS_VERTEX_SHADER);

    // http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
    glShaderSource(shaderProgram->vertex, 1, (const char**) vertexSource, 0);

    // http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
    glCompileShader(shaderProgram->vertex);

    // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
    glGetShaderiv(shaderProgram->vertex, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
        glGetShaderiv(shaderProgram->vertex, GL_INFO_LOG_LENGTH, &logLength);

        log = (char*) malloc(logLength);

        if (!log)
        {
            return GLUS_FALSE;
        }

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
        glGetShaderInfoLog(shaderProgram->vertex, logLength, &charsWritten, log);

        printf("Vertex shader compile error:\n");
        printf("%s\n", log);

        free(log);

        shaderProgram->vertex = 0;

        return GLUS_FALSE;
    }

    if (controlSource)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
        shaderProgram->control = glCreateShader(GLUS_TESS_CONTROL_SHADER);

        // http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
        glShaderSource(shaderProgram->control, 1, (const char**) controlSource, 0);

        // http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
        glCompileShader(shaderProgram->control);

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
        glGetShaderiv(shaderProgram->control, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
            glGetShaderiv(shaderProgram->control, GL_INFO_LOG_LENGTH, &logLength);

            log = (char*) malloc(logLength);

            if (!log)
            {
                glusDestroyProgram(shaderProgram);

                return GLUS_FALSE;
            }

            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
            glGetShaderInfoLog(shaderProgram->control, logLength, &charsWritten, log);

            printf("Control shader compile error:\n");
            printf("%s\n", log);

            free(log);

            shaderProgram->control = 0;

            glusDestroyProgram(shaderProgram);

            return GLUS_FALSE;
        }
    }

    if (evaluationSource)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
        shaderProgram->evaluation = glCreateShader(GLUS_TESS_EVALUATION_SHADER);

        // http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
        glShaderSource(shaderProgram->evaluation, 1, (const char**) evaluationSource, 0);

        // http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
        glCompileShader(shaderProgram->evaluation);

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
        glGetShaderiv(shaderProgram->evaluation, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
            glGetShaderiv(shaderProgram->evaluation, GL_INFO_LOG_LENGTH, &logLength);

            log = (char*) malloc(logLength);

            if (!log)
            {
                glusDestroyProgram(shaderProgram);

                return GLUS_FALSE;
            }

            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
            glGetShaderInfoLog(shaderProgram->evaluation, logLength, &charsWritten, log);

            printf("Evaluation shader compile error:\n");
            printf("%s\n", log);

            free(log);

            shaderProgram->evaluation = 0;

            glusDestroyProgram(shaderProgram);

            return GLUS_FALSE;
        }
    }

    if (geometrySource)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
        shaderProgram->geometry = glCreateShader(GLUS_GEOMETRY_SHADER);

        // http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
        glShaderSource(shaderProgram->geometry, 1, (const char**) geometrySource, 0);

        // http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
        glCompileShader(shaderProgram->geometry);

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
        glGetShaderiv(shaderProgram->geometry, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
            glGetShaderiv(shaderProgram->geometry, GL_INFO_LOG_LENGTH, &logLength);

            log = (char*) malloc(logLength);

            if (!log)
            {
                glusDestroyProgram(shaderProgram);

                return GLUS_FALSE;
            }

            // http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
            glGetShaderInfoLog(shaderProgram->geometry, logLength, &charsWritten, log);

            printf("Geometry shader compile error:\n");
            printf("%s\n", log);

            free(log);

            shaderProgram->geometry = 0;

            glusDestroyProgram(shaderProgram);

            return GLUS_FALSE;
        }
    }

    // http://www.opengl.org/sdk/docs/man/xhtml/glCreateShader.xml
    shaderProgram->fragment = glCreateShader(GLUS_FRAGMENT_SHADER);

    // http://www.opengl.org/sdk/docs/man/xhtml/glShaderSource.xml
    glShaderSource(shaderProgram->fragment, 1, (const char**) fragmentSource, 0);

    // http://www.opengl.org/sdk/docs/man/xhtml/glCompileShader.xml
    glCompileShader(shaderProgram->fragment);

    // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
    glGetShaderiv(shaderProgram->fragment, GL_COMPILE_STATUS, &compiled);

    if (!compiled)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShader.xml
        glGetShaderiv(shaderProgram->fragment, GL_INFO_LOG_LENGTH, &logLength);

        log = (char*) malloc(logLength);

        if (!log)
        {
            glusDestroyProgram(shaderProgram);

            return GLUS_FALSE;
        }

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetShaderInfoLog.xml
        glGetShaderInfoLog(shaderProgram->fragment, logLength, &charsWritten, log);

        printf("Fragment shader compile error:\n");
        printf("%s\n", log);

        free(log);

        shaderProgram->fragment = 0;

        glusDestroyProgram(shaderProgram);

        return GLUS_FALSE;
    }

    // http://www.opengl.org/sdk/docs/man/xhtml/glCreateProgram.xml
    shaderProgram->program = glCreateProgram();

    // http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
    glAttachShader(shaderProgram->program, shaderProgram->vertex);

    if (shaderProgram->control)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
        glAttachShader(shaderProgram->program, shaderProgram->control);
    }

    if (shaderProgram->evaluation)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
        glAttachShader(shaderProgram->program, shaderProgram->evaluation);
    }

    if (shaderProgram->geometry)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
        glAttachShader(shaderProgram->program, shaderProgram->geometry);
    }

    // http://www.opengl.org/sdk/docs/man/xhtml/glAttachShader.xml
    glAttachShader(shaderProgram->program, shaderProgram->fragment);

    return GLUS_TRUE;
}

GLUSboolean GLUSAPIENTRY glusLinkProgram(GLUSshaderprogram* shaderProgram)
{
    GLUSint linked;

    GLUSint logLength, charsWritten;

    char* log;

    if (!shaderProgram)
    {
        return GLUS_FALSE;
    }

    // http://www.opengl.org/sdk/docs/man/xhtml/glLinkProgram.xml
    glLinkProgram(shaderProgram->program);

    // http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
    glGetProgramiv(shaderProgram->program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glGetProgram.xml
        glGetProgramiv(shaderProgram->program, GL_INFO_LOG_LENGTH, &logLength);

        log = (char*) malloc(logLength);

        if (!log)
        {
            glusDestroyProgram(shaderProgram);

            return GLUS_FALSE;
        }

        // http://www.opengl.org/sdk/docs/man/xhtml/glGetProgramInfoLog.xml
        glGetProgramInfoLog(shaderProgram->program, logLength, &charsWritten, log);

        printf("Shader program link error:\n");
        printf("%s\n", log);

        free(log);

        shaderProgram->program = 0;

        glusDestroyProgram(shaderProgram);

        return GLUS_FALSE;
    }

    return GLUS_TRUE;
}

GLUSboolean GLUSAPIENTRY glusBuildProgramFromSource(GLUSshaderprogram* shaderProgram, const GLUSchar** vertexSource, const GLUSchar** controlSource, const GLUSchar** evaluationSource, const GLUSchar** geometrySource, const GLUSchar** fragmentSource)
{
    if (!glusCreateProgramFromSource(shaderProgram, vertexSource, controlSource, evaluationSource, geometrySource, fragmentSource))
    {
        return GLUS_FALSE;
    }

    return glusLinkProgram(shaderProgram);
}

GLUSvoid GLUSAPIENTRY glusDestroyProgram(GLUSshaderprogram* shaderprogram)
{
    if (!shaderprogram)
    {
        return;
    }

    if (shaderprogram->program)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteProgram.xml
        glDeleteProgram(shaderprogram->program);

        shaderprogram->program = 0;
    }

    if (shaderprogram->fragment)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
        glDeleteShader(shaderprogram->fragment);

        shaderprogram->fragment = 0;
    }

    if (shaderprogram->geometry)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
        glDeleteShader(shaderprogram->geometry);

        shaderprogram->geometry = 0;
    }

    if (shaderprogram->evaluation)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
        glDeleteShader(shaderprogram->evaluation);

        shaderprogram->evaluation = 0;
    }

    if (shaderprogram->control)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
        glDeleteShader(shaderprogram->control);

        shaderprogram->control = 0;
    }

    if (shaderprogram->vertex)
    {
        // http://www.opengl.org/sdk/docs/man/xhtml/glDeleteShader.xml
        glDeleteShader(shaderprogram->vertex);

        shaderprogram->vertex = 0;
    }
}
