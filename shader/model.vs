#version 450 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uvPosition;
layout(location = 2) in vec3 normal;

////////////////////////////
// const variables
////////////////////////////
const int MAX_ARRAY = 16;

////////////////////////////
// uniform variables
////////////////////////////
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform mat4 m4_aniScale;
uniform mat4 m4_aniTranslate;
uniform vec4 v4_lightColor[MAX_ARRAY];
uniform bool boolean_flip;
uniform bool boolean_light;
uniform bool boolean_bilboard;

uniform bool hasParent;
uniform mat4 m4_parentTranslate,
	m4_parentScale, m4_parentRotate;

////////////////////////////
// out variables
////////////////////////////
out	vec2 v2_outTexCoord;
out	vec3 v3_outNormal;
out	vec3 v3_outFragmentPosition;

////////////////////////////
// fucntion declarations
////////////////////////////
void Transforming(vec4 _position, mat4 _model);
void Mapping(vec4 _position, inout vec4 _texCoord);
void SendToFrag(vec4 _position, vec4 _texCoord, mat4 _model);

////////////////////////////
// entry point
////////////////////////////
void main() {

	vec4 newPosition = vec4(position, 1);
	mat4 model = m4_scale * m4_rotate * m4_translate;
	vec4 newTexCoord;

	Transforming(newPosition, model);
	Mapping(newPosition, newTexCoord);
	SendToFrag(newPosition, newTexCoord, model);

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
	gl_Position = mvp * _position;
}

void Mapping(vec4 _position, inout vec4 _texCoord) {

	// Animation mapping
	mat4 animation = m4_aniScale * m4_aniTranslate;
	_texCoord = transpose(animation) * vec4(uvPosition, 0, 1);

	// Check flipping
	if (boolean_flip)
		_texCoord.x = -_texCoord.x;

}

void SendToFrag(vec4 _position, vec4 _texCoord, mat4 _model) {

	// Texture coordinate
	v2_outTexCoord = _texCoord.xy;

	// Lighting attributes
	if (boolean_light) {

		// Lighing attributes
		v3_outFragmentPosition = vec3(transpose(_model) * _position);

		v3_outNormal = mat3(inverse(_model)) * normal;
	}
};