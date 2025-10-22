#include <vector>
#include <cmath>

#include "BOX.h"
#include <IGL/IGlib.h>
#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
//assim libs
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


//Declaración de CB
void resizeFunc(int width, int height);
void idleFunc();
void keyboardFunc(unsigned char key, int x, int y);
void mouseFunc(int button, int state, int x, int y);
void mouseMoveFunc(int x, int y);
void mousePassMoveFunc(int x, int y);
void mouseWheelFunc(int wheel, int dir, int x, int y);

//variables de carga de objetos 
const aiScene* sc;//escena con obejetos 3d cargadior en formato interno de assimp
//funcion propio craga de objetos
bool cargaModelos3D(const std::string& pFile);

glm::mat4 escalarObj(int objId, float escala);
glm::mat4 moverObj(int objId, glm::vec3 xyz);
glm::mat4 rotarObj(int objId, glm::vec3 angles);

void transformarObj(std::vector<int> localObjIds, glm::mat4 modelMat);

//Def parametros de la proyeccion
int w_width = 600;
int w_height = 600;

float fov = glm::radians(60.0f);//60º
float near = 0.1f;
float far = 50.0f;

//Definicion mouse y teclado
bool rPressed = false;
bool ePressed = false;
bool tPressed = false;
bool rightButtonPressed = false;
bool centralButtonPressed = false;
float lastX = (float)w_width / 2.0f;
float lastY = (float)w_height / 2.0f;

//Creacion de la camara
Camera camera(w_width, w_height, glm::vec3(0.0f, 0.0f, 3.0f));

//vector de IDs de objetos cargados
std::vector<std::vector<int>> objIds;
std::vector<glm::mat4> modelMatObjs;
int objSelected = 0; //Para seleccionar objetos con el teclado del 0 al 9.

int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("./shaders/shader.v1.vert", "./shaders/shader.v1.frag"))
		return -1;
	IGlib::setTimer(16);
	//Camera init
	camera.defMatrix(fov, near, far);

	//cargar objeto de fichero

	cargaModelos3D("../3D_assets/AlienAnimal.obj");
	
	cargaModelos3D("../3D_assets/AlienAnimal.obj");
	
	


	escalarObj(1, 0.1f);

	moverObj(0, glm::vec3(-10.0f, 0.0f, 0.0f));
	escalarObj(0, 0.1f);

	

	

	/*
	objId = IGlib::createObj(cubeNTriangleIndex, cubeNVertex, cubeTriangleIndex,
		cubeVertexPos, cubeVertexColor, cubeVertexNormal, cubeVertexTexCoord, cubeVertexTangent);

	glm::mat4 modelMat = glm::mat4(1.0f);
	IGlib::setModelMat(objId, modelMat);
	*/
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
	camera.setHeight(height);
	camera.setWidth(width);

	camera.refreshMatrixProj(near, far);
}


void idleFunc()
{
	camera.refreshMatrixView();

	static float orbit = 0.0f;
	static float rot = 0.0f;
	float radius = 5.0f;
	float doblePi = 2.0 * 3.14159265358979323846f;

	orbit += 0.002f;
	if (orbit > doblePi) orbit -= doblePi;

	rot += 0.001f;
	if (rot > doblePi) rot -= doblePi;

	glm::vec3 desplazamiento = glm::vec3(radius * cos(orbit), 0.0f, radius * sin(orbit));
	moverObjTo(0, desplazamiento);
	rotarObj(0, )

}

void keyboardFunc(unsigned char key, int x, int y)
{
	if (key >= '0' && key <= '9') {
		int sel = key - '0';
		if (sel < objIds.size()) {
			objSelected = sel;
			std::cout << "objSelected = " << objSelected << std::endl;
		}
		else {
			std::cout << "Objeto " << sel << " no existe" << std::endl;
		}
	}
	if (key == 'r' || key == 'R') {
		rPressed = !rPressed;
		std::cout << "Modo rotar: " << (rPressed ? "ON" : "OFF") << std::endl;
	}
	if (key == 'e' || key == 'E') {
		ePressed = !ePressed;
		std::cout << "Modo escalar: " << (ePressed ? "ON" : "OFF") << std::endl;
	}
	if (key == 't' || key == 'T') {
		tPressed = !tPressed;
		std::cout << "Modo mover: " << (tPressed ? "ON" : "OFF") << std::endl;
	}
	camera.keyInput(key);

	camera.keyInput(key);
}

