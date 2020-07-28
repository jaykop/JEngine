#version 450 core

layout (location = 0) in vec3 position;

uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;
uniform float f_scale;

out vec3 v3_fragPosition;
	
void main()
{
	mat4 scaledModel = f_scale * m4_scale;
	scaledModel[3][3] = 1.f;
	scaledModel = scaledModel * m4_rotate * m4_translate;
	
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport)* transpose(scaledModel);
	vec4 v4_position = vec4(position, 1);
	gl_Position = mvp * v4_position;
	
	mat4 model =  m4_scale * m4_rotate * m4_translate;
	v3_fragPosition = vec3(transpose(model) * v4_position);
    
	//v3_fragPosition = position;
	//vec4 v4_position = vec4(position, 1.0);
    //vec4 pos = v4_position * m4_viewport * m4_projection ;
    //gl_Position = pos;
}
