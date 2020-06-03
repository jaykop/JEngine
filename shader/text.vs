#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

////////////////////////////
// uniform variables
////////////////////////////
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform bool boolean_bilboard;

////////////////////////////
// out variables
////////////////////////////
out	vec2 v2_outTexCoord;

////////////////////////////
// entry point
////////////////////////////
void main(){

	vec4 newPosition = vec4(position, 1);
	mat4 model =  m4_scale * m4_rotate * m4_translate;

	// Calculate mvp transform matrix
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
	gl_Position = mvp * newPosition;

	// Texture coordinate
	v2_outTexCoord = uvPosition;
}