#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform vec3 lightDir;

in vec4 vertex;
in vec3 normal;
in vec3 tangent;
in vec2 texCoord;

out vec2 fragmentTexCoord;

out vec3 textureLight;
out vec3 textureEye;

void main(void)
{
	fragmentTexCoord = texCoord;

	vec3 n = (modelViewMatrix*vec4(normal, 0.0)).xyz;
	vec3 t = (modelViewMatrix*vec4(tangent, 0.0)).xyz;
	vec3 b = cross(n, t);

	vec3 light = normalize(lightDir);

	textureLight.x = dot(light, t);
	textureLight.y = dot(light, b);
	textureLight.z = dot(light, n);
	
	vec3 eye = (modelViewMatrix*vertex).xyz;
	eye = normalize(-eye);
	
	textureEye.x = dot(eye, t);
	textureEye.y = dot(eye, b);
	textureEye.z = dot(eye, n);

	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
