//Practica 12
//Bojorquez Covarrubias Evans Martin
//Fecha de entrega: 4 de noviembre de 2026
//321203018


#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

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


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

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
float rotDog = 0.0f;
int dogAnim = 0;
float FLegs = 0.0f;
float RLegs = 0.0f;
float head = 0.0f;
float tail = 0.0f;



// Partes del perro que se animan por separado
float FLegL = 0.0f;      // pata delantera izquierda (grados, positivo = pata hacia atras)
float FLegR = 0.0f;      // pata delantera derecha
float BLegL = 0.0f;      // pata trasera izquierda
float BLegR = 0.0f;      // pata trasera derecha
float bodyPitch = 0.0f;  // inclinacion del cuerpo sobre las caderas (negativo = levanta el frente, para sentarse)
float bodyRoll = 0.0f;   // giro del perro sobre su eje largo (90 = de lado, 180 = boca arriba)

// Velocidades de los controles (por segundo, para que no dependan de los FPS)
const float MOVE_SPEED = 0.5f;    // unidades por segundo
const float ROT_SPEED = 60.0f;    // grados por segundo

//KeyFrames
float dogPosX, dogPosY, dogPosZ;

#define MAX_FRAMES 30
int i_max_steps = 70;          // pasos entre keyframes (a 60 FPS son poco mas de 1 segundo)
int i_curr_steps = 0;
typedef struct _frame {

	float rotDog;
	float rotDogInc;
	float dogPosX;
	float dogPosY;
	float dogPosZ;
	float incX;
	float incY;
	float incZ;
	float head;
	float headInc;
	float tail;
	float tailInc;
	float FLegL;
	float FLegLInc;
	float FLegR;
	float FLegRInc;
	float BLegL;
	float BLegLInc;
	float BLegR;
	float BLegRInc;
	float bodyPitch;
	float bodyPitchInc;
	float bodyRoll;
	float bodyRollInc;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

// Archivo donde se guarda la animacion (en la carpeta del proyecto, junto a Models y Shader)
const char* ANIM_FILE = "keyframes.txt";
bool unsavedFrames = false;   // true si hay keyframes grabados con K que aun no se guardan con P

void saveFrame(void)
{

	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].dogPosX = dogPosX;
	KeyFrame[FrameIndex].dogPosY = dogPosY;
	KeyFrame[FrameIndex].dogPosZ = dogPosZ;

	KeyFrame[FrameIndex].rotDog = rotDog;
	KeyFrame[FrameIndex].head = head;
	KeyFrame[FrameIndex].tail = tail;
	KeyFrame[FrameIndex].FLegL = FLegL;
	KeyFrame[FrameIndex].FLegR = FLegR;
	KeyFrame[FrameIndex].BLegL = BLegL;
	KeyFrame[FrameIndex].BLegR = BLegR;
	KeyFrame[FrameIndex].bodyPitch = bodyPitch;
	KeyFrame[FrameIndex].bodyRoll = bodyRoll;

	// Valores guardados (sirven para el reporte)
	printf("  pos(%.2f, %.2f, %.2f) rot %.1f | lado %.1f | cuerpo %.1f | cabeza %.1f | cola %.1f | DI %.1f DD %.1f TI %.1f TD %.1f\n",
		dogPosX, dogPosY, dogPosZ, rotDog, bodyRoll, bodyPitch, head, tail, FLegL, FLegR, BLegL, BLegR);

	FrameIndex++;
	unsavedFrames = true;
}

// Coloca al perro exactamente en la pose del keyframe i
void applyFrame(int i)
{
	dogPosX = KeyFrame[i].dogPosX;
	dogPosY = KeyFrame[i].dogPosY;
	dogPosZ = KeyFrame[i].dogPosZ;
	rotDog = KeyFrame[i].rotDog;
	bodyRoll = KeyFrame[i].bodyRoll;
	bodyPitch = KeyFrame[i].bodyPitch;
	head = KeyFrame[i].head;
	tail = KeyFrame[i].tail;
	FLegL = KeyFrame[i].FLegL;
	FLegR = KeyFrame[i].FLegR;
	BLegL = KeyFrame[i].BLegL;
	BLegR = KeyFrame[i].BLegR;
}

void resetElements(void)
{
	applyFrame(0);
}
void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].dogPosX - KeyFrame[playIndex].dogPosX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].dogPosY - KeyFrame[playIndex].dogPosY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].dogPosZ - KeyFrame[playIndex].dogPosZ) / i_max_steps;
	KeyFrame[playIndex].headInc = (KeyFrame[playIndex + 1].head - KeyFrame[playIndex].head) / i_max_steps;
	KeyFrame[playIndex].tailInc = (KeyFrame[playIndex + 1].tail - KeyFrame[playIndex].tail) / i_max_steps;
	KeyFrame[playIndex].FLegLInc = (KeyFrame[playIndex + 1].FLegL - KeyFrame[playIndex].FLegL) / i_max_steps;
	KeyFrame[playIndex].FLegRInc = (KeyFrame[playIndex + 1].FLegR - KeyFrame[playIndex].FLegR) / i_max_steps;
	KeyFrame[playIndex].BLegLInc = (KeyFrame[playIndex + 1].BLegL - KeyFrame[playIndex].BLegL) / i_max_steps;
	KeyFrame[playIndex].BLegRInc = (KeyFrame[playIndex + 1].BLegR - KeyFrame[playIndex].BLegR) / i_max_steps;
	KeyFrame[playIndex].bodyPitchInc = (KeyFrame[playIndex + 1].bodyPitch - KeyFrame[playIndex].bodyPitch) / i_max_steps;
	KeyFrame[playIndex].bodyRollInc = (KeyFrame[playIndex + 1].bodyRoll - KeyFrame[playIndex].bodyRoll) / i_max_steps;

	KeyFrame[playIndex].rotDogInc = (KeyFrame[playIndex + 1].rotDog - KeyFrame[playIndex].rotDog) / i_max_steps;

}



