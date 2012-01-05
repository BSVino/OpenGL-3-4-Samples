#version 150

in vec3 fragmentNormal;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float intensity;
	
	intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
	
	fragColor = vec4(1.0, 0.0, 0.0, 1.0)*intensity;
}
