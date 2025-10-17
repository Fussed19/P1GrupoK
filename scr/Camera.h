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

		int width;
		int height;

		float c_speed = 0.1f;
		float c_sensibility = 0.5f;

		float fov = 60.0f;

		float c_yaw = -90.0f;
		float c_pitch = 0.0f;

		Camera(int width, int height, glm::vec3 pos);

		void setSensibility(float sensibity);
		void setWidth(int width);
		void setHeight(int heigth);

		void defMatrix(float fov, float near, float far);

		void refreshMatrixView();
		void refreshMatrixProj(float near, float far);

		void rotForward(float dX, float dY); //Cambiamos(rotamos) la direccion en la que miramos dad una entrada

		void keyInput(unsigned char key);

		void zoom(float d);
};

#endif