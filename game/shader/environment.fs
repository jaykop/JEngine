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
vec2 GetEnvUV(vec3 _entity);

uniform vec3 p_max;
uniform vec3 p_min;
vec3 p_max_c;
vec3 p_min_c;
int textureIndex = 0;

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
	
	vec3 entity = 2.f * Normal * dot_NL - View;
	v4_fragColor.rgb = vec3(texture(renderSampler[textureIndex], GetEnvUV(entity)));
	v4_fragColor.w = 1.f;
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

vec3 CentroidCalculation(vec3 _fragPos)
{
  vec3 c = (p_min + p_max) / 2.f;
  p_min_c = p_min - c;
  p_max_c = -p_min_c;

  return _fragPos - c;
}

vec2 GetEnvUV(vec3 _entity)
{
  vec3 p_c = CentroidCalculation(_entity);
  vec3 absVec = abs(p_c);
  vec2 uv = vec2(0.f);

  if (absVec.x >= absVec.y && absVec.x >= absVec.z)
  {
    //              +X  right             -X left
    if(p_c.x > 0.f)
    {
      uv.x = -p_c.z;
      uv.y = p_c.y;
      textureIndex = 1;
    }
    else
    {
      uv.x = p_c.z;
      uv.y = p_c.y;
      textureIndex = 0;
    }

    uv /= absVec.x;
  }
  else if (absVec.y >= absVec.x && absVec.y >= absVec.z)
  {
    //              +Y  top             -Y bottom
    if(p_c.y > 0.f)
    {
      uv.x = p_c.x;
      uv.y = -p_c.z;
      textureIndex = 3;
    }
    else
    {
      uv.x = p_c.x;
      uv.y = p_c.z;
      textureIndex = 2;
    }

    uv.x *= -1.f; // flipping
    uv.y *= -1.f; // flipping

    uv /= absVec.y;
  }
  else if (absVec.z >= absVec.x && absVec.z >= absVec.y)
  {
    //              +Z front              -Z back
    if(p_c.z > 0.f)
    {
      uv.x = p_c.x;
      uv.y = p_c.y;
      textureIndex = 5;
    }
    else
    {
      uv.x = -p_c.x;
      uv.y = p_c.y;
      textureIndex = 4;
    }

    uv /= absVec.z;
  }

  uv.x *= -1.f; // flipping
  uv = (uv + vec2(1.f)) * 0.5f;

  return uv;
}