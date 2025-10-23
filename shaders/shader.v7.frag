#version 330 core

out vec4 outColor;
in vec2 texCoord;
uniform sampler2D colorTex;


void main()
{
outColor= texture(colorTex,texCoord);// indicamos que queremos el sampler2d de color text y despues indicamos las coordenadas del sampler que queremos
}
