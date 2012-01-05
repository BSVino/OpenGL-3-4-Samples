#version 150

uniform sampler2D readTexture;

uniform float size;

uniform float time;

in vec2 vertex;

out vec4 newParticle;

float rand(vec2 seed)
{
	return fract(sin(dot(seed.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void)
{
	newParticle = texture(readTexture, vertex);

	newParticle.x += (rand(vertex*123.456)*2.0-1.0)*0.1*time;
	newParticle.y += (rand(vertex*234.567)*2.0-1.0)*0.1*time;
	newParticle.z += (rand(vertex*345.678)*2.0-1.0)*0.1*time;
	newParticle.w += 0.02*time;

	if (newParticle.w > 1.0)
	{
		newParticle.x = 0.0;
		newParticle.y = 0.0;
		newParticle.z = 0.0;
		newParticle.w = rand(vertex*456.789);
	}
	
	gl_PointSize = 1.0;
	
	gl_Position = vec4(vertex*2.0-vec2(1.0-1.0/size, 1.0-1.0/size), 0.0, 1.0);
}
