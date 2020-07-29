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
uniform vec3 v3_color;

uniform sampler2D renderSampler[6];
// uniform samplerCube skybox;

////////////////////////////
// in variables
////////////////////////////
in vec3 v3_outNormal;
in vec3 v3_outFragmentPosition;

vec3 ReflectPlanarUV(vec3 vInput);
vec3 RefractPlanarUV(vec3 vInput);

////////////////////////////
// entry point
////////////////////////////
void main()
{      
	// uniforms
	float tightness = 1.f;
	float material = 1.45;

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
		
	if (boolean_reflected) {
	
		vec3 Reflect = 2*dot_NL*Normal - View;
		finTex = ReflectPlanarUV(Reflect);
	}
	
	else if (boolean_refracted) {
		
		vec3 refract;
		
		if (boolean_wavedLength) {
		
			vec3 Refract_R = (K_R*dot_NL - sqrt(1 - K_R*K_R*(1-dot_NL*dot_NL)))*Normal-K_R*View;
			vec3 Refract_G = (K_G*dot_NL - sqrt(1 - K_G*K_G*(1-dot_NL*dot_NL)))*Normal-K_G*View;
			vec3 Refract_B = (K_B*dot_NL - sqrt(1 - K_B*K_B*(1-dot_NL*dot_NL)))*Normal-K_B*View;
		
			refract.r = RefractPlanarUV(Refract_R).r;
			refract.g = RefractPlanarUV(Refract_G).g;
			refract.b = RefractPlanarUV(Refract_B).b;
		}	

		else {

			vec3 Refract = (K*dot_NL - sqrt(1 - K*K*(1-dot_NL*dot_NL)))*Normal-K*View;
			refract = RefractPlanarUV(Refract);
		}	
		
		I_Refract = K * pow(max(dot(View, refract), 0), tightness);
		
		finTex = refract;
	}
	
	else {
		
		vec3 Reflect = 2*dot_NL*Normal - View;
		
		vec3 refract;
		
		if (boolean_wavedLength) {
		
			vec3 Refract_R = (K_R*dot_NL - sqrt(1 - K_R*K_R*(1-dot_NL*dot_NL)))*Normal-K_R*View;
			vec3 Refract_G = (K_G*dot_NL - sqrt(1 - K_G*K_G*(1-dot_NL*dot_NL)))*Normal-K_G*View;
			vec3 Refract_B = (K_B*dot_NL - sqrt(1 - K_B*K_B*(1-dot_NL*dot_NL)))*Normal-K_B*View;
		
			refract.r = RefractPlanarUV(Refract_R).r;
			refract.g = RefractPlanarUV(Refract_G).g;
			refract.b = RefractPlanarUV(Refract_B).b;
		}	

		else {

			vec3 Refract = (K*dot_NL - sqrt(1 - K*K*(1-dot_NL*dot_NL)))*Normal-K*View;
			refract = RefractPlanarUV(Refract);
		}	
		
		I_Refract = K * pow(max(dot(View, refract), 0), tightness);
		
		finTex = mix(refract, 
			ReflectPlanarUV(Reflect), 
			ratio);
		finTex = mix(finTex, v3_color, ratio);
	}
	
	v4_fragColor = vec4(finTex, 1.0);
}

