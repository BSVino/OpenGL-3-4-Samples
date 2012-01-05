#version 410 core

layout(location = 0, index = 0) out vec4 fragColor;

uniform sampler2D colorMapTexture;

uniform vec3 lightDir;

in vec2 fragmentTexCoord;

in vec3 fragmentNormal;

void main(void)
{
	float intensity = max(dot(normalize(fragmentNormal), normalize(lightDir)), 0.0);
	
	fragColor = texture(colorMapTexture, fragmentTexCoord) * intensity;
}
