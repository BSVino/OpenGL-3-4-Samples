#version 150

layout(triangles) in;

layout(triangle_strip, max_vertices = 6) out;

uniform mat4 projectionMatrix;
uniform mat4 modelViewMatrix;

in vec3 geometryNormal[3];

out vec3 fragmentNormal;

out vec3 fragmentEye;

out vec4 fragmentColor;

out vec3 fragmentLight;

void main(void)
{
    int i;

	mat4 moveMatrix;

	moveMatrix[0] = vec4(1.0, 0.0, 0.0, 0.0);
	moveMatrix[1] = vec4(0.0, 1.0, 0.0, 0.0);
	moveMatrix[2] = vec4(0.0, 0.0, 1.0, 0.0);
	moveMatrix[3] = vec4(0.0, 0.0, 0.0, 1.0);

	moveMatrix[3][0] = -1.5;
	
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = projectionMatrix*modelViewMatrix*moveMatrix*gl_in[i].gl_Position;

		fragmentNormal = geometryNormal[i];
		
		fragmentEye = -(modelViewMatrix*moveMatrix*gl_in[i].gl_Position).xyz;
		
		fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);

		fragmentLight = vec3(0.0, 0.0, 3.0) - (moveMatrix*gl_in[i].gl_Position).xyz;

        EmitVertex();
    }
    
    EndPrimitive();    

	moveMatrix[3][0] = 1.5;
	
    for (i=0; i < gl_in.length(); i++)
    {
        gl_Position = projectionMatrix*modelViewMatrix*moveMatrix*gl_in[i].gl_Position;

		fragmentNormal = geometryNormal[i];

		fragmentEye = -(modelViewMatrix*moveMatrix*gl_in[i].gl_Position).xyz;

		fragmentColor = vec4(0.0, 0.0, 1.0, 1.0);

		fragmentLight = vec3(0.0, 0.0, 3.0) - (moveMatrix*gl_in[i].gl_Position).xyz;

        EmitVertex();
    }
    
    EndPrimitive();    
}
