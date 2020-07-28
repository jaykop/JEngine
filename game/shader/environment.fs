#version 450 core

const float EtaR = 0.985;
const float EtaG = 1.015;
const float EtaB = 1.045;
const float FresnelPower = 5.0;

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
	vec3 finTex= vec3(0,0,0);
	
	vec3 Normal = normalize(v3_outNormal);
	vec3 View = normalize(v3_cameraPosition - v3_outFragmentPosition);
	float dot_NL = dot(Normal, View);

	if (boolean_reflected)
	{
		vec3 Reflect = 2*dot_NL*Normal - View;
		finTex = texture(skybox, Reflect).rgb;
	}
	
	else if (boolean_refracted)
	{
		// switch these to uniform variables
		float tightness = 1.f;
		float material = 1.f;
		
		float K = 1.0 / material;
		float K_R = EtaR / material;
		float K_G = EtaG / material;
		float K_B = EtaB / material;
		float F = ((1-K_G) * (1-K_G)) / ((1+K_G) * (1+K_G));
		float ratio = F + (1.0 - F) * pow((1.0 - dot(View, Normal)), FresnelPower);
				
		vec3 Refract = (K*dot_NL - sqrt(1 - K*K*(1-dot_NL*dot_NL)))*Normal-K*View;
	
		float I_Refract = K * pow(max(dot(View, Refract), 0), tightness);
		finTex = texture(skybox, Refract).rgb;
	}
	
	v4_fragColor = vec4(finTex, 1.0);
}
