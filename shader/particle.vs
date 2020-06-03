#version 450 core

// Input vertex data, different for all executions of this shader.
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

// uniform matrixs
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform vec4 v4_color;

uniform bool boolean_hide;
uniform bool boolean_bilboard;

out	vec2 	v2_outTexCoord;
out vec4 	v4_outColor;

void main(){

	if (!boolean_hide) {

		mat4 model = m4_scale * m4_rotate * m4_translate;
		mat4 modelview = transpose(m4_viewport) * transpose(model);
	
		if (boolean_bilboard) {
			modelview[0][0] 
			= modelview[1][1] 
			= modelview[2][2] = 1;
		
			modelview[0][1]
			= modelview[0][2]
			= modelview[1][0]
			= modelview[1][2]
			= modelview[2][0]
			= modelview[2][1] = 0;
		}
	
		mat4 mvp = transpose(m4_projection) * modelview;

		gl_Position = mvp *  vec4(position, 1); 
		v2_outTexCoord = uvPosition;
		v4_outColor = v4_color;
	}

	else {
		gl_Position = vec4(0,0,0,0); 
		v2_outTexCoord = vec2(0,0);
		v4_outColor = vec4(0,0,0,0);	
	}
}