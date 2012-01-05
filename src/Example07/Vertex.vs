#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 inverseCameraMatrix;

in vec4 vertex;
in vec3 normal;

out vec3 fragmentNormal;
out vec3 fragmentReflection;

void main(void)
{
	fragmentNormal = (modelViewMatrix*vec4(normal, 0.0)).xyz;
	
	vec3 eyeModelView = normalize((modelViewMatrix*vertex).xyz);
		
	fragmentReflection = (inverseCameraMatrix*vec4(reflect(eyeModelView, fragmentNormal), 0.0)).xyz;
	
	gl_Position = projectionMatrix*modelViewMatrix*vertex;
}
