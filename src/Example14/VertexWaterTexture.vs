#version 410 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec2 texCoord;

out vec2 fragmentTexCoord;

void main(void)
{
	fragmentTexCoord = texCoord;

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
