#version 150

uniform sampler2D firstTexture; 

in vec3 fragmentNormal;
in vec2 fragmentTexCoord;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float intensity;
	
	intensity = max(dot(fragmentNormal, vec3(0.0, 0.0, 1.0)), 0.0);
	
	fragColor = texture(firstTexture, fragmentTexCoord)*intensity;
}
