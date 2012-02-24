#version 150

uniform samplerCube u_cubemapTexture;

in vec3 v_reflect;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	fragColor = texture(u_cubemapTexture, v_reflect);
}
