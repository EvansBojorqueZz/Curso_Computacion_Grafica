//Practica 10
//Bojorquez Covarrubias Evans Martin
//Fecha de entrega: 21 de octubre de 2026
//321203018

#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

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

// Caja que envuelve a un modelo (minimo y maximo de sus vertices)
struct Bounds
{
	glm::vec3 min;
	glm::vec3 max;
};
Bounds GetOBJBounds(const char* path);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.8f, 5.0f));   // mas atras para ver todo el circulo
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
bool AnimBall = false;                // N inicia o pausa la animacion
GLfloat animTime = 0.0f;              // tiempo que lleva corriendo la animacion (segundos)

// PELOTA SIMULANDO GLOBO
const GLfloat BALLOON_PATH_RADIUS = 2.0f;   // radio del circulo que recorre
const GLfloat BALLOON_ANG_SPEED = 40.0f;    // grados por segundo (positivo = antihorario visto desde arriba)
GLfloat balloonBaseY = 0.95f;               // altura media a la que flota (se calcula en main para que pase sobre el perro)
const GLfloat BALLOON_CLEARANCE = 0.05f;    // espacio libre entre el globo (en su punto mas bajo) y la cabeza del perro
const GLfloat BALLOON_BOB_AMP = 0.20f;      // cuanto sube y baja al flotar
const GLfloat BALLOON_BOB_FREQ = 0.5f;      // oscilaciones de flotacion por segundo (una subida y bajada cada 2 s)
GLfloat balloonAngle = 0.0f;                // angulo actual sobre el circulo (grados)
glm::vec3 balloonPos(BALLOON_PATH_RADIUS, 0.95f, 0.0f);   // posicion del centro del globo

// ANIMACIÓN DE PERRO
const GLfloat DOG_PATH_RADIUS = 2.0f;      // radio del circulo que recorre
const GLfloat DOG_ANG_SPEED = -30.0f;      // grados por segundo (negativo = sentido contrario al globo)
GLfloat dogAngle = 180.0f;                 // angulo actual sobre el circulo (grados), inicia frente al globo
glm::vec3 dogPos(-DOG_PATH_RADIUS, 0.0f, 0.0f);   // posicion del perro sobre el piso
GLfloat dogYaw = 180.0f;                   // giro en Y para que mire hacia donde camina (grados)

// SIMULACIÓN SALTO DEL PERRO
enum DogState { CAMINANDO, PREPARACION, SALTO };
DogState dogState = CAMINANDO;
GLfloat stateTime = 0.0f;                  // tiempo que lleva el perro en el estado actual
const GLfloat PREP_TIME = 0.35f;           // segundos que dura agachandose
const GLfloat JUMP_UP_TIME = 0.35f;        // segundos que tarda en llegar a lo mas alto (bajar tarda lo mismo)
GLfloat jumpGravity = 9.8f;                // gravedad del salto: se calcula para que suba justo hasta el globo
GLfloat dogJumpY = 0.0f;                   // altura del perro sobre el piso durante el salto
GLfloat dogVelY = 0.0f;                    // velocidad vertical del perro
GLfloat dogScaleY = 1.0f;                  // aplastamiento en Y al agacharse
GLfloat dogScaleXZ = 1.0f;                 // ensanchamiento en X y Z al agacharse
GLfloat dogFeetY = 0.0f;                   // altura de las patas del modelo (punto de apoyo para agacharse)
GLfloat noseRestY = 0.0f;                  // altura del hocico con el perro parado en el piso
GLfloat balloonRadius = 0.1f;              // radio del globo (se mide en main)
GLfloat jumpTriggerGap = 60.0f;            // separacion angular a la que empieza a prepararse (se calcula en main)
glm::vec3 noseLocalPos(0.0f);              // hocico en coordenadas del modelo del perro (se mide en main)

// SIMULACIÓN GLOBO RECIBIENDO IMPULSO
const GLfloat BALLOON_IMPULSE = 3.0f;      // velocidad hacia arriba que le da el golpe (unidades/s)
const GLfloat BALLOON_LIFT_DECEL = 4.0f;   // desaceleracion que lo regresa a su altura normal (unidades/s^2)
GLfloat balloonLift = 0.0f;                // altura extra por el impulso
GLfloat balloonLiftVel = 0.0f;             // velocidad vertical extra por el impulso
bool hitThisJump = false;                  // evita dar mas de un impulso en el mismo salto

