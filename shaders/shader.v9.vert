#version 330 core

in vec3 inPos;	
in vec3 inNormal;

uniform mat4 modelViewProj;
uniform mat4 modelView;
//La matriz normal la tenemos que calcular y usarla siemprw que hagamos cambio de normales. si no los mapas de normales se ven raros)
//La matriz de camara esta en la matriz view por o que el sistema de coordenadas que vamos a mover es el view.Lo que decide hacia donde apunta una camra es el view
out vec3 norm;
void main()
{
	norm= (transpose(inverse(modelView))*vec4(inNormal,0.0)).xyz;
	gl_Position = modelViewProj * vec4(inPos,1.0);
}


