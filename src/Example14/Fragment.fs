#version 410 core

const float Eta = 0.15; // Water

uniform mat4 modelViewMatrix;
uniform mat4 inverseCameraMatrix;

uniform samplerCube cubemap;
uniform sampler2D waterTexture; 

in vec4 vertexFragment;

in vec3 binormal;
in vec3 tangent;
in vec3 normal;

in vec2 fragmentTexCoord;

layout(location = 0, index = 0) out vec4 fragColor;

vec3 textureToNormal(vec4 orgNormalColor)
{
	return normalize(vec3(clamp(orgNormalColor.r*2.0 - 1.0, -1.0, 1.0), clamp(orgNormalColor.g*2.0 - 1.0, -1.0, 1.0), clamp(orgNormalColor.b*2.0 - 1.0, -1.0, 1.0)));
}

void main(void)
{
	vec3 fragmentReflection;
	vec3 fragmentRefraction;
	float fresnel;

	vec3 textureNormal = textureToNormal(texture(waterTexture, fragmentTexCoord));

	mat3 matrix = mat3(binormal, normal, tangent);

	vec3 finalNormal = matrix * textureNormal;

	vec3 normalModelView = (modelViewMatrix*vec4(finalNormal, 0.0)).xyz;
	
	vec3 eyeModelView = normalize((modelViewMatrix*vertexFragment).xyz);

	fragmentReflection = (inverseCameraMatrix*vec4(reflect(eyeModelView, normalModelView), 0.0)).xyz;
	
	fragmentRefraction = (inverseCameraMatrix*vec4(refract(eyeModelView, normalModelView, Eta), 0.0)).xyz;
	
	fresnel = Eta + (1.0 - Eta) * pow(max(0.0, 1.0 - dot(-eyeModelView, normalModelView)), 5.0);

	vec4 reflectionColor = texture(cubemap, fragmentReflection);

	vec4 refractionColor = texture(cubemap, fragmentRefraction);
				
	fragColor = mix(refractionColor, reflectionColor, fresnel);
}
