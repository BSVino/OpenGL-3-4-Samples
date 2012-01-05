#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform mat4 shadowMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 fragmentNormal;
out vec3 fragmentEye;

out vec4 projCoord;

void main(void)
{
	fragmentNormal = (modelViewMatrix*vec4(normal, 0.0)).xyz;

	fragmentEye = (modelViewMatrix*vertex).xyz;
	fragmentEye = -normalize(fragmentEye);

	projCoord = shadowMatrix*modelViewMatrix*vertex;

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
