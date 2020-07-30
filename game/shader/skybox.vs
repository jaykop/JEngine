#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform float f_scale;

out vec3 v3_fragPosition;
	
void main()
{
	v3_fragPosition = position;
	vec4 v4_position = vec4(position, 1.0);
	mat4 scaled = f_scale * m4_viewport * m4_projection;

	scaled[3][3] /= f_scale;
    vec4 pos = v4_position * scaled;
    gl_Position = pos;
}
