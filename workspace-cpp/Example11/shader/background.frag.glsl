#version 150

uniform samplerCube u_cubemap;

in vec3 v_ray;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	fragColor = texture(u_cubemap, v_ray);
}
