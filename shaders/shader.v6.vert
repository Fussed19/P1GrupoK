#version 330 core

in vec3 inPos;	
in vec3 inColor;
uniform mat4 modelViewProj;
out vec4 color;
void main()
{
	color= vec4(inColor,1.0);
	gl_Position = modelViewProj*vec4(inPos,1.0);
}


