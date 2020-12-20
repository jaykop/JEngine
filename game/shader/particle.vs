#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uvPosition;

////////////////////////////
// uniform variables
////////////////////////////
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;

uniform bool boolean_hide;
uniform bool boolean_bilboard;

uniform bool hasParent;
uniform mat4 m4_parentTranslate,
	m4_parentScale, m4_parentRotate;

////////////////////////////
// out variables
////////////////////////////
out	vec2 v2_outTexCoord;

////////////////////////////
// fucntion declarations
////////////////////////////
void Transforming(vec4 _position, mat4 _model);

////////////////////////////
// entry point
////////////////////////////
void main(){

	if (boolean_hide) {
	
		gl_Position = vec4(0,0,0,0); 
	}

	else {
	
		vec4 newPosition = vec4(position, 1);
		mat4 model = m4_scale * m4_rotate * m4_translate;
		vec4 newTexCoord;

		Transforming(newPosition, model);
	}
	
	v2_outTexCoord = uvPosition;
}

////////////////////////////
// Fucntion bodies
////////////////////////////
void Transforming(vec4 _position, mat4 _model) {
	
	mat4 newModel = transpose(_model);
	if (hasParent) 
		newModel = transpose(m4_parentScale * m4_parentRotate * m4_parentTranslate) * newModel;

	// Calculate mvp transform matrix
	mat4 modelview = transpose(m4_viewport) * newModel;

	if (boolean_bilboard) {
		//modelview[0][0]
		//	= modelview[1][1]
		//	= modelview[2][2] = 1;

		modelview[0][1]
			= modelview[0][2]
			= modelview[1][0]
			= modelview[1][2]
			= modelview[2][0]
			= modelview[2][1] = 0;
	}

	mat4 mvp = transpose(m4_projection) * modelview;
	gl_Position = mvp * _position;
}