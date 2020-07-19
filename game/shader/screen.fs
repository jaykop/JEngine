#version 450 core

layout (location = 0) out vec4 v4_fragColor;

const int JE_EFFECT_NONE		= 0;
const int JE_EFFECT_BLUR		= 1;
const int JE_EFFECT_SOBEL		= 2;
const int JE_EFFECT_INVERSE	= 3;

in vec2 v2_outTexCoord;

uniform int			enum_effectType;
uniform float		float_blurSize;
uniform float		float_blurAmount;
uniform float		float_sobelAmount;
uniform vec4 		v4_screenColor;
uniform sampler2D 	Framebuffer;

void VisualEffect(inout vec4 _color, inout vec2 _textureCoord);

void main()
{

	vec2 textureCoord = v2_outTexCoord;
	textureCoord.y = 1 - textureCoord.y;
	vec4 finalScreen = texture(Framebuffer, textureCoord) * v4_screenColor;

	// Impose visual effect here...
	if (enum_effectType != JE_EFFECT_NONE)
		VisualEffect(finalScreen, textureCoord);

	if (finalScreen.a <= 0.0)
				discard;

	v4_fragColor = finalScreen;
}

void VisualEffect(inout vec4 _color, inout vec2 _textureCoord){

	// Blur effect
	if (enum_effectType == JE_EFFECT_BLUR) {
	
		int x_range = int(float_blurSize / 2.0);
		int y_range = int(float_blurSize / 2.0);
	
		vec4 sum = vec4(0,0,0,0);
		for (int x = -x_range ; x <= x_range ; x++)
			for(int y = -y_range ; y <= y_range ; y++){
				sum += texture(Framebuffer, 
					vec2(_textureCoord.x + x * (1 / float_blurAmount), 
						_textureCoord.y + y * (1 / float_blurAmount))) 
						/ (float_blurSize * float_blurSize);
			}
		
		_color = sum;
	}

	// Sobel effect
	else if (enum_effectType == JE_EFFECT_SOBEL){
		vec4 top         = texture(Framebuffer, vec2(_textureCoord.x, _textureCoord.y + float_sobelAmount));
		vec4 bottom      = texture(Framebuffer, vec2(_textureCoord.x, _textureCoord.y - float_sobelAmount));
		vec4 left        = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y));
		vec4 right       = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y));
		vec4 topLeft     = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y + float_sobelAmount));
		vec4 topRight    = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y + float_sobelAmount));
		vec4 bottomLeft  = texture(Framebuffer, vec2(_textureCoord.x - float_sobelAmount, _textureCoord.y - float_sobelAmount));
		vec4 bottomRight = texture(Framebuffer, vec2(_textureCoord.x + float_sobelAmount, _textureCoord.y - float_sobelAmount));
		vec4 sx = -topLeft - 2 * left - bottomLeft + topRight   + 2 * right  + bottomRight;
		vec4 sy = -topLeft - 2 * top  - topRight   + bottomLeft + 2 * bottom + bottomRight;
		_color = sqrt(sx * sx + sy * sy);
	}

	// Inverse effect
	else if (enum_effectType == JE_EFFECT_INVERSE)
	{
		vec4 inversed = vec4(1,1,1,1) - _color;
		inversed.w = 1.f;
	
		_color = inversed;
	}
}