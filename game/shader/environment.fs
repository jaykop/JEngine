#version 450 core

const float EtaR = 0.985;
const float EtaG = 1.015;
const float EtaB = 1.045;
const float FresnelPower = 5.0;

layout (location = 0) out vec4 v4_fragColor;

uniform bool boolean_wavedLength;
uniform bool boolean_reflected;
uniform bool boolean_refracted;
uniform vec3 v3_cameraPosition;
uniform vec4 v4_color;

uniform sampler2D renderSampler[6];

////////////////////////////
// in variables
////////////////////////////
in vec3 v3_outNormal;
in vec3 v3_outFragmentPosition;

vec3 environmentMapping(vec3 vInput);

////////////////////////////
// entry point
////////////////////////////
void main()
{      
	// uniforms
	float tightness = 1.f;
	float material = 1.;

	vec3 finTex= vec3(0,0,0);
	
	vec3 Normal = normalize(v3_outNormal);
	vec3 View = normalize(v3_cameraPosition - v3_outFragmentPosition);
	float dot_NL = dot(Normal, View);
	
	float K = 1.0 / material;
	float K_R = EtaR / material;
	float K_G = EtaG / material;
	float K_B = EtaB / material;
	float F = ((1-K_G) * (1-K_G)) / ((1+K_G) * (1+K_G));
	float ratio = F + (1.0 - F) * pow((1.0 - dot(View, Normal)), FresnelPower);
	
	float I_Refract = 0;
	float lightIntensity = 0.3;
		
	if (boolean_reflected)
	{
		vec3 Reflect = 2*dot_NL*Normal - View;
		finTex = environmentMapping(Reflect);
	}	
	
	else if (boolean_refracted) 
	{
		vec3 refract;
		
		//if (waveLength) {
		
		//	vec3 Refract_R = (K_R*dot_NL - sqrt(1 - K_R*K_R*(1-dot_NL*dot_NL)))*Normal-K_R*View;
		//	vec3 Refract_G = (K_G*dot_NL - sqrt(1 - K_G*K_G*(1-dot_NL*dot_NL)))*Normal-K_G*View;
		//	vec3 Refract_B = (K_B*dot_NL - sqrt(1 - K_B*K_B*(1-dot_NL*dot_NL)))*Normal-K_B*View;
		
		//	refract.r = environmentMapping(Refract_R).r;
		//	refract.g = environmentMapping(Refract_G).g;
			//refract.b = environmentMapping(Refract_B).b;
		//}	

		//else {

			vec3 Refract = (K*dot_NL - sqrt(1 - K*K*(1-dot_NL*dot_NL)))*Normal-K*View;
			refract = environmentMapping(Refract);
		//}	
		
		I_Refract = K * pow(max(dot(View, refract), 0), tightness);
		
		finTex = refract;
	}
	else
	{
		// this would not work yet
		vec3 Reflect = 2*dot_NL*Normal - View;
			
		vec3 refract;
		
		//if (waveLength) {
		
		//	vec3 Refract_R = (K_R*dot_NL - sqrt(1 - K_R*K_R*(1-dot_NL*dot_NL)))*Normal-K_R*View;
		//	vec3 Refract_G = (K_G*dot_NL - sqrt(1 - K_G*K_G*(1-dot_NL*dot_NL)))*Normal-K_G*View;
		//	vec3 Refract_B = (K_B*dot_NL - sqrt(1 - K_B*K_B*(1-dot_NL*dot_NL)))*Normal-K_B*View;
		
		//	refract.r = environmentMapping(Refract_R).r;
		//	refract.g = environmentMapping(Refract_G).g;
		//	refract.b = environmentMapping(Refract_B).b;
		//}	

		//else {

			vec3 Refract = (K*dot_NL - sqrt(1 - K*K*(1-dot_NL*dot_NL)))*Normal-K*View;
			refract = environmentMapping(Refract);
		//}	
		
		I_Refract = K * pow(max(dot(View, refract), 0), tightness);
		
		finTex = mix(refract, 
			environmentMapping(Reflect), 
			ratio);
		finTex = mix(finTex, v4_color.xyz, ratio);
	}
	
	//if (addPhong) {
		
	//	vec3 lightEffect = (GetDirLight(dirLight,
	//		View, 
	//		normalize((dirLight.position - fragPosition)), 
	//		finTex) + I_Refract* lightIntensity);
			
	//	finTex = mix(finTex, v4_color, ratio) + lightEffect;
	//}
	
	v4_fragColor.rgb = finTex;
	v4_fragColor.w = v4_color.w;
}

vec3 environmentMapping(vec3 vInput)
{
	vec3 mag = abs(vInput);
	float m = max(max(mag.x, mag.y), mag.z);
	
	int index;
	vec2 uv;
	
	if (m == mag.x) 
	{
		if (vInput.x >= 0)
		{
			index = 1;

			uv.x = 1-0.5f * (-vInput.z / m + 1.0f);
			uv.y = 0.5f * (vInput.y / m + 1.0f);
		}
		
		else
		{
			index = 2;
			
			uv.x = 1-0.5 * (vInput.z / m + 1.0);
			uv.y = 0.5 * (vInput.y / m + 1.0);
		}
	}

	else if (m == mag.y) 
	{
		if (vInput.y >= 0)
		{
			index = 3;
			
			uv.x = 0.5f * (-vInput.z / m + 1.0f);
			uv.y = 1-0.5f * (-vInput.x / m + 1.0f);
		}
		
		else
		{
			index = 4;
			
			uv.x = 1-0.5f * (-vInput.z / m + 1.f);
			uv.y = 1-0.5f * (-vInput.x / m + 1.f);
		}
	
	}

	else // if (m == mag.z) 
	{
		if (vInput.z >= 0)
		{
			index = 0;
			
			uv.x = 1-0.5f * (vInput.x / m + 1.0f);
			uv.y = 0.5f * (vInput.y / m + 1.0f);
		}
		
		else
		{
			index = 5;
			
			uv.x = 1-0.5f * (-vInput.x / mag.z + 1.0f);
			uv.y = 0.5f * (vInput.y / mag.z + 1.0f);
		}
	}	
	
	return vec3(texture(renderSampler[index], uv));
}