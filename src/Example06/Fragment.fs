#version 150

uniform sampler2D firstTexture; 
uniform sampler2D normalMap; 

in vec2 fragmentTexCoord;

in vec3 textureLight;
in vec3 textureEye;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float diffuseIntensity;
	float specularItensity;

	vec3 light;

	vec3 normal;
	vec3 eye;

	vec3 reflection;

	light = normalize(textureLight);
	
	normal = normalize( texture(normalMap, fragmentTexCoord).xyz * 2.0 - 1.0 );
	normal.y = -normal.y;	// Left handed to right handed space (Most normal maps are generated for DirectX)
	eye = normalize(textureEye);

	diffuseIntensity = clamp(max(dot(normal, light), 0.0), 0.0, 1.0);
	
	reflection = normalize(reflect(-light, normal));
	specularItensity = pow(clamp(max(dot(reflection, eye), 0.0), 0.0, 1.0), 0.0 );
	
	fragColor = vec4(0.0, 0.0, 0.0, 1.0) + vec4(0.0, 0.0, 0.0, 1.0) + texture(firstTexture, fragmentTexCoord)*diffuseIntensity + vec4(0.0, 0.0, 0.0, 1.0)*specularItensity;
}
