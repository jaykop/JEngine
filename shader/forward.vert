#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 v_color;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 color;

uniform bool uniformColor;

out vec3 outColor;

void main()
{
    gl_Position =  vec4(position, 1.0) * model * (view) * (projection);	
	
	if (uniformColor)
		outColor = color;
	else
		outColor = v_color;
}