vec3 RefractPlanarUV(vec3 vInput)
{
	vec3 uv = vInput;
	vec3 mag = abs(vInput);
	
	int isXPositive = vInput.x > 0 ? 1 : 0;
	int isYPositive = vInput.y > 0 ? 1 : 0;
	int isZPositive = vInput.z > 0 ? 1 : 0;
	
	float uc, vc;
	int index = -1;
	vec2 new_uv;
	
	if (isXPositive == 1 && mag.x >= mag.y && mag.x >= mag.z) {
		
		index = 5;
		
		uc = -uv.z;
		vc = uv.y;

		new_uv.x = 1-0.5f * (uc / mag.x + 1.0f);
		new_uv.y = 0.5f * (vc / mag.x + 1.0f);
	}
	
	else if (isXPositive == 0 && mag.x >= mag.y && mag.x >= mag.z) {

		index = 1;
		
		uc = uv.z;
		vc = uv.y;
		
		new_uv.x = 1-0.5 * (uc / mag.x + 1.0);
		new_uv.y = 0.5 * (vc / mag.x + 1.0);
	}
	
	else if (isYPositive == 1 && mag.y >= mag.x && mag.y >= mag.z) {
		
		index = 3;
		
		uc = -uv.z;
		vc = -uv.x;
		
		new_uv.x = 0.5f * (uc / mag.y + 1.0f);
		new_uv.y = 1-0.5f * (vc / mag.y + 1.0f);
	}
	
	else if (isYPositive == 0 && mag.y >= mag.x && mag.y >= mag.z) {
		
		index = 4;
		
		uc = -uv.z;
		vc = -uv.x;
		
		new_uv.x = 0.5f * (uc / mag.y + 1.0f);
		new_uv.y = 0.5f * (vc / mag.y + 1.0f);
	}
	
	else if (isZPositive == 1 && mag.z >= mag.y && mag.z >= mag.x) {
		
		index = 0;
		
		uc = uv.x;
		vc = uv.y;
		
		new_uv.x = 1-0.5f * (uc / mag.z + 1.0f);
		new_uv.y = 0.5f * (vc / mag.z + 1.0f);
	}
	
	else if (isZPositive == 0 && mag.z >= mag.y && mag.z >= mag.x) {
	
		index = 2;
		
		uc = -uv.x;
		vc = uv.y;
		
		new_uv.x = 1-0.5f * (uc / mag.z + 1.0f);
		new_uv.y = 0.5f * (vc / mag.z + 1.0f);
		
	}
	
	return vec3(texture(renderSampler[index], new_uv));
}

vec3 ReflectPlanarUV(vec3 vInput)
{
	vec3 uv = vInput;
	vec3 mag = abs(vInput);
	
	int isXPositive = vInput.x > 0 ? 1 : 0;
	int isYPositive = vInput.y > 0 ? 1 : 0;
	int isZPositive = vInput.z > 0 ? 1 : 0;
	
	float uc, vc;
	int index = -1;
	vec2 new_uv;
	
	if (isXPositive == 1 && mag.x >= mag.y && mag.x >= mag.z) {
		
		index = 5;
		
		uc = -uv.z;
		vc = uv.y;

		new_uv.x = 1-0.5f * (uc / mag.x + 1.0f);
		new_uv.y = 0.5f * (vc / mag.x + 1.0f);
	}
	
	else if (isXPositive == 0 && mag.x >= mag.y && mag.x >= mag.z) {

		index = 1;
		
		uc = uv.z;
		vc = uv.y;
		
		new_uv.x = 1-0.5 * (uc / mag.x + 1.0);
		new_uv.y = 0.5 * (vc / mag.x + 1.0);
	}
	
	else if (isYPositive == 1 && mag.y >= mag.x && mag.y >= mag.z) {
		
		index = 3;
		
		uc = -uv.z;
		vc = -uv.x;
		
		new_uv.x = 0.5f * (uc / mag.y + 1.0f);
		new_uv.y = 1-0.5f * (vc / mag.y + 1.0f);
	}
	
	else if (isYPositive == 0 && mag.y >= mag.x && mag.y >= mag.z) {
		
		index = 4;
		
		uc = -uv.z;
		vc = -uv.x;
		
		new_uv.x = 0.5f * (uc / mag.y + 1.0f);
		new_uv.y = 0.5f * (vc / mag.y + 1.0f);
	}
	
	else if (isZPositive == 1 && mag.z >= mag.y && mag.z >= mag.x) {
		
		index = 2;
		
		uc = uv.x;
		vc = uv.y;
		
		new_uv.x = 1-0.5f * (uc / mag.z + 1.0f);
		new_uv.y = 0.5f * (vc / mag.z + 1.0f);
	}
	
	else if (isZPositive == 0 && mag.z >= mag.y && mag.z >= mag.x) {
	
		index = 0;
		
		uc = -uv.x;
		vc = uv.y;
		
		new_uv.x = 1-0.5f * (uc / mag.z + 1.0f);
		new_uv.y = 0.5f * (vc / mag.z + 1.0f);
		
	}
	
	return vec3(texture(renderSampler[index], new_uv));
}
