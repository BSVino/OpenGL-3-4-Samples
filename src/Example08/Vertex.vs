#version 150

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

uniform sampler2D readTexture;

in vec2 vertex;

out float alpha;

void main(void)
{
	vec4 particle = vec4(texture(readTexture, vertex).xyz, 1.0);

	alpha = clamp(1.0 - texture(readTexture, vertex).w, 0.0, 1.0);

	gl_PointSize = 32.0;
	
	gl_Position = projectionMatrix*modelViewMatrix*particle;
}
