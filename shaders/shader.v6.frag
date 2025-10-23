#version 330 core

out vec4 outColor;
in vec4 color; //color de entrada desde el vertex


void main()
{
outColor= vec4(color);
}
