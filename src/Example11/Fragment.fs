#version 150

uniform vec4 shapeColor;
uniform vec3 lightDirection;

uniform sampler2DShadow shadowTexture;

in vec3 fragmentNormal;
in vec3 fragmentEye;
in vec3 fragmentLight;

in vec4 projCoord;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float diffuseIntensity;
	float specularItensity;

	vec3 light;

	vec3 normal;
	vec3 eye;
	
	vec3 reflection;

	light = normalize(lightDirection);

	normal = normalize(fragmentNormal);
	eye = normalize(fragmentEye);

	diffuseIntensity = clamp(max(dot(normal, light), 0.0), 0.0, 1.0);
	
	reflection = normalize(reflect(-light, normal));
	specularItensity = pow(clamp(max(dot(reflection, eye), 0.0), 0.0, 1.0), 20.0 );

	float rValue = 1.0;

	vec3 coordPos  = projCoord.xyz / projCoord.w;
	
	if(coordPos.x >= 0.0 && coordPos.y >= 0.0 && coordPos.x <= 1.0 && coordPos.y <= 1.0 )
   	{
		rValue = texture(shadowTexture, coordPos) < coordPos.z - 0.001 ? 0.5 : 1.0;
		
		if (rValue == 0.5)
		{
			specularItensity = 0.0;
		}
	}

	fragColor = vec4(0.0, 0.0, 0.0, 1.0) + vec4(0.1, 0.1, 0.1, 1.0) + shapeColor*diffuseIntensity*rValue + vec4(0.2, 0.2, 0.2, 1.0)*specularItensity;
}
