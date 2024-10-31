#version 330 core
out vec4 FragColor;

in vec3 crntPos;
in vec3 normal;
in vec2 texCoord;

uniform vec3 gridColor; // New uniform for grid color

void main()
{
    FragColor = vec4(gridColor, 1.0f); // Set color to the uniform value
}
