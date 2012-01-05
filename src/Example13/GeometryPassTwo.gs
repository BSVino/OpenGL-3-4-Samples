#version 410 core

uniform sampler2DRect heightMapTexture;
uniform sampler2DRect normalMapTexture;

uniform mat4 TMVP;

uniform vec4 positionTexture;

uniform vec3 leftNormalTexture;
uniform vec3 rightNormalTexture;
uniform vec3 backNormalTexture;

layout(triangles, invocations = 1) in;
layout(max_vertices = 3) out;

out vec2 fragmentTexCoord;

out vec3 fragmentNormal;

vec4 getCenter(in vec4 v0, in vec4 v1, in vec4 v2)
{
	vec4 middle = mix(v0, v1, 0.5);
	return mix(middle, v2, 0.5);
}

void main(void)
{
	ivec2 heightMapTextureSize = textureSize(heightMapTexture);

	vec4 heightMapPosition;
	
	vec3 tempNormal;
	
	for(int i = 0; i < gl_in.length(); ++i)
	{
		heightMapPosition = gl_in[i].gl_Position;
		
		heightMapPosition.y = texture(heightMapTexture, heightMapPosition.xz).r;
		
		tempNormal = texture(normalMapTexture, heightMapPosition.xz).xyz * 2.0 - 1.0;
		
		fragmentNormal.x = tempNormal.r;
		fragmentNormal.y = tempNormal.b;
		fragmentNormal.z = tempNormal.g;
		
		fragmentTexCoord = vec2((heightMapPosition.x - 0.5) / (heightMapTextureSize.s - 1.0), (heightMapPosition.z - 0.5) / (heightMapTextureSize.t - 1.0));
		
		gl_Position = TMVP*heightMapPosition;
		
		EmitVertex();
	}

	EndPrimitive();
}
