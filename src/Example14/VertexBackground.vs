#version 410 core

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 cubeMapVector;

void main(void)
{
	cubeMapVector = normalize(vertex.xyz);
	
	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
