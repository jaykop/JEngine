#version 450 core

out vec4 FragColor;

in vec3 v3_fragPosition;

uniform vec3 v3_cameraPosition;
uniform vec3 v3_color;
uniform samplerCube skybox;

void main()
{    
	FragColor.rgb = texture(skybox, v3_fragPosition).rgb * v3_color;
	FragColor.w = 1.f;
}
