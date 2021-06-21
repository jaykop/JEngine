#version 450 core

layout (location = 0) out vec4 v4_fragColor;

////////////////////////////
// in variables
////////////////////////////
in vec2 v2_outTexCoord;
in vec4 v4_outColor;

////////////////////////////
// uniform variables
////////////////////////////
uniform sampler2D Texture;

////////////////////////////
// entry point
////////////////////////////
void main() {

	vec4 finalTexture = texture(Texture, v2_outTexCoord ) * v4_outColor;
	if (finalTexture.a <= 0.0)
		discard;

	v4_fragColor = finalTexture;

}