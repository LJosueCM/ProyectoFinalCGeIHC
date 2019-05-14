
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"
#include"Model.h"
#include "Skybox.h"
#include"SpotLight.h"

//para audio
#include <iostream>
#include <irrKlang.h>


using namespace irrklang;
const float toRadians = 3.14159265f / 180.0f;
float movOffset;
bool avanza, sube = 1, kilauea = 0;
float altitud = 0.0f, giro = 0.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;



Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
Texture Hojas1;
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;


/************************************Declaracion de las luces de ambos tipos*********************************/
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

Model Kitt_M;
Model Llanta_M;
Model Camino_M;
Model Blackhawk_M;
Model street_Lamp;
Model Iron_door;
Model road;
Model wall;
Model tree;
Model stone;
Model RoadsFair;
Model Bath;
Model Kart;
Model BaseTree;
Model Hojas;
Model Tree;
Model Globos;
Model Globo;
Model HotDog;
Model Coke;
Model Cotton;
Model Corn;
Model Kilahuea;
Model base_kilahuea;
Model BaseNoria;
Model RuedaNoria;
Model CabinaNoria;
Model Pan;
Model Duck;
Model Mesa;
Model Park;
Model Ice_Cream;
Model PopCorn;

Skybox skybox;
Skybox skybox2;
Skybox skybox3;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int cont = 0;
float hora = 0.0;
int camara1 = 1, camara2 = 0;
float mov_x_pato = 0, mov_y_pato = 0;
int apagarS1 = 0, apagarS2 = 0, apagarP1 = 0, apagarP2 = 0, luces_entrada = 0, globo = 0, trigger_globo = 0;
float distancia_luz1 = -8.0f, distancia_luz2 = -1.5f,  distanca_Luz1P = 9.5f, distancia_Luz2P = -1.5f,aumenta_globo = 0 ;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";
//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount,
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
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-30.0f, 0.0f, -30.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, -30.0f,	30.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-30.0f, 0.0f, 30.0f,	0.0f, 30.0f,	0.0f, -1.0f, 0.0f,
		30.0f, 0.0f, 30.0f,		30.0f, 30.0f,	0.0f, -1.0f, 0.0f
	};

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


	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh *obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	ISoundEngine* engine = createIrrKlangDevice();

	if (!engine)
		return 0;

	/*************************************SONIDO MP3 *****************************************/

	engine->play2D("Audio/lon-lon-ranch.mp3", true);


	plainTexture = Texture("Textures/pasto.png");
	plainTexture.LoadTextureA();
	Hojas1 = Texture("Textures/leafs1.tga");
	Hojas1.LoadTextureA();
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt.3ds");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/k_rueda.3ds");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");
	Camino_M = Model();
	Camino_M.LoadModel("Models/railroad track.obj");

	//LUMINARIA
	street_Lamp = Model();
	street_Lamp.LoadModel("Models/StreetLamp.obj");
	RoadsFair = Model();
	RoadsFair.LoadModel("Models/RoadsFair.obj");
	stone = Model();
	stone.LoadModel("Models/CobbleStones2.obj");

	//Vegetacion
	tree = Model();
	tree.LoadModel("Models/tree4a_lod1.obj");

	//Carretaera
	road = Model();
	road.LoadModel("Models/road.obj");
	wall = Model();
	wall.LoadModel("Models/wall.obj");

	//Kart
	Kart = Model();
	Kart.LoadModel("Models/kart.obj");

	//noria
	BaseNoria = Model();
	BaseNoria.LoadModel("Models/base_noria.obj");
	RuedaNoria = Model();
	RuedaNoria.LoadModel("Models/rueda.obj");
	CabinaNoria = Model();
	CabinaNoria.LoadModel("Models/cabinas.obj");

	//Kilahue
	Kilahuea = Model();
	Kilahuea.LoadModel("Models/base.obj");
	base_kilahuea = Model();
	base_kilahuea.LoadModel("Models/cabina.obj ");

	//Carrito de globos
	Globos = Model();
	Globos.LoadModel("Models/balloon.obj");

	//carrito de hotdogs
	HotDog = Model();
	HotDog.LoadModel("Models/hotdog.obj");

	//carrito de algodon de azucar
	Cotton = Model();
	Cotton.LoadModel("Models/CottonCandy.obj");

	//Puesto de elotes
	Corn = Model();
	Corn.LoadModel("Models/elotes.obj");

	//Maquina de refrescos
	Coke = Model();
	Coke.LoadModel("Models/coke.obj");

	//arbol
	BaseTree = Model();
	BaseTree.LoadModel("Models/basetree.obj");
	Hojas = Model();
	Hojas.LoadModel("Models/hojas.obj");

	//Bath
	Bath = Model();
	Bath.LoadModel("Models/bathroom.obj");

	//Camion pan de feria
	Pan = Model();
	Pan.LoadModel("Models/truck.obj");

	//Mesa picnic
	Mesa = Model();
	Mesa.LoadModel("Models/mesa.obj");

	//globo
	Globo = Model();
	Globo.LoadModel("Models/globo_1.obj");

	Duck = Model();
	Duck.LoadModel("Models/10602_Rubber_Duck_v1_L3.obj");

	//Parque
	Park = Model();
	Park.LoadModel("Models/park.obj");

	//Helado
	Ice_Cream = Model();
	Ice_Cream.LoadModel("Models/helado.obj");

	//Palomitas chidas
	PopCorn = Model();
	PopCorn.LoadModel("Models/Puesto_palomitas.obj");

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);



	//skybox para dia
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/hills_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_up.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/hills_ft.tga");
	skybox = Skybox(skyboxFaces);
	//skybox para amanecer o atardecer
	std::vector<std::string> skyboxFaces2;
	skyboxFaces2.push_back("Textures/Skybox/hills_rt_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_lf_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_dn_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_up_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_bk_night.tga");
	skyboxFaces2.push_back("Textures/Skybox/hills_ft_night.tga");
	skybox2 = Skybox(skyboxFaces2);
	//skybox para noche
	std::vector<std::string> skyboxFaces3;
	skyboxFaces3.push_back("Textures/Skybox/hills_rt_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_lf_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_dn_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_up_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_bk_noche1.tga");
	skyboxFaces3.push_back("Textures/Skybox/hills_ft_noche1.tga");
	skybox3 = Skybox(skyboxFaces3);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	movOffset = 1.0f;
	avanza = 1;

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;


		unsigned int spotLightCount = 0;
		unsigned int pointLightCount = 0;

		//Recibir eventos del usuario
		glfwPollEvents();

	

		//contador para cambiar de hora cada cierto tiempo
		//de esta forma cambiamos el skybox de acuerdo a la hora del dia.
		while (cont != 500000000) {
			cont += 1;
		}
		if (cont == 500000000) {
			cont = 0;
			hora += 0.008;//si se quiere aumentar o disminuir la velocidad del cambio de hora, se modifica esta variable
			if (hora >= 24.0) {//reinicia el contador al cumplir las 24 horas
				hora = 0.0;
			}
		}


		//Camara libre :v
		if (camara1 == 1)
		{
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (camara2 == 1)
		{


			camera.keyControlAerea(mainWindow.getsKeys(), deltaTime);
			camera.mouseControlAerea(0, 0);

		}




		/****************************************** LUCES SPOTLIGHT**********************************************/

		

		spotLights[1] = SpotLight(1.0f, 1.0f, 1.0f, //vector de color
			0.0f, 2.0f,
			distancia_luz1, 24.0f, -30.0f, //posicion
			0.0f, -15.0f, 0.0f, //direccion
			1.0f, 0.0f, 0.0f,
			12.0f); //Angulo de apertura
		spotLightCount++;


		spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f, //vector de color
			0.0f, 2.0f,
			distancia_luz2, 24.0f, -30.0f, //posicion
			0.0f, -15.0f, 0.0f, //direccion
			1.0f, 0.0f, 0.0f,
			12.0f); //Angulo de apertura
		spotLightCount++;


		spotLights[3] = SpotLight(1.0f, 1.0f, 1.0f, //vector de color
			0.0f, 2.0f,
			-2.0f, 0.0f, -1.0f, //posicion
			0.0f, -15.0f, 0.0f, //direccion
			1.0f, 0.0f, 0.0f,
			12.0f); //Angulo de apertura
		spotLightCount++;

		/*******************************Luces POINTLIGHT*****************************************************/

		pointLights[0] =PointLight(1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,
			distanca_Luz1P, 0.90f, -23.0f, //estas son las coordenadas
			0.1f, 0.1f, 0.1f);
		pointLightCount++;


		pointLights[1] = PointLight(1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,
			distancia_Luz2P, 0.90f, -10.0f, //estas son las coordenadas
			0.1f, 0.1f ,0.1f);
		pointLightCount++;
	
		pointLights[2] = PointLight(1.0f, 1.0f, 1.0f,
			1.0f, 1.0f,
			12.0f, 0.9f, -2.5f, //estas son las coordenadas
			0.1f, 0.1f, 0.1f);
		pointLightCount++;

		//Luces de la entrada

		if(luces_entrada == 1)
		{ 
			pointLights[3] = PointLight(0.0f, 0.0f, 0.0f,
				1.0f, 1.0f,
				-1.4, 0.9f, -1.0f, //estas son las coordenadas
				0.1f, 0.1f, 0.1f);
			pointLightCount++;

			pointLights[4] = PointLight(0.0f, 0.0f, 0.0f,
				1.0f, 1.0f,
				2.9f, 0.9f, -1.0f, //estas son las coordenadas
				0.1f, 0.1f, 0.1f);
			pointLightCount++;

		}



		/********************************APAGAR LUCES SPOTLIGHT********************************/

		if (apagarS1 == 1) {
			distancia_luz1 = -50.0f;
		}
		else if (apagarS1 == 2)
			distancia_luz1 = -8.0f;

		if (apagarS2 == 1) {
			distancia_luz2 = 50.0f;
		}
		else if (apagarS2 == 2)
		{
			distancia_luz2 = -1.5f ;
		}
		/**************************************APAGAR LUCES POINTLIGHT*************************************************/

		if (apagarP1 == 1) {
			distanca_Luz1P = -80.0f;
		}
		else if (apagarP1 == 2)
		{ 
			distanca_Luz1P = 9.5f;
		}
		
		if (apagarP2 == 1) {
			distancia_Luz2P = -80.0f;
		}
		else if (apagarP2 == 2)
		{
			distancia_Luz2P = -1.5f;
		}

		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Cambia skybox de acuerdo a la hora del dia
		//dia
		if (hora >= 0.0 && hora <= 8.0) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			luces_entrada = 0;
		}
		//amanecer o atardecer
		else if ((hora > 8.0 && hora <= 10.0) || (hora > 21.0 && hora <= 24.0)) {
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//noche
		else if (hora > 10.0 && hora <= 21.0) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
			luces_entrada = 1;
		}

		//movimiento del kilauea
		if (kilauea == 1) {
			if (sube == 1) {
				altitud += 0.05f;
				if (altitud >= 16.0f) { sube = 0; }
			}
			else {
				altitud -= 0.05f;
				if (altitud <= 0.0f) { sube = 1; }
			}
		}
		else { altitud = 0.0f; }

		//movimiento noria
		giro += 0.1f;
		if (giro >= 360) { giro = 0.0f; }


		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		shaderList[0].SetDirectionalLight(&mainLight);
		//Luces
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		//Piso Pasto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();

		//lUMINARIA
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.2, -2.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		street_Lamp.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, -2.0f, -1.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		street_Lamp.RenderModel();

		//Luminaria dentro de la feria
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.5f, -2.0f, -23.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		street_Lamp.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.2f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		street_Lamp.RenderModel();

		//Luminaria baño
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.0f, -1.95f, -2.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		street_Lamp.RenderModel();

		/******************************************************* JUEGOS MECANICOS *********************************************************************************/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		BaseNoria.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 6.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		model = glm::rotate(model, giro * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RuedaNoria.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -8.0f));
		model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		CabinaNoria.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.5f, -2.0f, -29.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Kilahuea.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.5f, -2.0f, -29.0f));
		model = glm::translate(model, glm::vec3(0.0f, altitud, 0.0f));
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		base_kilahuea.RenderModel();

		/***************************************************************** ELEMENTOS EXTRA **************************************************************************/
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, -2.0f, -17.5f));
		model = glm::scale(model, glm::vec3(0.18f, 0.18f, 0.18f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		PopCorn.RenderModel();


		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, -2.0f, -15.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Ice_Cream.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.0f, -2.0f, -16.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(7.0f, -2.0f, -3.5f));
		model = glm::scale(model, glm::vec3(2.5f,2.5f, 2.5f));	
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Park.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, -2.0f, -19.0f));
		model = glm::scale(model, glm::vec3(0.33f, 0.33f, 0.33f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Globos.RenderModel();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(17.5f, -2.0f, -12.0f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		HotDog.RenderModel();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.2f, -2.0f, -12.5f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Coke.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.7f, -2.0f, -19.0f));
		model = glm::scale(model, glm::vec3(0.38f, 0.38f, 0.38f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Cotton.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.7f, -2.0f, -15.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.5f, -2.0f, -15.5f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, -2.0f, -19.0f));
		model = glm::scale(model, glm::vec3(0.32f, 0.32f, 0.32f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Corn.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.5f, -2.0f, -27.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//		model = glm::rotate(model, 270 * toRadians, glm::vec3(00.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pan.RenderModel();

		//Entrada a la feria
			//pared izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.3f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.3f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.3f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.3f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();

		//pared derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.6f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.6f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.6f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.6f, -2.0f, -1.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();

		//Pared lateral izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -3.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -6.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -9.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -12.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -15.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -18.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -21.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -24.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -27.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -30.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.8f, -2.0f, -33.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();


		//Pared lateral derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -6.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -9.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -12.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -15.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -18.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -21.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -24.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -27.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -30.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.2f, -2.0f, -33.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();

		//Pared trasera

		//pared izquierda trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.3f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.3f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.3f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.3f, -2.0f, -34.5));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-12.3f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();

		//pared derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.6f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(6.6f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.6f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(12.6f, -2.0f, -34.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		wall.RenderModel();


		//path
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, 2.7f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -2.7f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -5.4f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -8.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -10.8f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -13.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -16.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -18.9f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.4f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.1f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.8f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.1f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();

		//Camino para la noria
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.3f, -2.05f, -8.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.6f, -2.05f, -8.1f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		//Camino para kilauea
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.7f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.4f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.1f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.8f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.1f, -2.05f, -21.6f));
		model = glm::scale(model, glm::vec3(0.2f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();

		//Camino al baño 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, -2.05f, -11.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.4f, -2.05f, -11.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(8.1f, -2.05f, -11.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(10.8f, -2.05f, -11.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.1f, -2.05f, -11.0f));
		model = glm::scale(model, glm::vec3(0.25f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		stone.RenderModel();

		//baño 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.0f, -1.95f, -8.5f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
		model = glm::rotate(model,- 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Bath.RenderModel();

		/*******************Los triggers del globo          ********************/

		if (globo == 1) {
			aumenta_globo += 0.01;
			
	
		}

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.45f, 0.5 + aumenta_globo, -9.0f));
		model = glm::scale(model, glm::vec3(10.0f,10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (trigger_globo == 0) {
			Globo.RenderModel();
		}
		

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(13.45f, 0.5 + (aumenta_globo + 0.05), -8.0f));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		if (trigger_globo == 0) {
			Globo.RenderModel();
		}


		


		


		/*******************************Para los triggers de teoria************

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f, -2.0f, -10.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		/*****************Parte del trigger en pato*********/

		/*v_x_pato += 0.001;
		mov_y_pato += 0.001;

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(9.0f + mov_x_pato, -1.0f + mov_y_pato, -10.0f));
		model = glm::rotate(model, 90  * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Duck.RenderModel();

		printf("Movimiento pato: %f", mov_x_pato);

		if (mov_y_pato == 1.0f){
			printf("Soy igual a 1");
			

		}*/


		mainWindow.swapBuffers();
	}

	engine->drop();
	return 0;
}
