#version 330 core
out vec4 FragColor;

in vec3 crntPos;

uniform vec3 gridColor; // uniform for grid color

void main()
{
    FragColor = vec4(gridColor, 1.0f);
}