#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec4 vertex;
in vec3 normal;
in vec2 texCoord;

out vec3 fragmentNormal;
out vec2 fragmentTexCoord;

void main(void)
{
	fragmentTexCoord = texCoord;

	fragmentNormal = (modelViewMatrix*vec4(normal, 0.0)).xyz;

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
