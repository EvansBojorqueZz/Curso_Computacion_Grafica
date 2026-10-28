//Practica 11
//Fecha de entrega: 28 de octubre de 2026
//Bojorquez Covarrubias Evans Martin
//321203018

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <fstream>
#include <sstream>

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();
float GetMaxZ(const char* path);

// Caja que envuelve a un modelo (minimo y maximo de sus vertices)
struct Bounds
{
	glm::vec3 min;
	glm::vec3 max;
};
Bounds GetOBJBounds(const char* path);

// Animaciones del perro
void StepLegs(float rate, float amp); // ciclo de pasos (patas, cabeza y cola) a 'rate' grados por segundo y amplitud 'amp'
float YawTowards(glm::vec3 from, glm::vec3 to);   // angulo en Y para mirar de 'from' hacia 'to'
float AngDiff(float from, float to);  // diferencia angular mas corta (-180 a 180)
glm::vec3 DirFromYaw(float yaw);      // direccion hacia la que mira el perro con ese giro
void BuildPath();                     // arma la trayectoria (rectas y curvas) a partir de los puntos
void Recorrido();                     // avanza al perro sobre la trayectoria y llama a la animacion que toca
void Caminar(float s);                // animacion de caminar en linea recta
void GiroIzquierda(float s);          // animacion de dar la vuelta a la izquierda (curva)
void GiroDerecha(float s);            // animacion de dar la vuelta a la derecha (curva)

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);
//Anim
float rotBall = 0.0f;
bool AnimBall = false;
bool AnimDog = false;
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;
glm::vec3 dogPos(0.0f, 0.0f, 0.0f);
float dogRot = 0.0f;
float floorLimitZ = 0.0f;   // hasta donde puede avanzar el cuerpo del perro
bool step = false;
bool pauseAnim = false;

// ---------------- TRAYECTORIA ----------------
// Puntos del recorrido (se calculan en main a partir del tamano real del piso)
enum Punto { P_E, P_A, P_B, P_C, P_D };
glm::vec3 puntos[5];

// La trayectoria E -> A -> B -> C -> D -> E se arma con tramos rectos (caminar) y tramos curvos
// (dar la vuelta). En cada esquina, en lugar de girar en su lugar, el perro sigue caminando sobre
// un arco de circulo que une las dos rectas, como lo haria un perro real al dar la vuelta.
enum TipoTramo { RECTA, CURVA_IZQ, CURVA_DER };
struct Tramo
{
	TipoTramo tipo;
	float largo;           // longitud del tramo
	glm::vec3 inicio;      // RECTA: punto inicial
	glm::vec3 dir;         // RECTA: direccion | CURVA: direccion al entrar a la curva
	float yawInicio;       // giro del perro al entrar al tramo
	glm::vec3 centro;      // CURVA: centro del arco
	float radio;           // CURVA: radio del arco
	float angulo;          // CURVA: grados que gira (siempre positivo)
};
Tramo tramos[16];
int numTramos = 0;
float largoTotal = 0.0f;     // longitud total del recorrido
float sRecorrido = 0.0f;     // distancia recorrida sobre la trayectoria
int tramoActual = 0;
float velFactor = 0.0f;      // velocidad actual como fraccion de WALK_SPEED (suaviza los cambios)

