#include <vector>

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

glm::mat4 escalarObj(std::vector<int> localObjIds, float escala);
glm::mat4 moverObj(std::vector<int> localObjIds, glm::vec3 xyz);
glm::mat4 rotarObj(std::vector<int> localObjIds, glm::vec3 angles);

void transformarObj(std::vector<int> localObjIds, glm::mat4 modelMat);

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

//vector de IDs de objetos cargados
std::vector<std::vector<int>> objIds;


int main(int argc, char** argv)
{
	std::locale::global(std::locale("spanish"));// acentos ;)
	if (!IGlib::init("./shaders/shader.v1.vert", "./shaders/shader.v1.frag"))
		return -1;

	//Camera init
	camera.defMatrix(fov, near, far);

	//cargar objeto de fichero

	cargaModelos3D("../3D_assets/AlienAnimal.obj");
	
	cargaModelos3D("../3D_assets/AlienAnimal.obj");
	
	

	transformarObj(objIds[1], moverObj(objIds[1], glm::vec3(0.0f, 0.0f, 0.0f)) * escalarObj(objIds[1], 0.1f));

	transformarObj(objIds[0], moverObj(objIds[0], glm::vec3(-10.0f, 0.0f, 0.0f)) * escalarObj(objIds[0], 0.1f));
	

	

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

	static float angle = 0.0f;
	static float velocity = 0.05f;
	float radius = 5.0f;

	angle = (angle < 3.141592f * 2.0f) ? angle + 0.5f : 0.0f;
	glm::vec3 desplazamiento = glm::vec3(radius * cos(angle*velocity), 0.0f, radius * sin(angle*velocity));
	glm::mat4 rotacion180 = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f) + 10 * glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));

	transformarObj(objIds[0], moverObj(objIds[0], desplazamiento) * rotacion180 * escalarObj(objIds[0], 0.1f));

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

bool cargaModelos3D(const std::string& pFile)
{

	Assimp::Importer importer;
	sc = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!sc) {
		//DoTheErrorLogging(importer.GetErrorString());
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
	objIds.push_back(localObjIds);
	return true;
}


glm::mat4 escalarObj(std::vector<int> localObjIds, float escala) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::scale(modelMat, glm::vec3(escala, escala, escala));
	return modelMat;
}

glm::mat4 moverObj(std::vector<int> localObjIds, glm::vec3 xyz) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::translate(modelMat, xyz);
	return modelMat;
}

glm::mat4 rotarObj(std::vector<int> localObjIds, glm::vec3 angles) {
	glm::mat4 modelMat = glm::mat4(1.0f);
	modelMat = glm::rotate(modelMat, glm::radians(angles.x), glm::vec3(1, 0, 0));
	modelMat = glm::rotate(modelMat, glm::radians(angles.y), glm::vec3(0, 1, 0));
	modelMat = glm::rotate(modelMat, glm::radians(angles.z), glm::vec3(0, 0, 1));
	return modelMat;

}

void transformarObj(std::vector<int> localObjIds, glm::mat4 modelMat) {

	for (int i = 0; i < localObjIds.size(); i++) {
			IGlib::setModelMat(localObjIds[i], modelMat);
	}
}