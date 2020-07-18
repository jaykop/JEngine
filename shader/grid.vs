#version 450 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 uvPosition;

uniform mat4 m4_translate;
uniform mat4 m4_scale;
uniform mat4 m4_rotate;
uniform mat4 m4_viewport;
uniform mat4 m4_projection;

smooth out vec2 v2_outTexCoord;

void main()
{
	mat4 model = m4_scale * m4_rotate * m4_translate;
	mat4 mvp = transpose(m4_projection) * transpose(m4_viewport) * transpose(model);
	
    gl_Position = mvp * vec4(position, 1);

    v2_outTexCoord = uvPosition;
}