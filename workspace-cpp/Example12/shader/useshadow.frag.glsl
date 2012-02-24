#version 150

uniform vec4 u_shapeColor;
uniform vec3 u_lightDirection;
uniform sampler2DShadow u_shadowTexture;

in vec3 v_normal;
in vec3 v_eye;
in vec4 v_projCoord;

layout(location = 0, index = 0) out vec4 fragColor;

void main(void)
{
	vec4 color;

	// Calculate z in NDC with a liitle offset to avoid artifacts.
	float zNDC = v_projCoord.z / v_projCoord.w - 0.001;
	
	// Compare fragment zNDC value against the z value in the shadow texture. If zNDC value is greater (NDC range is from -1 to +1, where -1 is closest), the fragment is shadowed.
	if (textureProj(u_shadowTexture, v_projCoord) < zNDC)
	{
		// Shadow, so darken the shape color to simulate shadow.
		color = u_shapeColor*0.5;
	}
	else
	{
		// No shadow, so calculate color.
		vec3 normal = normalize(v_normal);	

		float diffuseIntensity = max(dot(normal, u_lightDirection), 0.0);

		color = u_shapeColor*0.3;

		if (diffuseIntensity > 0.0)
		{
			color += u_shapeColor*diffuseIntensity;
		
			vec3 reflection = reflect(-u_lightDirection, normal);

			vec3 eye = normalize(v_eye);
	
			float specularItensity = pow(max(dot(reflection, eye), 0.0), 20.0);
			
			color += vec4(1.0, 1.0, 1.0, 1.0)*specularItensity;
		}
	}

	fragColor = color;
}