void mouseFunc(int button, int state, int x, int y)
{
	bool pressed = (state == 0); 

	if (button == 1) centralButtonPressed = pressed; // central
	if (button == 2) rightButtonPressed = pressed;   // derecho

	if (pressed) {
		lastX = (float)x;
		lastY = (float)y;
	}
}


void mouseMoveFunc(int x, int y)
{

	float dX = (float)(x - lastX);
	float dY = (float)(y - lastY);

	lastX = (float)x;
	lastY = (float)y;

	if (centralButtonPressed) {
		camera.rotForward(dX, -dY);
	}

	if (rightButtonPressed && rPressed) {
		//std::cout << "ROTANDO " << std::endl;
		//similar al giro de camara con mouse
		float rx = glm::radians(dY); 
		float ry = glm::radians(dX); 
		rotarObj(objSelected, glm::vec3(3.0f*rx, 3.0f*ry, 0.0f));
	}
	else if (rightButtonPressed && ePressed) {
		//std::cout << "ESCALANDO " << std::endl;
		
		float delta = -dY * 0.02f; 
		// clamp para evitar escalados negativos / enormes
		float factor = 1.0f + delta;
		if (factor < 0.01f) factor = 0.01f;
		escalarObj(objSelected, factor);
	}
	else if (rightButtonPressed && tPressed) {
		//std::cout << "TRANSLADANDO " << std::endl;

		glm::vec3 deltaPos = glm::vec3(dX * 0.5f, 0.0f , dY * 0.5f);
		moverObj(objSelected, deltaPos);
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
		if(rPressed) rotarObj(objSelected, glm::vec3(0.0f, 0.0f, -glm::radians(40.0f)));
		else if(tPressed) moverObj(objSelected, glm::vec3(0.0f, 1.0f ,0.0f));
		else camera.zoom(-zoomVel);
	}
	else {
		if (rPressed) rotarObj(objSelected, glm::vec3(0.0f, 0.0f, glm::radians(40.0f)));
		else if (tPressed) moverObj(objSelected, glm::vec3(0.0f, -1.0f, 0.0f));
		else camera.zoom(zoomVel);
	}

	camera.refreshMatrixProj(near, far);

	//std::cout << "en la posición " << x << " " << y << std::endl << std::endl;
}

