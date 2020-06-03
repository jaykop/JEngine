#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

// uniform matrixs
uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform mat4 m4_rotateY;
uniform mat4 m4_rotateZ;

void main(){

	mat4 model =  m4_scale * (m4_rotateY * m4_rotateZ) * m4_translate;
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);

	gl_Position = mvp * vec4(position, 1);
}