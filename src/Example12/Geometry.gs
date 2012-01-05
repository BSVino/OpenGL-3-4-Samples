#version 410 core

layout(triangles, invocations = 1) in;
layout(line_strip, max_vertices = 4) out;

void main(void)
{
	for(int i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
	}
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	EndPrimitive();
}
