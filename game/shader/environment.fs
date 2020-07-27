#version 450 core

layout (location = 0) out vec4 v4_fragColor;

uniform bool boolean_reflected;
uniform bool boolean_refracted;
uniform vec3 v3_cameraPosition;

uniform samplerCube skybox;

////////////////////////////
// in variables
////////////////////////////
in vec3 v3_outNormal;
in vec3 v3_outFragmentPosition;

////////////////////////////
// entry point
////////////////////////////
void main()
{      
	if (boolean_reflected)
	{
		vec3 I = normalize(v3_outFragmentPosition - v3_cameraPosition);
		vec3 R = reflect(I, normalize(v3_outNormal));
		v4_fragColor = vec4(texture(skybox, R).rgb, 1.0);
	}
	
	else if (boolean_refracted)
	{
		//vec3 I = normalize(v3_outFragmentPosition - v3_cameraPosition);
		//vec3 R = refract(I, normalize(v3_outNormal));
		v4_fragColor = vec4(1,1,1,1);//vec4(texture(skybox, R).rgb, 1.0);
	}
}
