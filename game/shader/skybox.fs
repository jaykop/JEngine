#version 450 core
out vec4 FragColor;

in vec3 v3_fragPosition;
uniform samplerCube skybox;

void main()
{    
    FragColor = texture(skybox, v3_fragPosition);
}