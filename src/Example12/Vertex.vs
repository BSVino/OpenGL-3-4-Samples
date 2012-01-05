#version 410 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;

void main(void)
{
	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
