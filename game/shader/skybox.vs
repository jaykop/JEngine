#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 m4_viewport;
uniform mat4 m4_projection;

out vec3 v3_fragPosition;
	
void main()
{
    v3_fragPosition = position;
	vec4 v4_position = vec4(position, 1.0);
    vec4 pos = v4_position * m4_viewport * m4_projection ;
    gl_Position = pos;
}