const GLfloat NOSE_HEIGHT = 0.78f;    // la nariz esta al 78% de la altura del perro (se usara para la colision, se calculó en el previo)


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

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	// El mouse queda dentro de la ventana (oculto) para mover la camara libremente; ESC cierra el programa
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Model Dog((char*)"Models/RedDog.obj");
	Model Piso((char*)"Models/piso.obj");
	Model Ball((char*)"Models/ball.obj");

	// Medidas reales de los modelos (a partir de sus vertices)
	Bounds bDog = GetOBJBounds("Models/RedDog.obj");
	Bounds bBall = GetOBJBounds("Models/ball.obj");
	glm::vec3 dogSize = bDog.max - bDog.min;
	glm::vec3 ballCenterLocal = (bBall.min + bBall.max) * 0.5f;   // el modelo no esta centrado en el origen
	GLfloat ballRadius = (bBall.max.y - bBall.min.y) * 0.5f;

	// Hocico del perro en sus coordenadas locales: centro en X, punto mas al frente en Z (se usara para la colision)
	glm::vec3 noseLocal((bDog.min.x + bDog.max.x) * 0.5f, bDog.min.y + dogSize.y * NOSE_HEIGHT, bDog.max.z);
	std::cout << "Radio del globo: " << ballRadius << " | Hocico del perro en y = " << noseLocal.y << std::endl;
	dogFeetY = bDog.min.y;
	noseRestY = noseLocal.y;
	noseLocalPos = noseLocal;
	balloonRadius = ballRadius;

	// Altura media del globo: su punto mas bajo (centro - amplitud - radio) queda arriba de la cabeza del perro
	balloonBaseY = bDog.max.y + BALLOON_CLEARANCE + ballRadius + BALLOON_BOB_AMP;
	balloonPos.y = balloonBaseY;
	std::cout << "El globo flota entre y = " << balloonBaseY - BALLOON_BOB_AMP << " y y = " << balloonBaseY + BALLOON_BOB_AMP << std::endl;

	// El salto se dispara con anticipacion: el tiempo de agacharse mas el de subir hasta lo mas alto,
	// multiplicado por la rapidez con que se acercan (suma de ambas velocidades angulares), mas el angulo
	// entre el centro del perro y el centro del globo cuando el hocico lo toca.
	GLfloat timeToApex = PREP_TIME + JUMP_UP_TIME;
	GLfloat closingSpeed = fabs(BALLOON_ANG_SPEED) + fabs(DOG_ANG_SPEED);   // grados por segundo
	GLfloat contactGap = glm::degrees((noseLocal.z + 0.7f * ballRadius) / DOG_PATH_RADIUS);
	jumpTriggerGap = closingSpeed * timeToApex + contactGap;
	std::cout << "El perro se prepara para saltar cuando el globo esta a " << jumpTriggerGap << " grados" << std::endl;
	std::cout << "Controles: N = iniciar/pausar animacion | ESPACIO = luz puntual | ESC = salir" << std::endl;



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

		// Perro: se traslada a su punto del circulo y se gira para mirar en la direccion en que camina
		model = glm::mat4(1);
		model = glm::translate(model, dogPos + glm::vec3(0.0f, dogJumpY, 0.0f));   // dogJumpY = altura del salto
		model = glm::rotate(model, glm::radians(dogYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		// Aplastamiento al agacharse: se escala respecto a las patas para que no se hunda en el piso
		model = glm::translate(model, glm::vec3(0.0f, dogFeetY, 0.0f));
		model = glm::scale(model, glm::vec3(dogScaleXZ, dogScaleY, dogScaleXZ));
		model = glm::translate(model, glm::vec3(0.0f, -dogFeetY, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Dog.Draw(lightingShader);

		model = glm::mat4(1);
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		// Globo: se traslada a su posicion sobre el circulo (balloonPos se calcula en Animation)
		model = glm::translate(model, balloonPos);
		model = glm::translate(model, -ballCenterLocal);   // centra el modelo para moverlo desde su centro
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
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
	// N inicia o detiene la animacion (solo al presionar, para que no se active y desactive sola)
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		AnimBall = !AnimBall;
	}
}
void Animation() {
	if (!AnimBall) return;

	animTime += deltaTime;

	// ---------- Trayectoria del globo ----------
	// Avanza sobre el circulo en sentido positivo (angulo creciente)
	balloonAngle += BALLOON_ANG_SPEED * deltaTime;
	if (balloonAngle >= 360.0f) balloonAngle -= 360.0f;

	// Punto del circulo: es (R, 0, 0) girado balloonAngle grados sobre el eje Y
	GLfloat a = glm::radians(balloonAngle);
	balloonPos.x = BALLOON_PATH_RADIUS * cos(a);
	balloonPos.z = -BALLOON_PATH_RADIUS * sin(a);

	// Flotacion: sube y baja con una onda seno alrededor de su altura media
	// Impulso: la altura extra sube con la velocidad del golpe y la desaceleracion lo regresa a flotar
	if (balloonLift > 0.0f || balloonLiftVel > 0.0f)
	{
		balloonLiftVel -= BALLOON_LIFT_DECEL * deltaTime;
		balloonLift += balloonLiftVel * deltaTime;
		if (balloonLift <= 0.0f)
		{
			balloonLift = 0.0f;
			balloonLiftVel = 0.0f;
		}
	}
	balloonPos.y = balloonBaseY + BALLOON_BOB_AMP * sin(animTime * BALLOON_BOB_FREQ * 2.0f * glm::pi<float>()) + balloonLift;

	// ---------- Trayectoria del perro ----------
	// Avanza sobre su circulo en sentido negativo (angulo decreciente)
	dogAngle += DOG_ANG_SPEED * deltaTime;
	if (dogAngle < 0.0f) dogAngle += 360.0f;

	GLfloat b = glm::radians(dogAngle);
	dogPos.x = DOG_PATH_RADIUS * cos(b);
	dogPos.z = -DOG_PATH_RADIUS * sin(b);

	// Orientacion: al recorrer el circulo con angulo decreciente, la direccion de avance es (sin b, 0, cos b).
	// El modelo del perro mira hacia +Z, asi que basta girarlo en Y el mismo angulo b para que mire hacia adelante.
	dogYaw = dogAngle;

	// ---------- Preparacion y salto ----------
	// Separacion angular entre perro y globo: se reduce a medida que se acercan de frente
	GLfloat gap = fmod(dogAngle - balloonAngle + 720.0f, 360.0f);
	stateTime += deltaTime;

	switch (dogState)
	{
	case CAMINANDO:
		dogScaleY = 1.0f;
		dogScaleXZ = 1.0f;
		// Cuando el globo esta por pasar frente a el, empieza a prepararse
		if (gap < jumpTriggerGap)
		{
			dogState = PREPARACION;
			stateTime = 0.0f;
		}
		break;

	case PREPARACION:
	{
		// Se agacha poco a poco: hasta 25% mas bajo y 10% mas ancho
		GLfloat p = glm::min(stateTime / PREP_TIME, 1.0f);
		dogScaleY = 1.0f - 0.25f * p;
		dogScaleXZ = 1.0f + 0.10f * p;
		if (stateTime >= PREP_TIME)
		{
			// Altura que tendra el globo cuando el perro llegue a lo mas alto del salto
			GLfloat tApex = animTime + JUMP_UP_TIME;
			GLfloat balloonApexY = balloonBaseY + BALLOON_BOB_AMP * sin(tApex * BALLOON_BOB_FREQ * 2.0f * glm::pi<float>());
			// Altura que debe subir para que la punta del hocico toque el globo por abajo y al frente
			// (0.7 r abajo y 0.7 r al frente del centro: la distancia queda casi igual al radio, solo lo roza)
			GLfloat h = balloonApexY - 0.7f * balloonRadius - noseRestY;
			if (h < 0.1f) h = 0.1f;
			// Con un tiempo de subida fijo: h = v0*t - g*t^2/2 y v0 = g*t  ->  g = 2h/t^2, v0 = 2h/t
			jumpGravity = 2.0f * h / (JUMP_UP_TIME * JUMP_UP_TIME);
			dogVelY = 2.0f * h / JUMP_UP_TIME;   // impulso hacia arriba
			dogState = SALTO;
			stateTime = 0.0f;
		}
		break;
	}

	case SALTO:
	{
		// Sube y baja: la gravedad frena la subida y luego lo hace caer
		dogVelY -= jumpGravity * deltaTime;
		dogJumpY += dogVelY * deltaTime;

		// ---------- Colision hocico - globo ----------
		// Posicion real del hocico: se gira con el perro (dogYaw) y se traslada a su posicion actual
		glm::mat4 dogModel(1);
		dogModel = glm::translate(dogModel, dogPos + glm::vec3(0.0f, dogJumpY, 0.0f));
		dogModel = glm::rotate(dogModel, glm::radians(dogYaw), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec3 noseWorld = glm::vec3(dogModel * glm::vec4(noseLocalPos, 1.0f));
		// Hay colision cuando la punta del hocico llega a la superficie del globo
		if (!hitThisJump && glm::distance(noseWorld, balloonPos) <= balloonRadius * 1.05f)
		{
			hitThisJump = true;
			balloonLiftVel = BALLOON_IMPULSE;   // el perro lo impulsa hacia arriba
			balloonLift = glm::max(balloonLift, 0.001f);
			std::cout << "Colision! El perro impulsa el globo" << std::endl;
		}

		// Recupera su forma normal al despegar
		dogScaleY = 1.0f;
		dogScaleXZ = 1.0f;
		if (dogJumpY <= 0.0f)
		{
			// Toca el piso y sigue caminando
			dogJumpY = 0.0f;
			dogVelY = 0.0f;
			dogState = CAMINANDO;
			stateTime = 0.0f;
			hitThisJump = false;
		}
		break;
	}
	}

	//printf("Globo: angulo %.1f  pos (%.2f, %.2f, %.2f)\n", balloonAngle, balloonPos.x, balloonPos.y, balloonPos.z);
}

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
	if (b.min.x > b.max.x)   // no se encontro el archivo
	{
		std::cout << "No se pudo abrir " << path << std::endl;
		b.min = glm::vec3(0.0f);
		b.max = glm::vec3(0.0f);
	}
	return b;
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