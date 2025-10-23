#version 330 core

in vec3 inPos;	
in vec3 inNormal;

uniform mat4 modelViewProj;
out vec3 norm;
void main()
{
	norm= inNormal;
	gl_Position = modelViewProj * vec4(inPos,1.0);
}