// Parametros de la animacion
const float WALK_SPEED = 0.45f;      // velocidad de caminata en recta (unidades/s)
const float TURN_SPEED = 0.30f;      // velocidad al dar la vuelta (mas lenta para que la curva sea limpia)
const float ACCEL_TIME = 0.6f;       // segundos para pasar de detenido a velocidad normal
const float DECEL_DIST = 0.40f;      // distancia antes del final en la que empieza a frenar
const float LEG_SPEED = 75.0f;       // velocidad del ciclo de pasos (grados/s) a velocidad normal
const float LEG_AMP = 15.0f;         // amplitud del paso al caminar (grados)
const float TURN_LEG_AMP = 11.0f;    // pasos un poco mas cortos en la curva (grados)
const float TURN_RADIUS = 0.35f;     // radio de las curvas (se reduce solo si no cabe en el piso)
const float HEAD_TURN = 28.0f;       // cuanto voltea la cabeza hacia el lado de la vuelta (grados)
const float ANTICIP_DIST = 0.35f;    // distancia antes de la curva en la que empieza a voltear la cabeza
float headYaw = 0.0f;                // giro de la cabeza en Y (mira hacia el lado de la vuelta)



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bojorquez Covarrubias Evans Martin", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	//models
	Model DogBody((char*)"Models/DogBody.obj");
	Model HeadDog((char*)"Models/HeadDog.obj");
	Model DogTail((char*)"Models/TailDog.obj");
	Model F_RightLeg((char*)"Models/F_RightLegDog.obj");
	Model F_LeftLeg((char*)"Models/F_LeftLegDog.obj");
	Model B_RightLeg((char*)"Models/B_RightLegDog.obj");
	Model B_LeftLeg((char*)"Models/B_LeftLegDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");

	float pisoMaxZ = GetMaxZ("Models/piso.obj");
	float headMaxZ = GetMaxZ("Models/HeadDog.obj");
	floorLimitZ = pisoMaxZ - (0.208f + headMaxZ);   // borde del piso menos lo que sobresale la cabeza
	std::cout << "Borde del piso en z = " << pisoMaxZ << " | El perro se detiene en z = " << floorLimitZ << std::endl;

	// ---------- Puntos de la trayectoria ----------
	// Margen: lo que el perro sobresale de su centro (la nariz al frente o la cola atras),
	// asi en ninguna esquina ni al girar se sale del piso
	Bounds bPiso = GetOBJBounds("Models/piso.obj");
	Bounds bBody = GetOBJBounds("Models/DogBody.obj");
	Bounds bTail = GetOBJBounds("Models/TailDog.obj");
	float frente = 0.208f + headMaxZ;                              // nariz
	float atras = glm::max(-bBody.min.z, 0.288f - bTail.min.z);    // cola o parte trasera del cuerpo
	float margen = glm::max(frente, atras) + 0.05f;
	glm::vec3 centro = (bPiso.min + bPiso.max) * 0.5f;
	puntos[P_E] = glm::vec3(centro.x, 0.0f, centro.z);                           // centro del pasto
	puntos[P_A] = glm::vec3(centro.x, 0.0f, bPiso.max.z - margen);               // frente, al centro
	puntos[P_B] = glm::vec3(bPiso.max.x - margen, 0.0f, bPiso.max.z - margen);   // esquina frente-izquierda del perro
	puntos[P_C] = glm::vec3(bPiso.min.x + margen, 0.0f, bPiso.min.z + margen);   // esquina contraria a B
	puntos[P_D] = glm::vec3(bPiso.max.x - margen, 0.0f, bPiso.min.z + margen);   // esquina al fondo, del lado de B
	dogPos = puntos[P_E];
	BuildPath();
	const char* nombres = "EABCD";
	for (int i = 0; i < 5; i++)
		std::cout << "Punto " << nombres[i] << ": (" << puntos[i].x << ", " << puntos[i].z << ")" << std::endl;
	std::cout << "Controles: B = iniciar recorrido | P = pausa" << std::endl;

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Body
		modelTemp = model = glm::translate(model, dogPos);
		modelTemp = model = glm::rotate(model, glm::radians(dogRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
		model = glm::rotate(model, glm::radians(headYaw), glm::vec3(0.0f, 1.0f, 0.0f));   // voltea hacia el giro
		model = glm::rotate(model, glm::radians(head), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		HeadDog.Draw(lightingShader);
		//Tail 
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.026f, -0.288f));
		model = glm::rotate(model, glm::radians(tail), glm::vec3(0.0f, 0.0f, -1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogTail.Draw(lightingShader);
		//Front Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.112f, -0.044f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(RLegs), glm::vec3(-1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_RightLeg.Draw(lightingShader);


		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		model = glm::rotate(model, glm::radians(rotBall), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ball.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	if (keys[GLFW_KEY_N])
	{
		AnimBall = !AnimBall;

	}
	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		dogAnim = 1;   // inicia (o reinicia desde E) el recorrido
	}
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		pauseAnim = !pauseAnim;
	}
}
void Animation() {
	if (pauseAnim) return;
	if (AnimBall)
	{
		rotBall += 0.4f;
		//printf("%f", rotBall);
	}

	if (AnimDog)
	{
		rotDog -= 0.6f;
		//printf("%f", rotBall);
	}
	if (dogAnim == 1) {  // recorrido E -> A -> B -> C -> D -> E
		Recorrido();
	}
	else {
		// Detenido: patas, cabeza y cola regresan poco a poco a su posicion original
		float k = glm::min(1.0f, 6.0f * deltaTime);
		RLegs -= RLegs * k; FLegs -= FLegs * k; head -= head * k; tail -= tail * k;
		headYaw -= headYaw * k;
	}
}

// Ciclo de pasos de la maquina de estados del previo, ahora en grados por segundo
void StepLegs(float rate, float amp)
{
	float d = rate * deltaTime;
	if (!step) { //state 1
		RLegs += d; FLegs += d; head += d; tail += d;
		if (RLegs > amp) { //condition
			step = true;
		}
	}
	else { //state 2
		RLegs -= d; FLegs -= d; head -= d; tail -= d;
		if (RLegs < -amp) { //condition
			step = false;
		}
	}
}

float YawTowards(glm::vec3 from, glm::vec3 to)
{
	glm::vec3 d = to - from;
	return glm::degrees(atan2(d.x, d.z));   // el modelo mira hacia +Z cuando dogRot = 0
}

glm::vec3 DirFromYaw(float yaw)
{
	float r = glm::radians(yaw);
	return glm::vec3(sin(r), 0.0f, cos(r));
}

float AngDiff(float from, float to)
{
	return fmod(to - from + 540.0f, 360.0f) - 180.0f;
}

// Agrega un tramo recto de 'a' a 'b'
void AddRecta(glm::vec3 a, glm::vec3 b)
{
	float L = glm::length(b - a);
	if (L < 0.0005f) return;
	Tramo t;
	t.tipo = RECTA; t.inicio = a; t.dir = (b - a) / L; t.largo = L;
	t.yawInicio = YawTowards(a, b);
	tramos[numTramos++] = t;
}

// Agrega la curva que une la recta que llega con direccion d0 y la que sale con direccion d1.
// T0 es el punto donde empieza la curva. Regresa en T1 el punto donde termina.
void AddCurva(glm::vec3 T0, glm::vec3 d0, glm::vec3 d1, float R, glm::vec3& T1)
{
	float yaw0 = glm::degrees(atan2(d0.x, d0.z));
	float giro = AngDiff(yaw0, glm::degrees(atan2(d1.x, d1.z)));   // + izquierda, - derecha
	int sentido = (giro > 0.0f) ? 1 : -1;
	glm::vec3 izq(d0.z, 0.0f, -d0.x);                               // normal hacia la izquierda de d0
	Tramo t;
	t.tipo = (sentido > 0) ? CURVA_IZQ : CURVA_DER;
	t.dir = d0; t.yawInicio = yaw0; t.radio = R;
	t.angulo = fabsf(giro);
	t.centro = T0 + izq * (R * sentido);
	t.largo = R * glm::radians(t.angulo);
	t.inicio = T0;
	tramos[numTramos++] = t;
	float a = glm::radians(t.angulo);
	T1 = t.centro - izq * (R * sentido * (float)cos(a)) + d0 * (R * (float)sin(a));
}

// Arma la trayectoria: rectas entre los puntos y una curva en cada esquina.
// La ultima curva termina exactamente en E mirando hacia A (fin e inicio del recorrido).
void BuildPath()
{
	glm::vec3 E = puntos[P_E], A = puntos[P_A], B = puntos[P_B], C = puntos[P_C], D = puntos[P_D];
	glm::vec3 dirA = glm::normalize(A - E);   // direccion final en E (mirar hacia A)

	// Esquinas: punto de la esquina y punto siguiente
	glm::vec3 vert[7] = { E, A, B, C, D, E, E };
	float R = TURN_RADIUS;

	for (int intento = 0; intento < 20; intento++)
	{
		// La ultima esquina (V) se calcula para que la curva final termine justo en E mirando hacia A
		glm::vec3 d0 = glm::normalize(E - D);
		glm::vec3 V = E;
		for (int k = 0; k < 6; k++)
		{
			float g = fabsf(glm::radians(AngDiff(glm::degrees(atan2(d0.x, d0.z)), glm::degrees(atan2(dirA.x, dirA.z)))));
			V = E - dirA * (R * (float)tan(g * 0.5f));
			d0 = glm::normalize(V - D);
		}
		vert[5] = V;
		vert[6] = E;

		// Distancia de cada esquina a la que empieza/termina su curva: t = R * tan(giro/2)
		float tEsq[7] = { 0 };
		for (int k = 1; k <= 5; k++)
		{
			glm::vec3 a = glm::normalize(vert[k] - vert[k - 1]);
			glm::vec3 b = glm::normalize(vert[k + 1] - vert[k]);
			float g = fabsf(glm::radians(AngDiff(glm::degrees(atan2(a.x, a.z)), glm::degrees(atan2(b.x, b.z)))));
			tEsq[k] = R * tan(g * 0.5f);
		}
		// Si alguna recta no alcanza para sus dos curvas, se reduce el radio
		bool cabe = true;
		for (int k = 0; k < 5; k++)
			if (tEsq[k] + tEsq[k + 1] > 0.95f * glm::length(vert[k + 1] - vert[k]) && k + 1 != 5) cabe = false;
		if (tEsq[5] > 0.95f * glm::length(vert[5] - vert[4])) cabe = false;
		if (!cabe) { R *= 0.85f; continue; }

		// Construccion de los tramos
		numTramos = 0;
		glm::vec3 p = vert[0];
		for (int k = 1; k <= 5; k++)
		{
			glm::vec3 a = glm::normalize(vert[k] - vert[k - 1]);
			glm::vec3 b = glm::normalize(vert[k + 1] - vert[k]);
			glm::vec3 T0 = vert[k] - a * tEsq[k];
			AddRecta(p, T0);
			glm::vec3 T1;
			AddCurva(T0, a, b, R, T1);
			p = T1;
		}
		AddRecta(p, E);
		break;
	}

	largoTotal = 0.0f;
	for (int i = 0; i < numTramos; i++) largoTotal += tramos[i].largo;
	std::cout << "Trayectoria: " << numTramos << " tramos, largo " << largoTotal << ", radio de las vueltas " << R << std::endl;
}

// Avanza al perro sobre la trayectoria y llama a la animacion del tramo en el que esta
void Recorrido()
{
	if (sRecorrido >= largoTotal) { sRecorrido = 0.0f; tramoActual = 0; velFactor = 0.0f; }   // B lo reinicia

	// Velocidad: mas lenta en las curvas, arranca suave y frena al final
	Tramo& t = tramos[tramoActual];
	float objetivo = (t.tipo == RECTA) ? 1.0f : TURN_SPEED / WALK_SPEED;
	velFactor += (objetivo - velFactor) * glm::min(1.0f, deltaTime / ACCEL_TIME * 2.5f);
	float falta = largoTotal - sRecorrido;
	float fren = glm::clamp(falta / DECEL_DIST, 0.2f, 1.0f);
	sRecorrido += WALK_SPEED * velFactor * fren * deltaTime;

	if (sRecorrido >= largoTotal)
	{
		sRecorrido = largoTotal;
		dogPos = puntos[P_E];
		dogRot = YawTowards(puntos[P_E], puntos[P_A]);   // termina mirando hacia A
		dogAnim = 0;   // fin del recorrido: queda en E mirando hacia A hasta presionar B
		std::cout << "Recorrido terminado" << std::endl;
		return;
	}

	// Tramo en el que esta y distancia recorrida dentro de el
	float acum = 0.0f;
	for (tramoActual = 0; tramoActual < numTramos - 1; tramoActual++)
	{
		if (sRecorrido < acum + tramos[tramoActual].largo) break;
		acum += tramos[tramoActual].largo;
	}
	float sLocal = sRecorrido - acum;

	// Ritmo de los pasos de acuerdo con la velocidad
	float ritmo = LEG_SPEED * glm::max(0.45f, velFactor * fren);

	switch (tramos[tramoActual].tipo)
	{
	case RECTA:     Caminar(sLocal); StepLegs(ritmo, LEG_AMP); break;
	case CURVA_IZQ: GiroIzquierda(sLocal); StepLegs(ritmo, TURN_LEG_AMP); break;
	case CURVA_DER: GiroDerecha(sLocal); StepLegs(ritmo, TURN_LEG_AMP); break;
	}
}

// Caminar en linea recta. Antes de llegar a la siguiente curva, voltea la cabeza hacia ese lado
void Caminar(float s)
{
	Tramo& t = tramos[tramoActual];
	dogPos = t.inicio + t.dir * s;
	dogRot = t.yawInicio;

	float objetivo = 0.0f;
	float faltaTramo = t.largo - s;
	if (tramoActual + 1 < numTramos && tramos[tramoActual + 1].tipo != RECTA && faltaTramo < ANTICIP_DIST)
	{
		int lado = (tramos[tramoActual + 1].tipo == CURVA_IZQ) ? 1 : -1;
		objetivo = lado * HEAD_TURN * (1.0f - faltaTramo / ANTICIP_DIST);
	}
	headYaw += (objetivo - headYaw) * glm::min(1.0f, 5.0f * deltaTime);
}

// Vuelta comun: el perro avanza sobre el arco y su cuerpo va girando con la tangente de la curva,
// de modo que siempre mira hacia donde camina (no gira en su lugar ni se desliza).
// La cabeza va volteada hacia el lado de la vuelta y se endereza al terminarla.
void Vuelta(float s, int lado)
{
	Tramo& t = tramos[tramoActual];
	float phi = s / t.radio;                                   // angulo recorrido en el arco (radianes)
	glm::vec3 izq(t.dir.z, 0.0f, -t.dir.x);
	dogPos = t.centro - izq * (t.radio * lado * (float)cos(phi)) + t.dir * (t.radio * (float)sin(phi));
	dogRot = t.yawInicio + lado * glm::degrees(phi);          // mira en la direccion de la curva

	float p = s / t.largo;                                     // avance en la curva (0 a 1)
	float r = glm::clamp((p - 0.65f) / 0.35f, 0.0f, 1.0f);
	float objetivo = lado * HEAD_TURN * (1.0f - r * r * (3.0f - 2.0f * r));
	headYaw += (objetivo - headYaw) * glm::min(1.0f, 5.0f * deltaTime);
}

void GiroIzquierda(float s) { Vuelta(s, 1); }
void GiroDerecha(float s) { Vuelta(s, -1); }

// Lee los vertices ("v x y z") de un archivo .obj y regresa su caja envolvente
Bounds GetOBJBounds(const char* path)
{
	Bounds b;
	b.min = glm::vec3(1e9f);
	b.max = glm::vec3(-1e9f);
	std::ifstream file(path);
	std::string line;
	while (std::getline(file, line))
	{
		if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream ss(line.substr(2));
			glm::vec3 v;
			ss >> v.x >> v.y >> v.z;
			b.min = glm::min(b.min, v);
			b.max = glm::max(b.max, v);
		}
	}
	if (b.min.x > b.max.x)
	{
		std::cout << "No se pudo abrir " << path << std::endl;
		b.min = glm::vec3(0.0f);
		b.max = glm::vec3(0.0f);
	}
	return b;
}

// Lee los vertices ("v x y z") de un .obj y regresa su coordenada Z mas grande (lo mas al frente)
float GetMaxZ(const char* path)
{
	std::ifstream file(path);
	std::string line;
	float maxZ = -1e9f;
	while (std::getline(file, line))
	{
		if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
		{
			std::istringstream ss(line.substr(2));
			float x, y, z;
			ss >> x >> y >> z;
			if (z > maxZ) maxZ = z;
		}
	}
	if (maxZ < -1e8f)
	{
		std::cout << "No se pudo abrir " << path << std::endl;
		maxZ = 0.0f;
	}
	return maxZ;
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}