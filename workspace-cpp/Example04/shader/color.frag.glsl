#version 150

in vec4 v_color;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	fragColor = v_color;
}
