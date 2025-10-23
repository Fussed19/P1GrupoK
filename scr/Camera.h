#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <IGL/IGlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
	public:
		glm::vec3 c_pos; //posicion camara
		glm::vec3 c_forward = glm::vec3(0.0f, 0.0f, -1.0f); //direccion de la camara(donde apunta)
		glm::vec3 c_up = glm::vec3(0.0f, 1.0f, 0.0f);//valores defecto
		//tamaño camara
		int width;
		int height;
		//velocidad al moverse y sensibilidad(raton)
		float c_speed = 0.5f;
		float c_sensibility = 0.5f;
		//Fov (angulo para y)
		float fov = 60.0f;
		//Valores giro defecto
		float c_yaw = -90.0f;
		float c_pitch = 0.0f;

		Camera(int width, int height, glm::vec3 pos); //Cosntructor
		//Setters
		void setSensibility(float sensibity);
		void setWidth(int width);
		void setHeight(int heigth);
		//METODOS VARIOS
		//definir matrices de la camara(view y proj) para iniciar
		void defMatrix(float fov, float near, float far);
		//actualizador de matrices
		void refreshMatrixView();
		void refreshMatrixProj(float near, float far);
		//Giro de la camara según deltas (input ratón)
		void rotForward(float dX, float dY); //Cambiamos(rotamos) la direccion en la que miramos dad una entrada
		//Transformaciones de la camara (movimiento) por inputs de teclado
		void keyInput(unsigned char key);
		//Zoom (modifica el fov) según rueda ratón.
		void zoom(float d);
};

#endif