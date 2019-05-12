#include "Window.h"

extern GLfloat posX, posY, posZ;
extern GLfloat frontX, frontY, frontZ;
extern GLfloat upX, upY, upZ;
extern GLint camara1, camara2, apagarS1, apagarS2, apagarP1, apagarP2;

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Proyecto Final :) para Roque0", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas

							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
		printf("se presiono la tecla: %s\n", key_name);
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{


		printf("PosX: %f\n", posX);
		printf("PosY: %f\n", posY);
		printf("PosZ: %f\n", posZ);
		printf("frontX: %f\n", frontX);
		printf("frontY: %f\n", frontY);
		printf("frontZ: %f\n", frontZ);
		printf("UpX: %f\n", upX);
		printf("UpY: %f\n", upY);
		printf("UpZ: %f\n", upZ);


	}

	/**********************Para camara aerea************************/
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		camara2 = 1;
		camara1 = 0;

	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		camara1 = 1;
		camara2 = 0;

	}
	/*************************Para luces tipo spotlight **********************************************/
	if (key == GLFW_KEY_O && action == GLFW_PRESS)
	{
		apagarS1 = 1;

	}
	if (key == GLFW_KEY_I && action == GLFW_PRESS)
	{
		apagarS1 = 2;

	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		apagarS2 = 1;

	}
	if (key == GLFW_KEY_U && action == GLFW_PRESS)
	{
		apagarS2 = 2;

	}

	//movimiento kilauea
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		if (kilauea == 1) { kilauea = 0; }
		else { kilauea = 1; }
	}

	/******************************Para luces tipo point******************************************************************/
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		apagarP1 = 1;

	}
	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		apagarP1 = 2;
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		apagarP2 = 1;

	}
	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		apagarP2 = 2;
	}



	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			printf("se solto la tecla %d'\n", key);
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
