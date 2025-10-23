#version 330 core

in vec3 inPos;	
uniform mat4 modelViewProj;
out vec4 color;
void main()
{
	if(mod(gl_VertexID,4.0)==0.0)
		color = vec4(1.0,0.0,0.0,0.0); 
	else if (mod(gl_VertexID,4.0)==1.0)
		color = vec4(0.0,1.0,0.0,0.0); 
	else if (mod(gl_VertexID,4.0)==2.0)
		color = vec4(0.0,0.0,1.0,0.0); 
	else if (mod(gl_VertexID,4.0)==3.0)
		color = vec4(1.0,1.0,1.0,0.0); 

	gl_Position = modelViewProj*vec4(inPos,1.0);
}


