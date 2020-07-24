#version 450 core

uniform vec3 v3_color;
out	vec4 v4_fragColor;

void main()
{
	v4_fragColor = vec4(v3_color, 1.f);
}