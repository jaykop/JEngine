#version 450 core

uniform vec4 v4_color;
out	vec4 v4_fragColor;

void main()
{
	if (v4_color.a <= 0.0)
		discard;

	v4_fragColor = v4_color;
}