#version 410 core

#define NUMBERWAVES 4

const float PI = 3.141592654;
const float G = 9.81;

uniform float waterPlaneLength;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;
uniform mat4 inverseCameraMatrix;
uniform float passedTime;

uniform vec4 waveParameters[NUMBERWAVES];
uniform vec2 waveDirections[NUMBERWAVES];

in vec4 vertex;

out vec4 vertexFragment;

out vec3 binormal;
out vec3 tangent;
out vec3 normal;

out vec2 fragmentTexCoord;

void main(void)
{
	vec4 finalVertex;

	finalVertex.x = vertex.x;
	finalVertex.y = vertex.y;
	finalVertex.z = vertex.z;
	finalVertex.w = 1.0;

	vec3 finalBinormal;
	vec3 finalTangent;
	vec3 finalNormal;

	finalBinormal.x = 0.0;
	finalBinormal.y = 0.0;
	finalBinormal.z = 0.0;

	finalTangent.x = 0.0;
	finalTangent.y = 0.0;
	finalTangent.z = 0.0;
	
	finalNormal.x = 0.0;
	finalNormal.y = 0.0;
	finalNormal.z = 0.0;

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency*dot(direction, vertex.xz)+phase*passedTime;
		
		finalVertex.x += steepness*amplitude*direction.x*cos(alpha);
		finalVertex.y += amplitude*sin(alpha);
		finalVertex.z += steepness*amplitude*direction.y*cos(alpha);
	}

	for (int i = 0; i < NUMBERWAVES; i++)
	{
		vec2 direction = normalize(waveDirections[i]);
		float speed = waveParameters[i].x;
		float amplitude = waveParameters[i].y;
		float wavelength = waveParameters[i].z;
		float steepness = waveParameters[i].w;

		float frequency = sqrt(G*2.0*PI/wavelength);
		float phase = speed*frequency;
		float alpha = frequency * dot(direction, finalVertex.xz) + phase*passedTime;
				
		finalBinormal.x += steepness * direction.x*direction.x * wavelength * amplitude * sin(alpha);
		finalBinormal.y += direction.x * wavelength * amplitude * cos(alpha);	
		finalBinormal.z += steepness * direction.x*direction.y * wavelength * amplitude * sin(alpha);	

		finalTangent.x += steepness * direction.x*direction.y * wavelength * amplitude * sin(alpha);
		finalTangent.y += direction.y * wavelength * amplitude * cos(alpha);	
		finalTangent.z += steepness * direction.y*direction.y * wavelength * amplitude * sin(alpha);	
	
		finalNormal.x += direction.x * wavelength * amplitude * cos(alpha);
		finalNormal.y += steepness * wavelength * amplitude * sin(alpha);
		finalNormal.z += direction.y * wavelength * amplitude * cos(alpha);
	}

	finalBinormal.x = 1.0 - finalBinormal.x;
	finalBinormal.z = -finalBinormal.z;
	finalBinormal = normalize(finalBinormal);

	finalTangent.x = -finalTangent.x;
	finalTangent.z = 1.0 - finalTangent.z;
	finalTangent = normalize(finalTangent);

	finalNormal.x = -finalNormal.x;
	finalNormal.y = 1.0 - finalNormal.y;
	finalNormal.z = -finalNormal.z;
	finalNormal = normalize(finalNormal);
	
	binormal = finalBinormal;
	tangent = finalTangent;
	normal = finalNormal;
		
	vertexFragment = finalVertex;
		
	fragmentTexCoord = vec2(clamp((finalVertex.x+64.0-0.5)/128.0, 0.0, 1.0), clamp(-(finalVertex.z-64.0+0.5)/128.0, 0.0, 1.0));
		
	gl_Position = projectionMatrix*modelViewMatrix*finalVertex;
}
