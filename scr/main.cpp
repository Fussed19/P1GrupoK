#include "BOX.h"
#include <IGL/IGlib.h>
#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>


//Idenficadores de los objetos de la escena
int objId = -1;

//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMoveFunc(int x, int y);
void mousePassMoveFunc(int x, int y);
void mouseWheelFunc(int wheel, int dir, int x, int y);

//Def parametros de la proyeccion
int w_width = 600;
int w_height = 600;

float fov = glm::radians(60.0f);//60º
float near = 0.1f;
float far = 50.0f;

//Definicion mouse
bool centralButtonPressed = false;
float lastX = (float)w_width / 2.0f;
float lastY = (float)w_height / 2.0f;

//Creacion de la camara
Camera camera(w_width, w_height, glm::vec3(0.0f, 0.0f, 3.0f));


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("./shaders/shader.v1.vert", "./shaders/shader.v1.frag"))
		return -1;

	//Camera init
	camera.defMatrix(fov, near, far);


	//Creamos el objeto que vamos a visualizar

	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);

	//Incluir texturas aquí.


	//CBs
	IGlib::setResizeCB(resizeFunc);
	IGlib::setIdleCB(idleFunc);
	IGlib::setKeyboardCB(keyboardFunc);
	IGlib::setMouseCB(mouseFunc);
	IGlib::setMouseMoveCB(mouseMoveFunc);
	IGlib::setMousePassiveMoveCB(mousePassMoveFunc);
	IGlib::setMouseWheelMoveCB(mouseWheelFunc);


	//Mainloop
	IGlib::mainLoop();
	IGlib::destroy();
	return 0;
}

void resizeFunc(int width, int height)
{
    w_width = width;
    w_height = height;

	camera.setHeight(w_height);
	camera.setWidth(w_width);

	camera.refreshMatrixProj(near, far);
}

void idleFunc()
{
	camera.refreshMatrixView();
}

void keyboardFunc(unsigned char key, int x, int y)
{
	camera.keyInput(key);
}

void mouseFunc(int button, int state, int x, int y)
{
	if (state == 0) {
		if (button == 1) centralButtonPressed = true;

		lastX = (float)x;
		lastY = (float)y;

		//std::cout << "Se ha pulsado el boton derecho del raton" << std::endl;
	}
	else {
		if (button == 1) centralButtonPressed = false;
		//std::cout << "Se ha soltado el boton derecho del raton" << std::endl;
	}
}


void mouseMoveFunc(int x, int y)
{

	if (centralButtonPressed) {

		//std::cout << "Se ha pulsado el boton derecho del raton y se esta moveindo" << std::endl;

		float dX = (float)(x - lastX);
		float dY = (float)(y - lastY);

		lastX = (float)x;
		lastY = (float)y;

		camera.rotForward(dX, -dY);
		
	}


}

void mousePassMoveFunc(int x, int y)
{
	//std::cout << "Pasivo en la posición " << x << " " << y << std::endl << std::endl;
}
void mouseWheelFunc(int wheel, int dir, int x, int y)
{
	float zoomVel = glm::radians(2.0f);

	if (dir > 0) {
		camera.zoom(-zoomVel);
	}
	else {
		camera.zoom(zoomVel);
	}

	camera.refreshMatrixProj(near, far);

	//std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}