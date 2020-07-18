#version 450 core

layout (location = 0) out vec4 fragColor;

smooth in vec2 v2_outTexCoord;

uniform vec3 v3_color;
uniform sampler2D Texture;

void main()
{
    if(fract(v2_outTexCoord.x / 0.001f) < 0.1f || fract(v2_outTexCoord.y / 0.001f) < 0.1f)
        fragColor = vec4(v3_color, 1);
    else
        fragColor = vec4(0);

	//vec2 coord = v2_outTexCoord * 10;
	//vec4 finalTexture = texture(Texture, coord) * vec4(v3_color, 1);
	//fragColor = finalTexture;
}
