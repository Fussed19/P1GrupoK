#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos) {
	Camera::width = width;
	Camera::height = height;
	Camera::c_pos = pos;
	
}

/////////SETTERS/////////
void Camera::setSensibility(float sensibility) {
	Camera::c_sensibility = sensibility;
}
void Camera::setHeight(int height) {
	Camera::height = height;
}
void Camera::setWidth(int width) {
	Camera::width = width;
}

//////MATRICES//////

//INICIO MATRICES DE VISTA Y PROYECCION
void Camera::defMatrix(float fov, float near, float far) {
	Camera::fov = fov;

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	//Ajustamos view
	view = glm::lookAt(c_pos, c_pos + c_forward, c_up);
	IGlib::setViewMat(view);

	//ajustamos proj
	proj = glm::perspective(fov, (float)width / (float)height, near, far);
	IGlib::setProjMat(proj);
}
//PARA REFRESCAR LA MATRIZ DE VISTA
void Camera::refreshMatrixView() {

	glm::mat4 view = glm::lookAt(c_pos, c_pos + c_forward, c_up);
	IGlib::setViewMat(view);
}
//PARA REFRESCAR LA MATRIZ DE PROJ
void Camera::refreshMatrixProj(float near, float far) {

	glm::mat4 proj = glm::perspective(fov, (float)width / (float)height, near, far);
	IGlib::setProjMat(proj);
}

////////INPUTS////////////
// 
//ROTACION DE CAMARA DADOS INPUT DE MOUSE
void Camera::rotForward(float dX, float dY) {
	c_yaw += dX * c_sensibility;
	c_pitch += dY * c_sensibility;

	if (c_pitch > 89.0f) {
		c_pitch = 89.0f;
	}
	if (c_pitch < -89.0f) {
		c_pitch = -89.0f;
	}

	glm::vec3 newForward;

	float ryaw = glm::radians(c_yaw);
	float rpitch = glm::radians(c_pitch);

	//Esto es una rotación de euler en Rx y Ry ya multiplicada para ahorrar calculos al pc. (forward * Rx * Ry)
	newForward.x = cos(ryaw) * cos(rpitch);
	newForward.y = sin(rpitch);
	newForward.z = sin(ryaw) * cos(rpitch);

	c_forward = glm::normalize(newForward);
	
}
//MOVIMIENTO CON TECLADO
void Camera::keyInput(unsigned char key) {
	//WASD
	if (key == 'w' || key == 'W') {
		c_pos += c_speed * c_forward;
	}
	if (key == 's' || key == 'S') {
		c_pos += c_speed * -c_forward;
	}
	if (key == 'a' || key == 'A') {
		c_pos += c_speed * -glm::normalize(glm::cross(c_forward, c_up)); //esto saca el vector perpendicular a la direcion y al up que equivale a andar de lado.
	}
	if (key == 'd' || key == 'D') {
		c_pos += c_speed * glm::normalize(glm::cross(c_forward, c_up));
	}
	//ZX CONTROL DE ALTURA
	if (key == 'z' || key == 'Z') {
		c_pos += c_speed * c_up;
	}
	if (key == 'x' || key == 'X') {
		c_pos += c_speed * -c_up;
	}

}
//ZOOM 
void Camera::zoom(float d) {
	fov += d;

	if (fov < glm::radians(25.0f)) fov = glm::radians(25.0f);
	if (fov > glm::radians(90.0f)) fov = glm::radians(90.0f);
}
