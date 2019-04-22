
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
//para probar el importer
//#include<assimp/Importer.hpp>

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

const float toRadians = 3.14159265f / 180.0f;
float movOffset;
bool avanza;
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
//materiales
Material Material_brillante;
Material Material_opaco;
//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
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

Skybox skybox;
Skybox skybox2;
Skybox skybox3;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int cont=0;
float hora = 0.0;

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

	
	plainTexture = Texture("Textures/pasto.png");
	plainTexture.LoadTextureA();
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

	//Bath
	Bath = Model();
	Bath.LoadModel("Models/bathroom.obj");


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
								0.3f, 0.3f,
								0.0f, 0.0f, -1.0f);
//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
								0.0f, 1.0f,
								2.0f, 1.5f,1.5f,
								0.3f, 0.2f, 0.1f);
	pointLightCount++;
	
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;

	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		10.0f, 0.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		10.0f);
	spotLightCount++;

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

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//Cambia skybox de acuerdo a la hora del dia
		//dia
		if (hora >= 0.0 && hora <= 8.0) {
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//amanecer o atardecer
		else if((hora > 8.0 && hora <= 10.0)|| (hora > 21.0 && hora <= 24.0)){
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//noche
		else if (hora > 10.0 && hora <= 21.0) {
			skybox3.DrawSkybox(camera.calculateViewMatrix(), projection);
		}

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
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		//Piso Pasto
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, -25.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f,1.0f));
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

			//pista gokarts	
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(6.5f, -1.95f, -11.8f));
			model = glm::scale(model, glm::vec3(0.55f, 0.55f, 0.55f));
			model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			RoadsFair.RenderModel();
			//kart
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(4.0f, -1.95f, -8.8f));
			model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Kart.RenderModel();


			//baño Cambiar escala y posicion 
			model = glm::mat4(1.0);
			model = glm::translate(model, glm::vec3(13.0f, -1.95f, -2.5f));
			model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.006f));
			model = glm::rotate(model, 180* toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
			Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
			Bath.RenderModel();

		

			glUseProgram(0);

			


		mainWindow.swapBuffers();
	}

	return 0;
}
