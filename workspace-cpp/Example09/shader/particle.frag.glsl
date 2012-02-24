#version 150

uniform sampler2D u_texture; 

in float v_alpha;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	// Red fading out sprite
	fragColor = texture(u_texture, gl_PointCoord)*vec4(1.0, 0.0, 0.0, v_alpha);
}
