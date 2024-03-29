#version 450 core

layout (location = 0) out vec4 v4_fragColor;

////////////////////////////
// const variables
////////////////////////////
const int MAX_LIGHTS 		= 16;
const int POINTLIGHT		= 1;
const int DIRECTIONALLIGHT	= 2;
const int SPOTLIGHT			= 3;

const int NONE	 	= 0;
const int POSITION	= 1;
const int NORMAL	= 2;
const int DIFFUSE	= 3;

const float ns = 25;

////////////////////////////
// structs
////////////////////////////
// Per-light values
struct Light{
	int mode;
	bool activate;
	vec3 position;
	vec3 aColor, dColor, sColor;
	float aIntensity, dIntensity, sIntensity;
	float innerAngle, outerAngle, fallOff;
	float constant, linear, quadratic;
	float radius;
};

////////////////////////////
// uniform variables
////////////////////////////

uniform bool boolean_lighten;
uniform vec4 v4_color;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D Texture;
uniform sampler2D gFog;
uniform sampler2D kAmb;

uniform vec3 fogColor;
uniform vec3 kAmbient;

uniform Light light[MAX_LIGHTS];
uniform uint lightSize; 
uniform int targetType;

uniform float zFar, zNear;
uniform vec3 v3_cameraPosition;

////////////////////////////
// in variables
////////////////////////////
in vec2 v2_outTexCoord;
in vec3 v3_outNormal;
in vec3 v3_outFragmentPosition;

////////////////////////////
// function declarations
////////////////////////////
vec3 GetDirLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal);
vec3 GetPointLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal, float distance);
vec3 GetSpotLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal, float distance);
void LightingEffect(vec3 finalTexture);

////////////////////////////
// entry point
////////////////////////////
void main()
{      
	vec4 finalTexture = vec4(0,0,0,0);

	finalTexture = texture(Texture, v2_outTexCoord) * v4_color;
	if (finalTexture.a <= 0.0)
		discard;

	if (boolean_lighten)
		LightingEffect(finalTexture.xyz);
	
	else
		v4_fragColor = finalTexture * v4_color;
}

////////////////////////////
// function definitions
////////////////////////////
vec3 GetDirLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal)
{
	vec3 lightDir = normalize(light.position - fragPos);
	// Calculate ambient  
	vec3 ambient = light.aIntensity * light.aColor * finalTexture;
	// Calculate diffuse
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.dColor * light.dIntensity * finalTexture;
	// Calculate specular
	vec3 reflectDir = 2*(dot(normal, lightDir)) * normal - lightDir;
	vec3 specular = light.sIntensity
		* pow(max(dot(reflectDir, viewDir), 0.0), ns)
		* light.sColor* finalTexture;
	
	return 	(ambient + specular + diffuse);
}

vec3 GetPointLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal, float distance)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float attenuation = min(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1);			
			
	// Calculate ambient  
	vec3 ambient = light.aIntensity * light.aColor * finalTexture;
	// Calculate diffuse
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.dColor * light.dIntensity * finalTexture;
	// Calculate specular
	vec3 reflectDir = 2*(dot(normal, lightDir)) * normal - lightDir;
	vec3 specular = light.sIntensity
		* pow(max(dot(reflectDir, viewDir), 0.0), ns)
		* light.sColor* finalTexture;
	
	return 	attenuation * (ambient + specular + diffuse);
}

vec3 GetSpotLight(Light light, vec3 fragPos, vec3 viewDir, 
	vec3 finalTexture, vec3 normal, float distance)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float attenuation = min(1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance)), 1);			
	float alpha = dot(normalize(lightDir), lightDir);
	float spotlightEffect = clamp(
		(alpha - cos(light.outerAngle))
		/ (cos(light.innerAngle) - cos(light.outerAngle))
		, 0.0, 1.0);
	spotlightEffect = pow(spotlightEffect, light.fallOff);
	// Calculate ambient  
	vec3 ambient = light.aIntensity * light.aColor * finalTexture;
	// Calculate diffuse
	vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.dColor * light.dIntensity * finalTexture;
	// Calculate specular
	vec3 reflectDir = 2*(dot(normal, lightDir)) * normal - lightDir;
	vec3 specular = light.sIntensity
		* pow(max(dot(reflectDir, viewDir), 0.0), ns)
		* light.sColor* finalTexture;
	
	return 	spotlightEffect * (ambient + attenuation * (specular + diffuse));
}

void LightingEffect(vec3 finalTexture)
{
	vec3 FragPos = texture(gPosition, v2_outTexCoord).rgb;
	vec3 Normal = texture(gNormal, v2_outTexCoord).rgb;
	
	// Send it to fragment shader by the toggle
	switch (targetType)
	{
	case POSITION:
	v4_fragColor = vec4(FragPos, 1.0);
	break;
	
	case NORMAL:
	v4_fragColor = vec4(Normal, 1.0);
	break;

	case DIFFUSE:
	v4_fragColor = vec4(finalTexture, 1.0);
	break;		
	
	default:
	case NONE:
			
	//vec3 sFog = texture(gFog, v2_outTexCoord).rgb;
	//vec3 kAmbi = texture(kAmb, v2_outTexCoord).rgb;
	vec3 sFog = texture(gFog, v2_outTexCoord).rgb;
	vec3 kAmbi = texture(kAmb, v2_outTexCoord).rgb;
		
	FragPos = v3_outFragmentPosition;
	Normal = v3_outNormal;
	sFog = fogColor;
	kAmbi = kAmbient;

	vec3 local = vec3(0,0,0);
	vec3 project = vec3(0,0,0);
	vec3 viewDir = normalize(v3_cameraPosition - FragPos);
	
	for (uint index = 0; index < lightSize; ++index) {
	
		if (light[index].activate) {
		
			float distance = length(light[index].position - FragPos);

			switch (light[index].mode)
			{
			case POINTLIGHT:
			//if (distance < light[index].radius)
				project = GetPointLight(light[index], FragPos, viewDir, 
				finalTexture, Normal, distance);
			break;
			
			case DIRECTIONALLIGHT:
			project = GetDirLight(light[index], FragPos, viewDir, 
			finalTexture, Normal);
			break;
			
			case SPOTLIGHT:
			//if (distance < light[index].radius)
				project = GetSpotLight(light[index], FragPos, viewDir, 
				finalTexture, Normal, distance);
			break;
			}

			// Add all those three and multiply by color of object
			local += 0.1 * kAmbi + project;
		}
	}
	
	float distanceToCamera = length(v3_cameraPosition - FragPos);
	float S = (zFar - distanceToCamera) / (zFar - zNear);
	local *= v4_color.xyz;
	v4_fragColor = vec4(S*local + (1-S)*sFog, v4_color.w);
		
	break;
	}	
}