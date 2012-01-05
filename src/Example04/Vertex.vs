#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 fragmentNormal;
out vec3 fragmentEye;

void main(void)
{
	fragmentNormal = (modelViewMatrix*vec4(normal, 0.0)).xyz;

	fragmentEye = (modelViewMatrix*vertex).xyz;
	fragmentEye = -normalize(fragmentEye);

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
