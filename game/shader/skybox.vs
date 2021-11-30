#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_translate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform float f_scale;

out vec3 v3_fragPosition;
	
void main()
{
	mat4 scaled =  m4_scale;
	scaled[3][3] /= f_scale;
	mat4 model = scaled * m4_rotate * m4_translate;
	gl_Position = transpose(m4_projection) * transpose(m4_viewport) * transpose(model) * vec4(position, 1.0);	
	v3_fragPosition = vec3(transpose(m4_scale* m4_rotate * m4_translate) *vec4(position, 1.0));
}
