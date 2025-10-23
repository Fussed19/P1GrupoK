#version 330 core

in vec3 inPos;	
unriform mat4 proj;
uniform mat4 view;
unirform mat4 model;
void main()
{
	gl_Position = proj*modelView*vec4(inPos,1.0);
}
