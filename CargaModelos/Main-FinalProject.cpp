/*
Semestre 2022-2
Pr�ctica 5: Carga de Modelos
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"

//#include "Modelos_MuchaLucha.h"

float contadorDiaNoche = 0.0f;
float posicionLedX, posicionLedZ, posicionLed1X, posicionLed1Z;
int banderaLedCama, banderaLedEscritorio, LedCama;
bool  dia = false;
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

//Camaras
Camera * camera;
Camera cameraWheezy;
Camera cameraLibre;



Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture plainBlueTexture;
Texture pisoTexture;

Texture WheezyTexture;
Texture FrijolitoTexture;
Texture JettTexture;

Texture StreetMan1_Texture;
Texture StreetMan2_Texture;
Texture StreetMan3_Texture;

Model Cuarto_M = Model();
Model Escritorio_M = Model();
Model Escritorio2_M = Model();
Model Escritorio3_M = Model();
Model Lampara_M = Model();
Model Dado_M = Model();
Model Cubo_M = Model();
Model Silla_M = Model();
Model Bote_basura_M = Model();
Model Puff_M = Model();
Model SetUp_M = Model();
Model Gabinete_M = Model();
Model HeadSet_M = Model();
Model LuzTecho_M = Model();
Model JettCompleta = Model();
Model Cama_M = Model();
Model Alfombra_M = Model();
//Wheezy
Model Wheezy_torso_M = Model();
Model Wheezy_cabeza_M = Model();
Model Wheezy_brazo_izq_M = Model();
Model Wheezy_brazo_der_M = Model();
Model Wheezy_pie_izq_M = Model();
Model Wheezy_pie_der_M = Model();


Model ML_Ring_M = Model();
Model Luchador_M = Model();

Skybox skybox;
//materiales
Material Material_brillante;
Material Material_opaco;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLightsNoche[MAX_POINT_LIGHTS];
PointLight pointLightsCama[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

Sphere sp = Sphere(10.0, 30, 30); //radio horizontal vertical





//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 1, 2,
		3, 4, 5,
		5, 6, 7,
		7, 8, 9,
		9, 10, 11,
		2, 1, 9,
		1, 5, 9,
		5, 7, 9
	};

	GLfloat vertices[] = {
		//		x      		y 		     z				u	  v			nx	  ny    nz
			0.054193f, -0.55023f, 0.83147f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.15008f,  -0.12317f, 0.98079f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.13795f,  -0.13795f, 0.98079f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			
			0.15008f,  -0.12317f, 0.98079f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.55023f,  -0.054193f, 0.83147f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.25788f,  0.12742f, 0.95233f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,

			0.30866f,  0.46194f, 0.83147f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.037329f,  0.28521f, 0.95233f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,

			-0.34285f,  0.39285f, 0.83147f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.27245f,  0.092253f, 0.95233f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,

			-0.53089f,  -0.11439f, 0.83147f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			-0.13795f,  -0.13795f, 0.98079f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,

	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	//En create objects: 
	unsigned int vegetacionIndices[] = {
			0, 1, 2,
			0, 2, 3,
			4,5,6,
			4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 24, vertices, 96, 8, 5);


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 96, 24);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 96, 24);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);
}

void CreatePersonaje() {
	unsigned int indices[] = {
		//arriba
		0,1,2,		//0, 1, 2,
		3,4,5,		//1, 2, 3,
		//izq
		6,7,8,		//0, 2, 4,
		9,10,11,	//2, 4, 6,
		//atras
		12,13,14,	//0, 1, 5,
		15,16,17,	//0, 4, 5,
		//der
		18,19,20,	//1, 3, 5,
		21,22,23,	//3, 5, 7,
		//abajo
		24,25,26,	//4, 5, 6,
		27,28,29,	//5, 6, 7
		//frente
		30,31,32,	//2, 3, 6,
		33,34,35,	//3, 6, 7,
	};

	GLfloat verticesCabeza[] = {
		//	x      y      z			u	  v				nx	  ny    nz
			//arriba
			-0.25f, 0.25f, -0.25f,	0.125f, 1.0f,		0.0f, 1.0f, 0.0f, //0
			0.25f, 0.25f, -0.25f,	0.250f, 1.0f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.25f, 0.25f,	0.125f, 0.875f,		0.0f, 1.0f, 0.0f, //2

			0.25f, 0.25f, -0.25f,	0.250f, 1.0f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.25f, 0.25f,	0.125f, 0.875f,		0.0f, 1.0f, 0.0f, //2
			0.25f, 0.25f, 0.25f,	0.250f, 0.875f,		0.0f, 1.0f, 0.0f, //3

			//izq
			-0.25f, 0.25f, -0.25f,	0.0f, 0.875f,		-1.0f, 0.0f, 0.0f, //0
			-0.25f, 0.25f, 0.25f,	0.125f, 0.875f,		-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.25f, -0.25f,	0.0f, 0.75f,		-1.0f, 0.0f, 0.0f, //4

			-0.25f, 0.25f, 0.25f,	0.125f, 0.875f,		-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.25f, -0.25f,	0.0f, 0.75f,		-1.0f, 0.0f, 0.0f, //4
			-0.25f, -0.25f, 0.25f,	0.125f, 0.75f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.25f, 0.25f, -0.25f,	0.375f, 0.875f,		0.0f, 0.0f, -1.0f, //0
			0.25f, 0.25f, -0.25f,	0.5f, 0.875f,		0.0f, 0.0f, -1.0f, //1
			0.25f, -0.25f, -0.25f,	0.5f, 0.75f,		0.0f, 0.0f, -1.0f, //5

			-0.25f, 0.25f, -0.25f,	0.375f, 0.875f,		0.0f, 1.0f, -1.0f, //0
			-0.25f, -0.25f, -0.25f,	0.5f, 0.75f,		0.0f, 0.0f, -1.0f, //4
			0.25f, -0.25f, -0.25f,	0.5f, 0.75f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.25f, 0.25f, -0.25f,	0.5f, 0.875f,		1.0f, 0.0f, 0.0f, //1
			0.25f, 0.25f, 0.25f,	0.25f, 0.875f,		1.0f, 0.0f, 0.0f, //3
			0.25f, -0.25f, -0.25f,	0.5f, 0.75f,		1.0f, 0.0f, 0.0f, //5

			0.25f, 0.25f, 0.25f,	0.25f, 0.875f,		1.0f, 0.0f, 0.0f, //3
			0.25f, -0.25f, -0.25f,	0.5f, 0.75f,		1.0f, 0.0f, 0.0f, //5
			0.25f, -0.25f, 0.25f,	0.25f, 0.75f,		1.0f, 0.0f, 0.0f, //7

			//abajo
			-0.25f, -0.25f, -0.25f,	0.25f, 1.0f,		0.0f, -1.0f, 0.0f, //4
			0.25f, -0.25f, -0.25f,	0.375f, 1.0f,		0.0f, -1.0f, 0.0f, //5
			-0.25f, -0.25f, 0.25f,	0.25f, 0.875f,		0.0f, -1.0f, 0.0f, //6

			0.25f, -0.25f, -0.25f,	0.375f, 1.0f,		0.0f, -1.0f, 0.0f, //5
			-0.25f, -0.25f, 0.25f, 0.25f, 0.875f,		0.0f, -1.0f, 0.0f, //6
			0.25f, -0.25f, 0.25f,	0.375f, 0.875f,		0.0f, -1.0f, 0.0f, //7

			//frente
			-0.25f, 0.25f, 0.25f,	0.125f, 0.875f,		0.0f, 0.0f, 1.0f, //2
			0.25f, 0.25f, 0.25f,	0.25f, 0.875f,		0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.25f, 0.25f,	0.125f, 0.75f,		0.0f, 0.0f, 1.0f, //6

			0.25f, 0.25f, 0.25f,	0.25f, 0.875f,		0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.25f, 0.25f,	0.125f, 0.75f,		0.0f, 0.0f, 1.0f, //6
			0.25f, -0.25f, 0.25f,	0.25f, 0.75f,		0.0f, 0.0f, 1.0f, //7
	};

	GLfloat verticesTorso[] = {
		//	x      y      z				u	  v				nx	  ny    nz
			//arriba
			-0.25f, 0.375f, -0.125f,	0.4375f, 0.5f,		0.0f, 1.0f, 0.0f, //0
			0.25f, 0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 1.0f, 0.0f, //2

			0.25f, 0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 1.0f, 0.0f, //2
			0.25f, 0.375f, 0.125f,		0.5625f, 0.4375f,	0.0f, 1.0f, 0.0f, //3

			//izq
			-0.25f, 0.375f, -0.125f,	0.25f, 0.4375f,		-1.0f, 0.0f, 0.0f, //0
			-0.25f, 0.375f, 0.125f,		0.3125f, 0.4375f,	-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.375f, -0.125f,	0.25f, 0.25f,		-1.0f, 0.0f, 0.0f, //4

			-0.25f, 0.375f, 0.125f,		0.3125f, 0.4375f,	-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.375f, -0.125f,	0.25f, 0.25f,		-1.0f, 0.0f, 0.0f, //4
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.25f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.25f, 0.375f, -0.125f,	0.4375f, 0.4375f,	0.0f, 0.0f, -1.0f, //0
			0.25f, 0.375f, -0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, -1.0f, //1
			0.25f, -0.375f, -0.125f,	0.5625f, 0.25f,		0.0f, 0.0f, -1.0f, //5

			-0.25f, 0.375f, -0.125f,	0.4375f, 0.4375f,	0.0f, 1.0f, -1.0f, //0
			-0.25f, -0.375f, -0.125f,	0.4375f, 0.25f,		-1.0f, 0.0f, -1.0f, //4
			0.25f, -0.375f, -0.125f,	0.5625f, 0.25f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.25f, 0.375f, -0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, 1.0f, //1
			0.25f, 0.375f, 0.125f,		0.625f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			0.25f, -0.375f, -0.125f,	0.5625f, 0.25f,		0.0f, 0.0f, 1.0f, //5

			0.25f, 0.375f, 0.125f,		0.625f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			0.25f, -0.375f, -0.125f,	0.5625f, 0.25f,		0.0f, 0.0f, 1.0f, //5
			0.25f, -0.375f, 0.125f,		0.625f, 0.25f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.25f, -0.375f, -0.125f,	0.4375f, 0.5f,		0.0f, 0.0f, 1.0f, //4
			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, 1.0f, //5
			-0.25f, -0.375f, 0.125f,	0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //6

			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, 0.0f, //5
			-0.25f, -0.375f, 0.125f,	0.4375f, 0.4375f,	0.0f, 0.0f, 0.0f, //6
			0.25f, -0.375f, 0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, 0.0f, //7

			//frente
			-0.25f, 0.375f, 0.125f,		0.3125f, 0.4375f,	0.0f, 0.0f, 1.0f, //2
			0.25f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.25f,		0.0f, 0.0f, 1.0f, //6

			0.25f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.25f,		0.0f, 0.0f, 1.0f, //6
			0.25f, -0.375f, 0.125f,		0.4375f, 0.25f,		0.0f, 0.0f, 1.0f, //7
	};

	GLfloat verticesBrazo[] = {
		//	x      y      z				u	  v				nx	  ny    nz
			//arriba
			-0.1f, 0.375f, -0.125f,		0.6875f, 0.75f,		0.0f, 1.0f, 0.0f, //0
			0.1f, 0.375f, -0.125f,		0.734375f, 0.75f,	0.0f, 1.0f, 0.0f, //1
			-0.1f, 0.375f, 0.125f,		0.6875f, 0.6875f,	0.0f, 1.0f, 0.0f, //2

			0.1f, 0.375f, -0.125f,		0.734375f, 0.75f,	0.0f, 1.0f, 0.0f, //1
			-0.1f, 0.375f, 0.125f,		0.6875f, 0.6875f,	0.0f, 1.0f, 0.0f, //2
			0.1f, 0.375f, 0.125f,		0.734375f, 0.5f,	0.0f, 1.0f, 0.0f, //3

			//izq
			-0.1f, 0.375f, -0.125f,		0.625f, 0.6875f,	-1.0f, 0.0f, 0.0f, //0
			-0.1f, 0.375f, 0.125f,		0.6875f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.1f, -0.375f, -0.125f,	0.625f, 0.5f,		-1.0f, 0.0f, 0.0f, //4

			-0.1f, 0.375f, 0.125f,		0.6875f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.1f, -0.375f, -0.125f,	0.625f, 0.5f,		-1.0f, 0.0f, 0.0f, //4
			-0.1f, -0.375f, 0.125f,		0.6875f, 0.5f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.1f, 0.375f, -0.125f,		0.734375f, 0.6875f,	0.0f, 0.0f, -1.0f, //0
			0.1f, 0.375f, -0.125f,		0.78125f, 0.6875f,	0.0f, 0.0f, -1.0f, //1
			0.1f, -0.375f, -0.125f,		0.78125f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			-0.1f, 0.375f, -0.125f,		0.734375f, 0.6875f,	0.0f, 1.0f, -1.0f, //0
			-0.1f, -0.375f, -0.125f,	0.734375f, 0.5f,	-1.0f, 0.0f, -1.0f, //4
			0.1f, -0.375f, -0.125f,		0.78125f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.1f, 0.375f, -0.125f,		0.78125f, 0.6875f,	0.0f, 0.0f, 1.0f, //1
			0.1f, 0.375f, 0.125f,		0.84375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			0.1f, -0.375f, -0.125f,		0.78125f, 0.5f,		0.0f, 0.0f, 1.0f, //5

			0.1f, 0.375f, 0.125f,		0.84375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			0.1f, -0.375f, -0.125f,		0.78125f, 0.5f,		0.0f, 0.0f, 1.0f, //5
			0.1f, -0.375f, 0.125f,		0.84375f, 0.5f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.1f, -0.375f, -0.125f,	0.734375f, 0.75f,	0.0f, 0.0f, 1.0f, //4
			0.1f, -0.375f, -0.125f,		0.78125f, 0.75f,	0.0f, 0.0f, 1.0f, //5
			-0.1f, -0.375f, 0.125f,		0.734375f, 0.6875f,	0.0f, 0.0f, 1.0f, //6

			0.1f, -0.375f, -0.125f,		0.78125f, 0.75f,	0.0f, 0.0f, 0.0f, //5
			-0.1f, -0.375f, 0.125f,		0.734375f, 0.6875f,	0.0f, 0.0f, 0.0f, //6
			0.1f, -0.375f, 0.125f,		0.78125f, 0.6875f,	0.0f, 0.0f, 0.0f, //7

			//frente
			-0.1f, 0.375f, 0.125f,		0.6875f, 0.6875f,	0.0f, 0.0f, 1.0f, //2
			0.1f, 0.375f, 0.125f,		0.734375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.1f, -0.375f, 0.125f,		0.6875f, 0.5f,		0.0f, 0.0f, 1.0f, //6

			0.1f, 0.375f, 0.125f,		0.734375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.1f, -0.375f, 0.125f,		0.6875f, 0.5f,		0.0f, 0.0f, 1.0f, //6
			0.1f, -0.375f, 0.125f,		0.734375f, 0.5f,	0.0f, 0.0f, 1.0f, //7
	};

	GLfloat verticesPierna[] = {
		//	x      y      z				u	  v				nx	  ny    nz
			//arriba
			-0.125f, 0.375f, -0.125f,	0.0625f, 0.75f,		0.0f, 1.0f, 0.0f, //0
			0.125f, 0.375f, -0.125f,	0.125f, 0.75f,		0.0f, 1.0f, 0.0f, //1
			-0.125f, 0.375f, 0.125f,	0.0625f, 0.6875f,	0.0f, 1.0f, 0.0f, //2

			0.125f, 0.375f, -0.125f,	0.125f, 0.75f,		0.0f, 1.0f, 0.0f, //1
			-0.125f, 0.375f, 0.125f,	0.0625f, 0.6875f,	0.0f, 1.0f, 0.0f, //2
			0.125f, 0.375f, 0.125f,		0.125f, 0.5f,		0.0f, 1.0f, 0.0f, //3

			//izq
			-0.125f, 0.375f, -0.125f,	0.0f, 0.6875f,		-1.0f, 0.0f, 0.0f, //0
			-0.125f, 0.375f, 0.125f,	0.0625f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.125f, -0.375f, -0.125f,	0.0f, 0.5f,			-1.0f, 0.0f, 0.0f, //4

			-0.125f, 0.375f, 0.125f,	0.0625f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.125f, -0.375f, -0.125f,	0.0f, 0.5f,			-1.0f, 0.0f, 0.0f, //4
			-0.125f, -0.375f, 0.125f,	0.0625f, 0.5f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.125f, 0.375f, -0.125f,	0.125f, 0.6875f,	0.0f, 0.0f, -1.0f, //0
			0.125f, 0.375f, -0.125f,	0.1875f, 0.6875f,	0.0f, 0.0f, -1.0f, //1
			0.125f, -0.375f, -0.125f,	0.1875f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			-0.125f, 0.375f, -0.125f,	0.125f, 0.6875f,	0.0f, 1.0f, -1.0f, //0
			-0.125f, -0.375f, -0.125f,	0.125f, 0.5f,		-1.0f, 0.0f, -1.0f, //4
			0.125f, -0.375f, -0.125f,	0.1875f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.125f, 0.375f, -0.125f,	0.1875f, 0.6875f,	0.0f, 0.0f, 1.0f, //1
			0.125f, 0.375f, 0.125f,		0.25f, 0.6875f,		0.0f, 0.0f, 1.0f, //3
			0.125f, -0.375f, -0.125f,	0.1875f, 0.5f,		0.0f, 0.0f, 1.0f, //5

			0.125f, 0.375f, 0.125f,		0.25f, 0.6875f,		0.0f, 0.0f, 1.0f, //3
			0.125f, -0.375f, -0.125f,	0.1875f, 0.5f,		0.0f, 0.0f, 1.0f, //5
			0.125f, -0.375f, 0.125f,	0.25f, 0.5f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.125f, -0.375f, -0.125f,	0.125f, 0.75f,		0.0f, 0.0f, 1.0f, //4
			0.125f, -0.375f, -0.125f,	0.1875f, 0.75f,		0.0f, 0.0f, 1.0f, //5
			-0.125f, -0.375f, 0.125f,	0.125f, 0.6875f,	0.0f, 0.0f, 1.0f, //6

			0.125f, -0.375f, -0.125f,	0.1875f, 0.75f,		0.0f, 0.0f, 0.0f, //5
			-0.125f, -0.375f, 0.125f,	0.125f, 0.6875f,	0.0f, 0.0f, 0.0f, //6
			0.125f, -0.375f, 0.125f,	0.1875f, 0.6875f,	0.0f, 0.0f, 0.0f, //7

			//frente
			-0.125f, 0.375f, 0.125f,	0.0625f, 0.6875f,	0.0f, 0.0f, 1.0f, //2
			0.125f, 0.375f, 0.125f,		0.125f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.125f, -0.375f, 0.125f,	0.0625f, 0.5f,		0.0f, 0.0f, 1.0f, //6

			0.125f, 0.375f, 0.125f,		0.125f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.125f, -0.375f, 0.125f,	0.0625f, 0.5f,		0.0f, 0.0f, 1.0f, //6
			0.125f, -0.375f, 0.125f,	0.125f, 0.5f,		0.0f, 0.0f, 1.0f, //7
	};

	GLfloat verticesTorso2[] = {
		//	x      y      z				u	  v				nx	  ny    nz
			//arriba
			-0.25f, 0.375f, -0.125f,	0.4375f, 0.75f,		0.0f, 1.0f, 0.0f, //0
			0.25f, 0.375f, -0.125f,		0.5625f, 0.75f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.375f, 0.125f,		0.4375f, 0.6875f,	0.0f, 1.0f, 0.0f, //2

			0.25f, 0.375f, -0.125f,		0.5625f, 0.75f,		0.0f, 1.0f, 0.0f, //1
			-0.25f, 0.375f, 0.125f,		0.4375f, 0.6875f,	0.0f, 1.0f, 0.0f, //2
			0.25f, 0.375f, 0.125f,		0.5625f, 0.6875f,	0.0f, 1.0f, 0.0f, //3

			//izq
			-0.25f, 0.375f, -0.125f,	0.25f, 0.6875f,		-1.0f, 0.0f, 0.0f, //0
			-0.25f, 0.375f, 0.125f,		0.3125f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.375f, -0.125f,	0.25f, 0.5f,		-1.0f, 0.0f, 0.0f, //4

			-0.25f, 0.375f, 0.125f,		0.3125f, 0.6875f,	-1.0f, 0.0f, 0.0f, //2
			-0.25f, -0.375f, -0.125f,	0.25f, 0.5f,		-1.0f, 0.0f, 0.0f, //4
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.5f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.25f, 0.375f, -0.125f,	0.4375f, 0.6875f,	0.0f, 0.0f, -1.0f, //0
			0.25f, 0.375f, -0.125f,		0.5625f, 0.6875f,	0.0f, 0.0f, -1.0f, //1
			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			-0.25f, 0.375f, -0.125f,	0.4375f, 0.6875f,	0.0f, 1.0f, -1.0f, //0
			-0.25f, -0.375f, -0.125f,	0.4375f, 0.5f,		-1.0f, 0.0f, -1.0f, //4
			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.25f, 0.375f, -0.125f,		0.5625f, 0.6875f,	0.0f, 0.0f, 1.0f, //1
			0.25f, 0.375f, 0.125f,		0.625f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, 1.0f, //5

			0.25f, 0.375f, 0.125f,		0.625f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			0.25f, -0.375f, -0.125f,	0.5625f, 0.5f,		0.0f, 0.0f, 1.0f, //5
			0.25f, -0.375f, 0.125f,		0.625f, 0.5f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.25f, -0.375f, -0.125f,	0.4375f, 0.75f,		0.0f, 0.0f, 1.0f, //4
			0.25f, -0.375f, -0.125f,	0.5625f, 0.75f,		0.0f, 0.0f, 1.0f, //5
			-0.25f, -0.375f, 0.125f,	0.4375f, 0.6875f,	0.0f, 0.0f, 1.0f, //6

			0.25f, -0.375f, -0.125f,	0.5625f, 0.75f,		0.0f, 0.0f, 0.0f, //5
			-0.25f, -0.375f, 0.125f,	0.4375f, 0.6875f,	0.0f, 0.0f, 0.0f, //6
			0.25f, -0.375f, 0.125f,		0.5625f, 0.6875f,	0.0f, 0.0f, 0.0f, //7

			//frente
			-0.25f, 0.375f, 0.125f,		0.3125f, 0.6875f,	0.0f, 0.0f, 1.0f, //2
			0.25f, 0.375f, 0.125f,		0.4375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.5f,		0.0f, 0.0f, 1.0f, //6

			0.25f, 0.375f, 0.125f,		0.4375f, 0.6875f,	0.0f, 0.0f, 1.0f, //3
			-0.25f, -0.375f, 0.125f,	0.3125f, 0.5f,		0.0f, 0.0f, 1.0f, //6
			0.25f, -0.375f, 0.125f,		0.4375f, 0.5f,		0.0f, 0.0f, 1.0f, //7
	};

	/*
	GLfloat verticesBrazoDer[] = {
		//	x      y      z				u	  v				nx	  ny    nz
			//arriba
			-0.1f, 0.375f, -0.125f,		0.4375f, 0.5f,		0.0f, 1.0f, 0.0f, //0
			0.1f, 0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 1.0f, 0.0f, //1
			-0.1f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 1.0f, 0.0f, //2

			0.1f, 0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 1.0f, 0.0f, //1
			-0.1f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 1.0f, 0.0f, //2
			0.1f, 0.375f, 0.125f,		0.5625f, 0.4375f,	0.0f, 1.0f, 0.0f, //3

			//izq
			-0.1f, 0.375f, -0.125f,		0.25f, 0.4375f,		-1.0f, 0.0f, 0.0f, //0
			-0.1f, 0.375f, 0.125f,		0.3125f, 0.4375f,	-1.0f, 0.0f, 0.0f, //2
			-0.1f, -0.375f, -0.125f,	0.25f, 0.25f,		-1.0f, 0.0f, 0.0f, //4

			-0.1f, 0.375f, 0.125f,		0.3125f, 0.4375f,	-1.0f, 0.0f, 0.0f, //2
			-0.1f, -0.375f, -0.125f,	0.25f, 0.25f,		-1.0f, 0.0f, 0.0f, //4
			-0.1f, -0.375f, 0.125f,		0.3125f, 0.25f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.1f, 0.375f, -0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, -1.0f, //0
			0.1f, 0.375f, -0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, -1.0f, //1
			0.1f, -0.375f, -0.125f,		0.5625f, 0.25f,		0.0f, 0.0f, -1.0f, //5

			-0.1f, 0.375f, -0.125f,		0.4375f, 0.4375f,	0.0f, 1.0f, -1.0f, //0
			-0.1f, -0.375f, -0.125f,	0.4375f, 0.25f,		-1.0f, 0.0f, -1.0f, //4
			0.1f, -0.375f, -0.125f,		0.5625f, 0.25f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.1f, 0.375f, -0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, 1.0f, //1
			0.1f, 0.375f, 0.125f,		0.625f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			0.1f, -0.375f, -0.125f,		0.5625f, 0.25f,		0.0f, 0.0f, 1.0f, //5

			0.1f, 0.375f, 0.125f,		0.625f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			0.1f, -0.375f, -0.125f,		0.5625f, 0.25f,		0.0f, 0.0f, 1.0f, //5
			0.1f, -0.375f, 0.125f,		0.625f, 0.25f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.1f, -0.375f, -0.125f,	0.4375f, 0.5f,		0.0f, 0.0f, 1.0f, //4
			0.1f, -0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 0.0f, 1.0f, //5
			-0.1f, -0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //6

			0.1f, -0.375f, -0.125f,		0.5625f, 0.5f,		0.0f, 0.0f, 0.0f, //5
			-0.1f, -0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 0.0f, //6
			0.1f, -0.375f, 0.125f,		0.5625f, 0.4375f,	0.0f, 0.0f, 0.0f, //7

			//frente
			-0.1f, 0.375f, 0.125f,		0.3125f, 0.4375f,	0.0f, 0.0f, 1.0f, //2
			0.1f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			-0.1f, -0.375f, 0.125f,		0.3125f, 0.25f,		0.0f, 0.0f, 1.0f, //6

			0.1f, 0.375f, 0.125f,		0.4375f, 0.4375f,	0.0f, 0.0f, 1.0f, //3
			-0.1f, -0.375f, 0.125f,		0.3125f, 0.25f,		0.0f, 0.0f, 1.0f, //6
			0.1f, -0.375f, 0.125f,		0.4375f, 0.25f,		0.0f, 0.0f, 1.0f, //7
	};

	GLfloat verticesPiernaDer[] = {
		//	x      y      z				u	  v			nx	  ny    nz
			//arriba
			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //0
			0.125f, 0.375f, 0.125f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //1
			-0.125f, 0.375f, -0.125f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //2

			0.125f, 0.375f, 0.125f,		0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //1
			-0.125f, 0.375f, -0.125f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //2
			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 1.0f, 0.0f, //3

			//izq
			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //0
			-0.125f, 0.375f, -0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //2
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //4

			-0.125f, 0.375f, -0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //2
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //4
			-0.125f, -0.375f, -0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, 0.0f, //6

			//atras
			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f, //0
			0.125f, 0.375f, 0.125f,		0.0f, 0.0f,		0.0f, 0.0f, -1.0f, //1
			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f, //5

			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 1.0f, -1.0f, //0
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		-1.0f, 0.0f, -1.0f, //4
			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, -1.0f, //5

			//der
			0.125f, 0.375f, 0.125f,		0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //1
			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //3
			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //5

			-0.125f, 0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //3
			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //5
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //7

			//abajo
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //4
			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //5
			-0.125f, -0.375f, -0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 1.0f, //6

			0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //5
			-0.125f, -0.375f, -0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //6
			-0.125f, -0.375f, 0.125f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f, //7

			//frente
			-0.125f, 0.375f, 0.125f,	0.125f, 0.875f,		0.0f, 0.0f, 1.0f, //2
			0.125f, 0.375f, 0.125f,		0.25f, 0.875f,		0.0f, 0.0f, 1.0f, //3
			-0.125f, -0.375f, 0.125f,	0.125f, 0.75f,		0.0f, 0.0f, 1.0f, //6

			0.125f, 0.375f, 0.125f,		0.25f, 0.875f,		0.0f, 0.0f, 1.0f, //3
			-0.125f, -0.375f, 0.125f,	0.125f, 0.75f,		0.0f, 0.0f, 1.0f, //6
			0.125f, -0.375f, 0.125f,	0.25f, 0.75f,		0.0f, 0.0f, 1.0f, //7
	};
	*/

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(verticesCabeza, indices, 288, 36);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(verticesTorso, indices, 288, 36);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(verticesBrazo, indices, 288, 36);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(verticesPierna, indices, 288, 36);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(verticesTorso2, indices, 288, 36);
	meshList.push_back(obj5);


	/*
	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(verticesBrazoDer, indices, 288, 36);
	meshList.push_back(obj4);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(verticesPiernaDer, indices, 288, 36);
	meshList.push_back(obj6);
	*/
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	//CreateObjects();
	CreatePersonaje();
	sp.init(); //inicializar esfera
	sp.load();//enviar la esfera al shader
	CreateShaders();

	cameraLibre = Camera(glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);
	cameraWheezy = Camera(glm::vec3(0.0f, 24.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 1.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	//WheezyTexture = Texture("Textures/wheezy_textura.png");
	WheezyTexture = Texture("Textures/wheezy_textura.tga");
	WheezyTexture.LoadTextureA();
	FrijolitoTexture = Texture("Textures/frijolito_texture.tga");
	FrijolitoTexture.LoadTextureA();
	JettTexture = Texture("Textures/jett_texture.jpg");
	JettTexture.LoadTextureA();

	StreetMan1_Texture = Texture("Textures/SimplePeople_StreetMan_Black.png");
	StreetMan1_Texture.LoadTexture();
	StreetMan2_Texture = Texture("Textures/SimplePeople_StreetMan_Brown.png");
	StreetMan2_Texture.LoadTexture();
	StreetMan3_Texture = Texture("Textures/SimplePeople_StreetMan_White.png");
	StreetMan3_Texture.LoadTexture();

	//Modelos para el proyecto
	Cuarto_M.LoadModel("Models/cuarto_text.obj");
	Escritorio_M.LoadModel("Models/SM_Prop_Desk_02_OBJ.obj");
	Escritorio2_M.LoadModel("Models/desk_2.obj");
	Escritorio3_M.LoadModel("Models/desk_3.obj");
	Lampara_M.LoadModel("Models/SM_Prop_DeskLamp_05_OBJ.obj");
	Bote_basura_M.LoadModel("Models/SM_Prop_Bin_03_OBJ.obj");
	//Silla_M.LoadModel("Models/SM_Prop_Chair_10_OBJ.obj");
	Silla_M.LoadModel("Models/silla_desk.obj");
	Puff_M.LoadModel("Models/SM_Prop_Chair_BeanBag_03_OBJ.obj");
	SetUp_M.LoadModel("Models/SM_Prop_Computer_Setup_02_OBJ.obj");
	//Gabinete_M.LoadModel("Models/SM_Prop_Computer_Tower_Modern_01_OBJ.obj");
	HeadSet_M.LoadModel("Models/SM_Prop_Headset_02_OBJ.obj");
	LuzTecho_M.LoadModel("Models/SM_Prop_Light_07_OBJ.obj");
	JettCompleta.LoadModel("Models/JettCompleta.obj");
	Cama_M.LoadModel("Models/bed_red.obj");
	Alfombra_M.LoadModel("Models/alfombra.obj");

	//Wheezy
	Wheezy_torso_M.LoadModel("Models/wheezy_torso.obj");
	Wheezy_cabeza_M.LoadModel("Models/wheezy_cabeza.obj");
	Wheezy_brazo_izq_M.LoadModel("Models/wheezy_brazo_izq.obj");
	Wheezy_brazo_der_M.LoadModel("Models/wheezy_brazo_der.obj");
	Wheezy_pie_izq_M.LoadModel("Models/wheezy_pie_izq.obj");
	Wheezy_pie_der_M.LoadModel("Models/wheezy_pie_der.obj");

	ML_Ring_M.LoadModel("Models/ring_texturizado.obj");
	Luchador_M.LoadModel("Models/Characters/SK_Character_Streetman.fbx");

	//Modelos_MuchaLucha* frijolito = new Modelos_MuchaLucha();

	// TODO: agreagar doble textura para que haya uno de día y uno de noche

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //color
		0.1f, 0.3f, // 0.5, 0.3 mas iluminado (AMBIENTAL, DIFUSA)
		0.0f, 0.0f, -1.0f); //(DESDE DONDE ILUMINA)
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual

	//LUZ DE LAMPARA DE TECHO 1
	pointLightsNoche[0] = PointLight(1.0f, 1.0f, 1.0f, //color 
		0.6f, 1.0f, //ambiente, difusa
		0.0f, 195.0f, 4.5f, // posicion
		0.1f, 0.01f, 0.0f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount++;

	//LUZ DE LAMPARA DE TECHO 2
	pointLightsNoche[1] = PointLight(1.0f, 1.0f, 1.0f, //color 
		0.9f, 0.1f, //ambiente, difusa
		0.0f, 195.0f, -297.0f, // posicion
		0.1f, 0.01f, 0.0f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount++;

	//LUZ DE LA LAMPARA DE ESCRITORIO ARREGLO DE SPOTLIGHTS 0
	unsigned int spotLightCount = 0;
	spotLights[spotLightCount] = SpotLight(1.0f, 0.5f, 0.0f, //COLOR
		0.0f, 2.0f, //AMBIENTE, DIFUSA
		50.0f, 125.0f, -360.0f, //POSICION
		0.0f, -1.0f, 1.0f, //DIRECCION
		0.7f, 0.01f, 0.0001f, //C,B,A: ECUACION DE SEGUNDO GRADO
		15.0f); //ANGULO DE APERTURA
	spotLightCount++;

	unsigned int pointLightCount1 = 0;
	//LUZ DE LEDS DE LA CAMA
	pointLightsCama[0] = PointLight(1.0f, 0.0f, 0.0f, //color 
		0.6f, 1.0f, //ambiente, difusa
		-150.0f, 20.0f, 40.0f, // posicion
		0.5f, 0.01f, 0.001f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount1++;

	//LUZ DE LEDS DEL ESCRITORIO
	pointLightsCama[1] = PointLight(1.0f, 0.0f, 0.0f, //color 
		0.6f, 1.0f, //ambiente, difusa
		0.0f, 60.0f, -375.0f, // posicion
		0.5f, 0.01f, 0.001f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount1++;

	//LUZ DE LAMPARA DE TECHO 1
	pointLightsCama[2] = PointLight(1.0f, 1.0f, 1.0f, //color 
		0.6f, 1.0f, //ambiente, difusa
		0.0f, 195.0f, 4.5f, // posicion
		0.1f, 0.01f, 0.0f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount1++;

	//LUZ DE LAMPARA DE TECHO 2
	pointLightsCama[3] = PointLight(1.0f, 1.0f, 1.0f, //color 
		0.9f, 0.1f, //ambiente, difusa
		0.0f, 195.0f, -297.0f, // posicion
		0.1f, 0.01f, 0.0f); // ecuaci�n de segundo grado 
	//  c,	b ,	 a
		//sqrt(b^2 -4ac)
	// para no dar una raiz comlejo
	pointLightCount1++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	float var1 = 0.0f;
	contadorDiaNoche = 0.0f;
	posicionLedX = -150.0f;
	posicionLedZ = 40.0f;
	posicionLed1X = 0.0f;
	posicionLed1Z = -374.0f;
	banderaLedCama = 0;
	banderaLedEscritorio = 0;
	bool isWheezyCam = false;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

//	Aquí se debe modificar, para hacer una condición en donde se modifique la cámara y se cambie a la que
//	puede ser fija o se vaya moviendo con el personaje
//  Se puede modificar la camara o crear dos y escoger cual se le pasa al shader

		isWheezyCam = mainWindow.getCamaraVal();

		//Recibir eventos del usuario
		glfwPollEvents();
		if (isWheezyCam) {
			camera = &cameraWheezy;
			camera->keyControl(mainWindow.getsKeys(), deltaTime);
			camera->mouseControl(mainWindow.getXChange(), 0.0f);
		}
		else {
			camera = &cameraLibre;
			camera->keyControl(mainWindow.getsKeys(), deltaTime);
			camera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera->calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		//Cambio del ciclo de luz de dia y noche_____________________________________
		
		if (contadorDiaNoche <= 1.0f and dia) {
			contadorDiaNoche += deltaTime * 0.0001f;
		}
		else if (contadorDiaNoche >= 1.0f and dia) {
			dia = false;
		}
		else if (contadorDiaNoche >= 0.0f and !dia) {
			contadorDiaNoche -= deltaTime * 0.0001f;
		}
		else if (contadorDiaNoche <= 0.0f and !dia) {
			dia = true;
		}
		//printf("\nContador = %f", contadorDiaNoche);
		mainLight.ChangeDiffuseAmbient(contadorDiaNoche, 0.3);

		//Movimiento del led de la cama ______________________________________________
		if (posicionLedX >= -248.0f and banderaLedCama == 0) {
			posicionLedX -= deltaTime * 0.5f;
			//printf("\nposicionLedX = %f", posicionLedX);
		}
		else if (posicionLedX <= -248.0f and banderaLedCama == 0) {
			banderaLedCama = 1;
		}
		else if (posicionLedX <= -45.0f and banderaLedCama == 1) {
			posicionLedX += deltaTime * 0.5f;
			//printf("\nposicionLedX = %f", posicionLedX);
		}
		else if (posicionLedX >= -45.0f and banderaLedCama == 1) {
			banderaLedCama = 2;
		}
		else if (posicionLedZ <= 140.0f and banderaLedCama == 2) {
			posicionLedZ += deltaTime * 0.5f;
			//printf("\nposicionLedZ = %f", posicionLedZ);
		}
		else if (posicionLedZ >= 140.0f and banderaLedCama == 2) {
			banderaLedCama = 3;
		}
		else if (posicionLedZ >= 40.0f and banderaLedCama == 3) {
			posicionLedZ -= deltaTime * 0.5f;
			//printf("\nposicionLedZ = %f", posicionLedZ);
		}
		else if (posicionLedZ <= 40.0f and banderaLedCama == 3) {
			banderaLedCama = 0;
		}
		pointLightsCama[0].SetPos(glm::vec3(posicionLedX, 20.0f, posicionLedZ));
		
		
		//Movimiento del led deL ESCRITORIO ______________________________________________
		if (posicionLed1X <= 150.0f and banderaLedEscritorio == 0) {
			posicionLed1X += deltaTime * 1.0f;
		}
		else if (posicionLed1X >= 150.0f and banderaLedEscritorio == 0) {
			banderaLedEscritorio = 1;
		}
		else if (posicionLed1X >= 0.0f and banderaLedEscritorio == 1) {
			posicionLed1X -= deltaTime * 1.0f;
		}
		else if (posicionLed1X <= 0.0f and banderaLedEscritorio == 1) {
			banderaLedEscritorio = 0;
		}

		pointLightsCama[1].SetPos(glm::vec3(posicionLed1X, 60.0f, posicionLed1Z));

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelaux_body(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Cuarto
		//color = glm::vec3(1.0f, 1.0f, 1.0f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(50.0f, 50.0f, 50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Cuarto_M.RenderModel();

		

		//CAMA
		//color = glm::vec3(0.705f, 0.705f, 0.105f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 1.0f, 90.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Cama_M.RenderModel();

		//Alfombra
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -100.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Alfombra_M.RenderModel();

		//Escritorio2
		//color = glm::vec3(0.705f, 0.705f, 0.105f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-150.0f, 1.0f, -330.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Escritorio2_M.RenderModel();

		//Escritorio3
		//color = glm::vec3(0.705f, 0.705f, 0.105f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(210.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Escritorio3_M.RenderModel();

		//Escritorio
		//color = glm::vec3(0.705f, 0.705f, 0.105f);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 1.0f, -317.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Escritorio_M.RenderModel();

		//Lampara de escritorio
		//color = glm::vec3(0.705f, 0.705f, 0.705f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 52.0f, -50.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Lampara_M.RenderModel();

		//Bote de basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 0.0f, -250.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Bote_basura_M.RenderModel();

		//Silla Gamer
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 0.0f, -270.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Silla_M.RenderModel();

		//Puff
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(40.0f, 0.0f, 80.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Puff_M.RenderModel();

		//SetUp
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(120.0f, 64.0f, -350.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SetUp_M.RenderModel();

		//Gabinete
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, 64.0f, -350.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Gabinete_M.RenderModel();

		//HeadSet
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(150.0f, 64.0f, -350.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		HeadSet_M.RenderModel();

		//Luz de techo 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 215.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LuzTecho_M.RenderModel();

		//Luz de techo 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 215.0f, -300.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LuzTecho_M.RenderModel();
	
	// TOY STORY
	  //WHEEZY
		
		//posicion de la camara Wheezy
		glm::vec3 poscam = cameraWheezy.getCameraPosition();
		//direccion de la camara Wheezy
		glm::vec3 dircam = cameraWheezy.getCameraDirection();
		float angulo_cam = atan(dircam.z / dircam.x);
		angulo_cam += (90 * toRadians);
		if (dircam.x > 0) {
			angulo_cam += (180 * toRadians);
		}
		//printf("\n angulo de camara: %.2f", angulo_cam / toRadians);
		glm::vec3 poswheezy = glm::vec3(0, 0, 0);
		poswheezy.x = poscam.x + ( ( 0*cos(angulo_cam) ) - (40*sin(angulo_cam)) );
		poswheezy.z = poscam.z + ( ( 0*sin(angulo_cam) ) + (40*cos(angulo_cam)) );
		poswheezy.y = poscam.y - 10;
		//Torso
		model = glm::mat4(1.0);
		model = glm::translate(model, poswheezy);
		model = glm::rotate(model, -angulo_cam, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux_body = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_torso_M.RenderModel();

		//Articulacion pie der
		model = modelaux_body;
		model = glm::translate(model, glm::vec3(-1.7f, -5.0f, 0.0f));
		model = glm::rotate(model, -40 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		//Pie der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_pie_der_M.RenderModel();

		//articulacion pie izq
		model = modelaux_body;
		model = glm::translate(model, glm::vec3(1.6f, -5.0f, 0.0f));
		model = glm::rotate(model,  10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		//pie izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_pie_izq_M.RenderModel();

		//articulacion brazo der
		model = modelaux_body;
		model = glm::translate(model, glm::vec3(-4.6f, 4.0f, 0.0f));
		model = glm::rotate(model, 0 * var1++ * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		//brazo der
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_brazo_der_M.RenderModel();

		//articulacion brazo izq
		model = modelaux_body;
		model = glm::translate(model, glm::vec3(4.0f, 4.0f, 0.0f));
		model = glm::rotate(model, 0 * var1++ * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		modelaux = model;

		//brazo izq
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_brazo_der_M.RenderModel();

		//articulacion cabeza
		model = modelaux_body;
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 0.0f));
		model = glm::rotate(model, 0 * var1++ * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;

		//cabeza
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Wheezy_cabeza_M.RenderModel();

		//PELOTA
		color = glm::vec3(248.0f/255.0f, 228.0f / 255.0f, 46.0f / 255.0f);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 10.0f, -20.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		plainTexture.UseTexture();
		sp.render(); //esfera
			//franja azul
		color = glm::vec3(29.0f / 255.0f, 68.0f / 255.0f, 135.0f / 255.0f);
		model = modelaux;
		model = glm::scale(model, glm::vec3(1.04f, 1.04f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		plainTexture.UseTexture();
		sp.render(); //esfera
			//estrellas rojas
		color = glm::vec3(212.0f / 255.0f, 62.0f / 255.0f, 44.0f / 255.0f);
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -4.4f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		plainTexture.UseTexture();
		meshList[0]->RenderMesh();


	//	M U C H A  L U C H A
		color = glm::vec3(1.0f,1.0f,1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		glm::mat4 auxML(1.0);

			//ring
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 63.5f, -230.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		auxML = model;
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ML_Ring_M.RenderModel();


	//  VALORATN ----------- VALORANT
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(20.0f, 70.0f, -230.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettCompleta.RenderModel();
		//auxML = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		
		//frijolito->RenderModels(uniformColor, uniformModel);

		//Luchador 1
		model = auxML;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StreetMan1_Texture.UseTexture();
		Luchador_M.RenderModel();

		//Luchador 2
		model = auxML;
		model = glm::translate(model, glm::vec3(20.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StreetMan2_Texture.UseTexture();
		Luchador_M.RenderModel();

		//Luchador 3
		model = auxML;
		model = glm::translate(model, glm::vec3(-20.0f, 0.0f, 20.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		StreetMan3_Texture.UseTexture();
		Luchador_M.RenderModel();

//	Personaje Frijolito
		glm::mat4 auxPersonaje(1.0);
 
		//Torso
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 50.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		auxPersonaje = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[1]->RenderMesh();

		//Cabeza
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.0f, 0.375f, 0.0f));
		model = glm::rotate(model, 20 * toRadians, glm::vec3(1.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[0]->RenderMesh();

		//Brazo izq
		model = glm::mat4(1.0);
		model = auxPersonaje;
		//se traslada lo necesario para el hombro
		model = glm::translate(model, glm::vec3(-0.25f, 0.0f, 0.0f));
		//se desplaza la distancia equivalente al cateto opuesto (distancia inferior) al realizar la rotación
		model = glm::translate(model, glm::vec3(-tan(10*toRadians)*0.375f, 0.0f, 0.0f));
		//se realiza la rotación
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, -1.0f));
		//se desplaza lo necesario para llegar al centro del objeto (la mitad de su ancho)
		model = glm::translate(model, glm::vec3(-0.1f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Brazo der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.25f, 0.0, 0.0f));
		model = glm::translate(model, glm::vec3(tan(10 * toRadians) * 0.375f, 0.0f, 0.0f));
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.1f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Pierna izq
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(-0.125f, -0.75f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -tan(15 * toRadians) * 0.25f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.f, -0.125f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[3]->RenderMesh();

		//Pierna der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.125f, -0.75f, 0.0f));
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, tan(15 * toRadians) * 0.25f));
		model = glm::rotate(model, 15 * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		FrijolitoTexture.UseTexture();
		meshList[3]->RenderMesh();

//	Personaje Jett
		
		//Torso (utiliza torso 2)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 50.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		auxPersonaje = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Cabeza
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.0f, 0.625f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[0]->RenderMesh();

		//Brazo izq
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(-0.35f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Brazo der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Pierna izq
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(-0.125f, -0.75f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[3]->RenderMesh();

		//Pierna der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.125f, -0.75f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		JettTexture.UseTexture();
		meshList[3]->RenderMesh();


		//informaci�n al shader de fuentes de iluminaci�n
		//informaci�n al shader de fuentes de iluminaci�n

//	Personaje Wheezy

		//Torso (utiliza torso 2)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.0f, 50.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		auxPersonaje = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[4]->RenderMesh();

		//Cabeza
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.0f, 0.625f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[0]->RenderMesh();

		//Brazo izq
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(-0.35f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Brazo der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[2]->RenderMesh();

		//Pierna izq
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(-0.125f, -0.75f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[3]->RenderMesh();

		//Pierna der
		model = auxPersonaje;
		model = glm::translate(model, glm::vec3(0.125f, -0.75f, 0.0f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		WheezyTexture.UseTexture();
		meshList[3]->RenderMesh();


		//informaci�n al shader de fuentes de iluminaci�n
		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		LedCama = mainWindow.getLedCama();
		if (contadorDiaNoche <= 0.7f) {
			//printf("\nLedCama = %d", LedCama);
			if(LedCama == 1)
				shaderList[0].SetPointLights(pointLightsCama, pointLightCount1);
			else
				shaderList[0].SetPointLights(pointLightsNoche, pointLightCount);

			shaderList[0].SetSpotLights(spotLights, spotLightCount);
		}
		else {
			if (LedCama == 1)
				shaderList[0].SetPointLights(pointLightsCama, pointLightCount1 - 2);
			else
				shaderList[0].SetPointLights(pointLightsNoche, pointLightCount - 2);

			shaderList[0].SetSpotLights(spotLights, spotLightCount - 1);
		}
		

		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}