// Guarda todos los keyframes en el archivo de texto (un keyframe por renglon)
void saveAnimationToFile(void)
{
	std::ofstream file(ANIM_FILE);
	if (!file.is_open())
	{
		printf("No se pudo crear %s\n", ANIM_FILE);
		return;
	}
	file << "# x y z rot lado cuerpo cabeza cola DI DD TI TD\n";
	for (int i = 0; i < FrameIndex; i++)
	{
		const FRAME& k = KeyFrame[i];
		file << k.dogPosX << " " << k.dogPosY << " " << k.dogPosZ << " " << k.rotDog << " "
			<< k.bodyRoll << " " << k.bodyPitch << " " << k.head << " " << k.tail << " "
			<< k.FLegL << " " << k.FLegR << " " << k.BLegL << " " << k.BLegR << "\n";
	}
	unsavedFrames = false;
	printf("Animacion guardada en %s (%d keyframes)\n", ANIM_FILE, FrameIndex);
}

// Lee los keyframes del archivo de texto y deja al perro en la pose del keyframe 0
bool loadAnimationFromFile(void)
{
	std::ifstream file(ANIM_FILE);
	if (!file.is_open())
	{
		printf("No se encontro %s, graba la animacion con K y guardala con P\n", ANIM_FILE);
		return false;
	}
	std::string line;
	int n = 0;
	while (std::getline(file, line) && n < MAX_FRAMES)
	{
		if (line.empty() || line[0] == '#') continue;   // renglones vacios o comentarios
		std::istringstream ss(line);
		FRAME k = {};
		if (ss >> k.dogPosX >> k.dogPosY >> k.dogPosZ >> k.rotDog >> k.bodyRoll >> k.bodyPitch
			>> k.head >> k.tail >> k.FLegL >> k.FLegR >> k.BLegL >> k.BLegR)
		{
			KeyFrame[n++] = k;
		}
	}
	FrameIndex = n;
	unsavedFrames = false;
	if (n > 0) applyFrame(0);
	printf("Animacion cargada de %s (%d keyframes). Presiona L para verla\n", ANIM_FILE, n);
	return n > 0;
}

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
	glfwSwapInterval(1);   // sincronizacion vertical: la animacion va a la frecuencia del monitor (~60 FPS)

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


	//KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].dogPosX = 0;
		KeyFrame[i].dogPosY = 0;
		KeyFrame[i].dogPosZ = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotDog = 0;
		KeyFrame[i].rotDogInc = 0;
		KeyFrame[i].head = 0;
		KeyFrame[i].headInc = 0;
		KeyFrame[i].tail = 0;
		KeyFrame[i].tailInc = 0;
		KeyFrame[i].FLegL = 0;
		KeyFrame[i].FLegLInc = 0;
		KeyFrame[i].FLegR = 0;
		KeyFrame[i].FLegRInc = 0;
		KeyFrame[i].BLegL = 0;
		KeyFrame[i].BLegLInc = 0;
		KeyFrame[i].BLegR = 0;
		KeyFrame[i].BLegRInc = 0;
		KeyFrame[i].bodyPitch = 0;
		KeyFrame[i].bodyPitchInc = 0;
		KeyFrame[i].bodyRoll = 0;
		KeyFrame[i].bodyRollInc = 0;
	}

	// Carga la animacion guardada para poder verla con solo presionar L
	loadAnimationFromFile();


	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO, EBO;
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
		modelTemp = model = glm::translate(model, glm::vec3(dogPosX, dogPosY, dogPosZ));
		modelTemp = model = glm::rotate(model, glm::radians(rotDog), glm::vec3(0.0f, 1.0f, 0.0f));
		// Giro sobre el eje largo del perro (de la cola a la cabeza) para acostarlo de lado o boca arriba
		model = glm::rotate(model, glm::radians(bodyRoll), glm::vec3(0.0f, 0.0f, 1.0f));
		// Inclinacion del cuerpo para sentarse: gira sobre las caderas (patas traseras)
		model = glm::translate(model, glm::vec3(0.0f, -0.05f, -0.22f));
		model = glm::rotate(model, glm::radians(bodyPitch), glm::vec3(1.0f, 0.0f, 0.0f));
		modelTemp = model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.22f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		DogBody.Draw(lightingShader);
		//Head
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.0f, 0.093f, 0.208f));
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
		model = glm::rotate(model, glm::radians(FLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_LeftLeg.Draw(lightingShader);
		//Front Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.111f, -0.055f, 0.074f));
		model = glm::rotate(model, glm::radians(FLegR), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		F_RightLeg.Draw(lightingShader);
		//Back Left Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.082f, -0.046, -0.218));
		model = glm::rotate(model, glm::radians(BLegL), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		B_LeftLeg.Draw(lightingShader);
		//Back Right Leg
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.083f, -0.057f, -0.231f));
		model = glm::rotate(model, glm::radians(BLegR), glm::vec3(1.0f, 0.0f, 0.0f));
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
	//Dog Controls (todas las velocidades por segundo, multiplicadas por deltaTime)
	float rot = ROT_SPEED * deltaTime;
	float mov = MOVE_SPEED * deltaTime;

	// Cabeza
	if (keys[GLFW_KEY_4]) head += rot;
	if (keys[GLFW_KEY_5]) head -= rot;
	// Pata delantera derecha
	if (keys[GLFW_KEY_6]) FLegR += rot;
	if (keys[GLFW_KEY_7]) FLegR -= rot;
	// Pata delantera izquierda
	if (keys[GLFW_KEY_8]) FLegL += rot;
	if (keys[GLFW_KEY_9]) FLegL -= rot;
	// Pata trasera derecha
	if (keys[GLFW_KEY_Z]) BLegR += rot;
	if (keys[GLFW_KEY_X]) BLegR -= rot;
	// Pata trasera izquierda
	if (keys[GLFW_KEY_C]) BLegL += rot;
	if (keys[GLFW_KEY_V]) BLegL -= rot;
	// Cola
	if (keys[GLFW_KEY_B]) tail += rot;
	if (keys[GLFW_KEY_N]) tail -= rot;
	// Inclinacion del cuerpo (sentarse)
	if (keys[GLFW_KEY_I]) bodyPitch += rot;
	if (keys[GLFW_KEY_O]) bodyPitch -= rot;
	// Giro del cuerpo de lado (acostarse boca arriba)
	if (keys[GLFW_KEY_Q]) bodyRoll += rot;
	if (keys[GLFW_KEY_E]) bodyRoll -= rot;
	// Altura del perro
	if (keys[GLFW_KEY_R]) dogPosY += mov * 0.3f;
	if (keys[GLFW_KEY_F]) dogPosY -= mov * 0.3f;

	if (keys[GLFW_KEY_2])
	{

		rotDog += rot;

	}

	if (keys[GLFW_KEY_3])
	{

		rotDog -= rot;

	}

	if (keys[GLFW_KEY_H])
	{
		dogPosZ += mov;
	}

	if (keys[GLFW_KEY_Y])
	{
		dogPosZ -= mov;
	}

	if (keys[GLFW_KEY_G])
	{
		dogPosX -= mov;
	}

	if (keys[GLFW_KEY_J])
	{
		dogPosX += mov;
	}

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

	// (Las teclas de la luz T, G, Y, H, U, J se quitaron porque G, Y, H y J ahora mueven al perro)
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{

	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{

			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
		}

	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}

	}

	// P: guarda la animacion en keyframes.txt
	if (key == GLFW_KEY_P && action == GLFW_PRESS && !play)
	{
		if (FrameIndex > 1)
			saveAnimationToFile();
		else
			printf("Graba al menos 2 keyframes con K antes de guardar\n");
	}

	// M: vuelve a cargar la animacion de keyframes.txt
	if (key == GLFW_KEY_M && action == GLFW_PRESS && !play)
	{
		// No recarga si hay keyframes sin guardar, para no perderlos
		if (unsavedFrames)
			printf("Hay keyframes sin guardar. Presiona P para guardarlos o 0 para descartarlos\n");
		else
			loadAnimationFromFile();
	}

	// 0: borra los keyframes de la memoria para grabar una animacion nueva (el archivo no cambia)
	if (key == GLFW_KEY_0 && action == GLFW_PRESS && !play)
	{
		FrameIndex = 0;
		unsavedFrames = false;
		printf("Keyframes borrados de la memoria\n");
	}

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


}
void Animation() {

	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("termina anim\n");
				applyFrame(FrameIndex - 1);   // queda exacto en el ultimo keyframe para poder seguir grabando
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
				//Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			dogPosX += KeyFrame[playIndex].incX;
			dogPosY += KeyFrame[playIndex].incY;
			dogPosZ += KeyFrame[playIndex].incZ;
			head += KeyFrame[playIndex].headInc;
			tail += KeyFrame[playIndex].tailInc;
			FLegL += KeyFrame[playIndex].FLegLInc;
			FLegR += KeyFrame[playIndex].FLegRInc;
			BLegL += KeyFrame[playIndex].BLegLInc;
			BLegR += KeyFrame[playIndex].BLegRInc;
			bodyPitch += KeyFrame[playIndex].bodyPitchInc;
			bodyRoll += KeyFrame[playIndex].bodyRollInc;
			rotDog += KeyFrame[playIndex].rotDogInc;

			i_curr_steps++;
		}

	}

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