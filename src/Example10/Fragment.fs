#version 150

uniform samplerCube cubemap;

in vec3 fragmentReflection;
in vec3 fragmentRefraction;
in float fresnel;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	vec4 reflectionColor = texture(cubemap, fragmentReflection);

	vec4 refractionColor = texture(cubemap, fragmentRefraction);
		
	fragColor = mix(refractionColor, reflectionColor, fresnel);
}
