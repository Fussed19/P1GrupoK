#version 330 core


in vec3 norm;
out vec4 outColor;


void main()
{
outColor= vec4((norm,1.0); // en este las normales negativas son las mas oscuras 
//outColor= vec4((norm+vec3(1.0))/2.0,1.0); //nos permite ver las normales 
}
