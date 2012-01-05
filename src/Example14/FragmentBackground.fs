#version 410 core

uniform samplerCube cubemap;

in vec3 cubeMapVector;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	vec4 environmentColor = texture(cubemap, cubeMapVector);
	
	fragColor = environmentColor;
}
