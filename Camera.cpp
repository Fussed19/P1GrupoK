#include "Camera.h"

Camera::Camera(int width, int height, glm::vec3 pos) {
	Camera::width = width;
	Camera::height = height;
	Camera::c_pos = pos;
}

void Camera::initMatrix(float fov, float near, float far) {

	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 proj = glm::mat4(1.0f);

	//Ajustamos view
	view = glm::lookAt(c_pos, c_pos + c_forward, c_up);
	IGlib::setViewMat(view);

	//ajustamos proj
	glm::mat4 proj = glm::perspective(fov, (float)width / (float)height, near, far);
	IGlib::setProjMat(proj);
}
