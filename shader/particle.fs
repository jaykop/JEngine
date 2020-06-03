#version 450 core

// Interpolated values from the vertex shaders
in vec2 v2_outTexCoord;
in vec4 v4_outColor;

// Ouput data
out vec4 color;

uniform sampler2D 	Texture;

void main(){

	vec4 finalTexture = texture( Texture, v2_outTexCoord ) * v4_outColor;
	if (finalTexture.a <= 0.0)
		discard;

	color = finalTexture;

}