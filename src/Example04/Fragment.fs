#version 150

in vec3 fragmentNormal;
in vec3 fragmentEye;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	float diffuseIntensity;
	float specularItensity;

	vec3 light;

	vec3 normal;
	vec3 eye;
	
	vec3 reflection;

	light = normalize(vec3(5.0, 5.0, 10.0));

	normal = normalize(fragmentNormal);
	eye = normalize(fragmentEye);

	diffuseIntensity = clamp(max(dot(normal, light), 0.0), 0.0, 1.0);
	
	reflection = normalize(reflect(-light, normal));
	specularItensity = pow(clamp(max(dot(reflection, eye), 0.0), 0.0, 1.0), 20.0 );

	fragColor = vec4(0.0, 0.0, 0.0, 1.0) + vec4(0.1, 0.1, 0.1, 1.0) + vec4(1.0, 0.0, 0.0, 1.0)*diffuseIntensity + vec4(1.0, 0.9, 0.9, 1.0)*specularItensity;
}
