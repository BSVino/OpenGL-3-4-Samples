#version 410 core

layout(location = 0) in vec2 vertex;

void main(void)
{
	gl_Position = vec4(vertex.x, 0.0, vertex.y, 1.0);
}
