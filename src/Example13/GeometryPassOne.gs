#version 410 core

layout(points, invocations = 1) in;
layout(points, max_vertices = 64) out;

uniform float halfDetailStep;

uniform uint firstPassDetailLevel;

uniform vec4 positionTexture;

uniform float fovRadius;

uniform vec3 leftNormalTexture;
uniform vec3 rightNormalTexture;
uniform vec3 backNormalTexture;

out vec2 vertexTransform;

bool isOutside(vec4 point, vec4 viewPoint, float step)
{
	float bias = 0.1;

	vec3 positionToCenter = (point - viewPoint).xyz;

	float boundingRadius = sqrt(step*step+step*step);

	if (length(positionToCenter) - boundingRadius > fovRadius)
	{
		return true;
	}

	if (dot(positionToCenter, backNormalTexture) > boundingRadius + bias)
	{
		return true;
	}

	if (dot(positionToCenter, leftNormalTexture) > boundingRadius + bias)
	{
		return true;
	}

	if (dot(positionToCenter, rightNormalTexture) > boundingRadius + bias)
	{
		return true;
	}
	
	return false;
}

void main(void)
{
	if (isOutside(gl_in[0].gl_Position, positionTexture, halfDetailStep))	
	{
		return;
	}

	uint steps = uint(pow(2, firstPassDetailLevel));

	float finalDetailStep = halfDetailStep * 2.0 / steps;

	for (uint z = 0; z < steps; z++)
	{
		for (uint x = 0; x < steps; x++)
		{	
			if (isOutside(vec4(gl_in[0].gl_Position.x + x * finalDetailStep - halfDetailStep + finalDetailStep / 2.0, 0.0, gl_in[0].gl_Position.z + z * finalDetailStep - halfDetailStep + finalDetailStep / 2.0, 1.0), positionTexture, finalDetailStep / 2.0))
			{
				continue;
			}
		
			vertexTransform = vec2(gl_in[0].gl_Position.x + x * finalDetailStep - halfDetailStep, gl_in[0].gl_Position.z + z * finalDetailStep - halfDetailStep);
			EmitVertex();
			EndPrimitive();

			vertexTransform = vec2(gl_in[0].gl_Position.x + (x + 1.0) * finalDetailStep - halfDetailStep, gl_in[0].gl_Position.z + z * finalDetailStep - halfDetailStep);
			EmitVertex();
			EndPrimitive();

			vertexTransform = vec2(gl_in[0].gl_Position.x + (x + 1.0) * finalDetailStep - halfDetailStep, gl_in[0].gl_Position.z + (z + 1.0) * finalDetailStep - halfDetailStep);
			EmitVertex();
			EndPrimitive();

			vertexTransform = vec2(gl_in[0].gl_Position.x + x * finalDetailStep - halfDetailStep, gl_in[0].gl_Position.z + (z + 1.0) * finalDetailStep - halfDetailStep);
			EmitVertex();
			EndPrimitive();
		}
	}
}
