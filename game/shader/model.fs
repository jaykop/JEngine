#version 450 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

uniform vec4 v4_color;

void main()
{    
    FragColor = texture(texture_diffuse1, TexCoords) * v4_color;
}