#version 450 core

out vec4 FragColor;

in vec3 v3_fragPosition;

uniform sampler2D sampler[6];
uniform vec3 v3_cameraPosition;
uniform vec3 v3_color;

vec3 PlanarUV(vec3 vInput, vec3 viewPos)
{
	vec3 mag = abs(vInput - viewPos);
	vec3 diff = vInput - viewPos;
	float m = max(max(mag.x,mag.y),mag.z);
	
	vec3 uv = vec3(0.5) + 0.5 * diff/m;
			
	if (m == mag.x) {
	
		vec2 leftUV = vec2(1-uv.z, 1-uv.y);
		vec2 rightUV = vec2(uv.z, 1-uv.y);
		
		if (diff.x > 0) 
			return vec3(texture(sampler[3], leftUV));
		
		else 
			return vec3(texture(sampler[2], rightUV));
	}
	
	else if (m == mag.y) {
		
		vec2 downUV = vec2(uv.x, uv.z);
		vec2 upUV = vec2(uv.x, 1-uv.z);
		
		if (diff.y > 0)
			return vec3(texture(sampler[5], downUV));
		
		else
			return vec3(texture(sampler[4], upUV));
	}
 
	else {
		
		vec2 frontUV = vec2(uv.x, 1-uv.y);
		vec2 backUV = vec2(1-uv.x, 1-uv.y);
		
		if (diff.z > 0)
			return vec3(texture(sampler[0], frontUV));
		
		else {
			return vec3(texture(sampler[1], backUV));
		}
	}
}

void main()
{    
	FragColor = vec4(PlanarUV(v3_fragPosition, v3_cameraPosition) * v3_color, 1.f);
}
