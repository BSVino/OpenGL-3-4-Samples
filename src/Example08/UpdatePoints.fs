#version 150

in vec4 newParticle;

layout(location = 0, index = 0) out vec4 fragParticle;

void main(void)
{
	fragParticle = newParticle;
}
