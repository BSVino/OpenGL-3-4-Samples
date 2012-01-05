#version 150

uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 geometryNormal;

void main(void)
{
	geometryNormal = (modelViewMatrix*vec4(normal, 0.0)).xyz;

	gl_Position = vertex;
}
