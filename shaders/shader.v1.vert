#version 330 core

in vec3 inPos;		
in vec3 inNormal;

uniform mat4 modelViewProj;
uniform mat4 modelView;

out vec3 norm;

void main()
{
	norm = (transpose(inverse(modelView))*vec4(inNormal, 0.0)).xyz;
	gl_Position = modelViewProj * vec4(inPos, 1.0);

}
