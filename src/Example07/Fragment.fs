#version 150

uniform samplerCube cubemap;

in vec3 fragmentNormal;
in vec3 fragmentReflection;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float intensity;
	
	vec4 environmentColor = texture(cubemap, fragmentReflection);
	
	intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
	
	fragColor = environmentColor * intensity;
}
