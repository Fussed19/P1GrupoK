#version 330 core

in vec3 inPos;		
#define M_PI 3.141592
void main()
{
	mat4 modelView= mat4(1.0); 
	//una model view identidad es un lienzo en blanco 
	modelView[3].z=-3;
	//ahora ocn la matrizde proyeccion modificamos como es la perspectiva
	mat4 proj=mat4(1.0);
	float f=1.0/tan(M_PI/6.0);
	//para floats siempre es recomendable que aunque sea un numero entero poner el .0 p.ej 6.0
	//la camara  que acabamos de definir es una focal en 30 grados de apertura, cuanto mas se cierre la apertura mas paralela es la camnara y mas habra que aljear los obejtos, auqnue una camara muy abierta puede llegar a deofrmar los obejtos de la camara (pi/6)
	float far= 10.0;
	float near=0.1;
	//definimos la matriz de proyecciona  mano
	proj[0].x=f;
	proj[1].y=f;
	proj[2].z=(far+near)/(near-far);
	proj[2].w=-1.0;
	proj[3].z=(2.0*far*near)/(near-far);
	proj[3].w=0.0;
	gl_Position = proj*modelView*vec4(inPos,1.0);
}
