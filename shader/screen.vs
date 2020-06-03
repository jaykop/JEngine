#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvPosition;
layout (location = 2) in vec3 normal;

out vec2 v2_outTexCoord;

void main(){

	vec4 newPosition = vec4(2 * position, 1);
	v2_outTexCoord = uvPosition;
	gl_Position = newPosition;

}