#version 450 core

in vec3 v3_fragPosition;

out vec4 outColor;

uniform sampler2D sampler[6];
uniform vec3 v3_color, v3_viewPosition;

vec3 PlanarUV(vec3 vInput, vec3 viewPos)
{
	vec3 uv = vec3(0.5) + 0.5 * (vInput - viewPos);
	vec3 mag = abs(vInput - viewPos);
			
	if (mag.x > mag.y && mag.x > mag.z) {
	
		vec2 leftUV = vec2(1-uv.z, 1-uv.y);
		vec2 rightUV = vec2(uv.z, 1-uv.y);
		
		if (viewPos.x - vInput.x > 0) 
			return vec3(texture(sampler[3], leftUV));
		
		else 
			return vec3(texture(sampler[2], rightUV));
	}
	
	else if (mag.y > mag.z) {
		
		vec2 downUV = vec2(1-uv.z, uv.x);
		vec2 upUV = vec2(1-uv.z, 1-uv.x);
		
		if (viewPos.y - vInput.y > 0)
			return vec3(texture(sampler[5], downUV));
		
		else
			return vec3(texture(sampler[4], upUV));
	}
 
	else {
		
		vec2 frontUV = vec2(uv.x, 1-uv.y);
		vec2 backUV = vec2(1-uv.x, 1-uv.y);
		
		if (viewPos.z - vInput.z > 0)
			return vec3(texture(sampler[0], frontUV));
		
		else {
			return vec3(texture(sampler[1], backUV));
		}
	}
}

void main()
{      
	vec3 finTex = PlanarUV(v3_fragPosition, v3_viewPosition) * v3_color;
	outColor = vec4(finTex, 1.f);
}