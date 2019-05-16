#include "Camera.h"

extern GLfloat posX = 0, posY = 0, posZ = 0;
extern GLfloat frontX = 0, frontY = 0, frontZ = 0;
extern GLfloat upX = 0, upY = 0, upZ = 0, lastposX = 0, lastposY = 0, LastposZ = 0;
extern GLfloat lastFrontx = 0, lastFronty = 0, lastFrontz = 0, lastUpx = 0, lastUpy = 0, lastUpz = 0;


Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

//Camara Libre
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;
	
	//Almaceno los valores
	position[0] = lastposX;
	position[1] = lastposY;
	position[2] = LastposZ;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}

	//Reasignacion 
	lastposX = position[0];
	lastposY = position[1];
	LastposZ = position[2];


}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	front[0] = lastFrontx;
	front[1] = lastFronty;
	front[2] = lastFrontz;
	up[0] = lastUpx;
	up[1] = lastUpy;
	up[2] = lastUpz;

	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;


	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	lastFrontx = front[0] ;
	lastFronty = front[1] ;
	lastFrontz = front[2] ;
	lastUpx = up[0] ;
	lastUpy = up[1] ;
	lastUpz = up[2] ;

	update();
}
//aqui termina

//Camara aerea
void Camera::keyControlAerea(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	position[1] = 35.324623; //valor de y


	if (keys[GLFW_KEY_D])
	{
		position[0] += front[1] * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position[0] -= front[1] * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position[2] -= right[2] * velocity*6.0;
	}

	if (keys[GLFW_KEY_W])
	{
		position[2] += right[2] * velocity*6.0;
	}
}

void Camera::mouseControlAerea(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;
	
	front[0] = 0.002127;
	front[1] = -0.999848;
	front[2] = 0.017322;

	up[0] = 0.121851;
	up[1] = 0.017452;
	up[2] = 0.992395;


	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right,front));

}

//Camara para la noria

void Camera::keyControlNoria(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	position[0] = 3.34335;
	position[1] = 5.077526;
	position[2] = -8.5496;

	if (keys[GLFW_KEY_D])
	{
		position[0] += front[1] * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position[0] -= front[1] * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position[2] -= right[2] * velocity*6.0;
	}

	if (keys[GLFW_KEY_W])
	{
		position[2] += right[2] * velocity*6.0;
	}
}

void Camera::mouseControlNoria(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	front[0] = -0.997412;
	front[1] = -0.17452;
	front[2] = -0.069746;

	up[0] = -0.017410;
	up[1] = 2.0;
	up[2] = -0.001217;


	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}

	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));

}
//Hasta aqui termina

glm::mat4 Camera::calculateViewMatrix()
{

	posX = position[0];
	posY = position[1];
	posZ = position[2];
	frontX = front[0];
	frontY = front[1];
	frontZ = front[2];
	upX = up[0];
	upY = up[1];
	upZ = up[2];

	return glm::lookAt(position, position + front, up);

}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}


glm::vec3 Camera::getCameraDirection()
{
	return glm::normalize(front);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