bool cargaModelos3D(const std::string& pFile)
{

	Assimp::Importer importer;
	sc = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!sc) {
		return false;
	}
	std::cout << "Se han cargado " << sc->mNumMeshes << " meshes"
		<< std::endl;

	//bucle de craga de meshes
	std::vector<int> localObjIds;

	for (unsigned int n = 0; n < sc->mNumMeshes; ++n) {

		const aiMesh* mesh = sc->mMeshes[n];
		/*Tenemos que convertir del formato interno de escena de assimp (aiFace, aiVector3D ,etc...) a los
		tipos esperados por openGL en los buffer objects (unsigned ints, floats, etc..)Se los pasamos al
		metodo "createObj" de IGLib para que los pase a los buffers GL
		*/

		

		unsigned int* faceArray = nullptr;
		float* vertexArray = nullptr;
		float* normalArray = nullptr;
		float* textCoordsArray = nullptr;

		unsigned int faceIndex = 0;
		unsigned int vertexIndex = 0;
		unsigned int normalIndex = 0;

		if (mesh->HasFaces()) {
			faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);
			if (!faceArray) {
				std::cerr << "Error: malloc faceArray failed\n";
				return false;
			}
			for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
				const aiFace* face = &mesh->mFaces[t];
				memcpy(&faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
				faceIndex += 3;
			}
		}
		if (mesh->HasPositions()) { 
			vertexArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
			if (!vertexArray) {
				std::cerr << "Error: malloc vertexArray failed\n";
				free(faceArray);
				return false;
			}
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* vertex = &mesh->mVertices[k];
				memcpy(&vertexArray[vertexIndex], &(vertex->x), sizeof(float));
				memcpy(&vertexArray[vertexIndex + 1], &(vertex->y), sizeof(float));
				memcpy(&vertexArray[vertexIndex + 2], &(vertex->z), sizeof(float));
				vertexIndex += 3;
			}
		}
		if (mesh->HasNormals()) {
			normalArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
			if (!normalArray) {
				std::cerr << "Error: malloc normalArray failed\n";
				free(faceArray); free(vertexArray);
				return false;
			}
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_normal = &mesh->mNormals[k];
				memcpy(&normalArray[normalIndex], &(a_normal->x), sizeof(float));
				memcpy(&normalArray[normalIndex + 1], &(a_normal->y), sizeof(float));
				memcpy(&normalArray[normalIndex + 2], &(a_normal->z), sizeof(float));
				normalIndex += 3;
			}
		}

		if (mesh->HasTextureCoords(0)) {
			textCoordsArray = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);
			if (!textCoordsArray) {
				std::cerr << "Error: malloc textCoordsArray failed\n";
				free(faceArray); free(vertexArray); free(normalArray);
				return false;
			}
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_textCoord = &mesh->mTextureCoords[0][k];
				memcpy(&textCoordsArray[k * 2], &(a_textCoord->x), sizeof(float));
				memcpy(&textCoordsArray[k * 2 + 1], &(a_textCoord->y), sizeof(float));
			}
		}
		
		int id = IGlib::createObj(mesh->mNumFaces, mesh->mNumVertices, faceArray, vertexArray, 0, normalArray, textCoordsArray, 0);
		localObjIds.push_back(id);

		glm::mat4 modelMat = glm::mat4(1.0f);
		
		IGlib::setModelMat(localObjIds.back(), modelMat);

		free(faceArray);
		free(vertexArray);
		free(normalArray);
		free(textCoordsArray);
	}
	glm::mat4 modelMat = glm::mat4(1.0f);
	objIds.push_back(localObjIds);
	modelMatObjs.push_back(modelMat);

	return true;
}

glm::mat4 escalarObj(int objId, float escala) {
	if (objId < 0 || objId >= modelMatObjs.size()) return glm::mat4(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::scale(modelMat, glm::vec3(escala, escala, escala));
	modelMatObjs[objId] = modelMatObjs[objId] * modelMat;

	transformarObj(objIds[objId], modelMatObjs[objId]);

	return modelMatObjs[objId];
}

glm::mat4 moverObj(int objId, glm::vec3 xyz) {
	if (objId < 0 || objId >= modelMatObjs.size()) return glm::mat4(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, xyz);
	modelMatObjs[objId] = modelMatObjs[objId] * modelMat;

	transformarObj(objIds[objId], modelMatObjs[objId]);

	return modelMatObjs[objId];
}
glm::mat4 moverObjTo(int objId, glm::vec3 xyz) {
	if (objId < 0 || objId >= modelMatObjs.size()) return glm::mat4(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, xyz);
	modelMatObjs[objId] = modelMat;

	transformarObj(objIds[objId], modelMatObjs[objId]);

	return modelMatObjs[objId];
}

glm::mat4 rotarObj(int objId, glm::vec3 angles) {
	if (objId < 0 || objId >= modelMatObjs.size()) return glm::mat4(1.0f);

	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(angles.x), glm::vec3(1, 0, 0));
	modelMat = glm::rotate(modelMat, glm::radians(angles.y), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, glm::radians(angles.z), glm::vec3(0, 0, 1));

	modelMatObjs[objId] = modelMatObjs[objId] * modelMat;

	transformarObj(objIds[objId], modelMatObjs[objId]);

	return modelMatObjs[objId];

}

void transformarObj(std::vector<int> localObjIds, glm::mat4 modelMat) {

	for (int i = 0; i < localObjIds.size(); i++) {
		IGlib::setModelMat(localObjIds[i],modelMat);
	}
}
