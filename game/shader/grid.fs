#version 450 core

layout (location = 0) out vec4 fragColor;

smooth in vec2 v2_outTexCoord;
uniform vec3 v3_color;
uniform int divisions;
uniform float thickness;

void main()
{
    float f_divisions = float(divisions);

    float x = fract(v2_outTexCoord.x * divisions);
    x = min(x, 1.0 - x);
    float xdelta = fwidth(x);
    x = smoothstep(x - xdelta, x + xdelta, thickness);

    float y = fract(v2_outTexCoord.y * divisions);
    y = min(y, 1.0 - y);
    float ydelta = fwidth(y);
    y = smoothstep(y - ydelta, y + ydelta, thickness);

    float c = clamp(x + y, 0.0, 1.0);
	fragColor.xyz = vec3(c) * v3_color;
	fragColor.w = .5f;
	
}
