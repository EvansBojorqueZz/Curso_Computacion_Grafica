// Práctica 4
// Bojorquez Covarrubias Evans Martin
// Fecha de entrega: 12 de Septiembre de 2026
// No. de Cuenta: 321203018


#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



// Shaders
#include "Shader.h"

void Inputs(GLFWwindow* window);


const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.75f;
float movZ = -12.0f;
float rot = 0.0f;

// Control de tiempo entre cuadros (para que la velocidad no dependa de los FPS)
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main() {
	glfwInit();
	//Verificación de compatibilidad
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);*/

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bojorquez Covarrubias Evans Martin", nullptr, nullptr);

	int screenWidth, screenHeight;

	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	//Verificación de errores de creacion  ventana
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificación de errores de inicialización de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}


	// Define las dimensiones del viewport
	glViewport(0, 0, screenWidth, screenHeight);


	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);

	// enable alpha support
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Build and compile our shader program
	Shader ourShader("Shader/core.vs", "Shader/core.frag");


	// Set up vertex data (and buffer(s)) and attribute pointers



	// use with Perspective Projection
	float vertices[] = {
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,//Front
		0.5f, -0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, 0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, 0.5f, 1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f,0.0f,

		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,//Back
		 0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		 0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		-0.5f,  0.5f,-0.5f, 0.0f, 1.0f,0.0f,
		-0.5f, -0.5f,-0.5f, 0.0f, 1.0f,0.0f,

		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		 0.5f,  -0.5f, 0.5f, 0.0f, 0.0f,1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,1.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,1.0f,

		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.2f,0.5f,
		-0.5f,  0.5f,  0.5f, 1.0f, 0.2f,0.5f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.2f,0.5f,
	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	glm::mat4 projection = glm::mat4(1);

	projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);//FOV, Radio de aspecto,znear,zfar
	//projection = glm::ortho(0.0f, (GLfloat)screenWidth, 0.0f, (GLfloat)screenHeight, 0.1f, 1000.0f);//Izq,Der,Fondo,Alto,Cercania,Lejania
	while (!glfwWindowShouldClose(window))
	{
		// Calculo del tiempo transcurrido desde el cuadro anterior
		float currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		Inputs(window);
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glm::mat4 model = glm::mat4(1);
		glm::mat4 view = glm::mat4(1);

		// Matrices para guardar el estado de cada pieza padre
		glm::mat4 modelA = glm::mat4(1.0f);   // cuerpo
		glm::mat4 modelC = glm::mat4(1.0f);   // cabeza
		glm::mat4 modelD = glm::mat4(1.0f);   // hocico
		glm::mat4 modelF = glm::mat4(1.0f);   // oreja izquierda
		glm::mat4 modelG = glm::mat4(1.0f);   // oreja derecha
		glm::mat4 modelJ = glm::mat4(1.0f);   // cola
		glm::mat4 modelL = glm::mat4(1.0f);   // pata delantera izquierda
		glm::mat4 modelM = glm::mat4(1.0f);   // pata trasera izquierda
		glm::mat4 modelN = glm::mat4(1.0f);   // pata delantera derecha
		glm::mat4 modelNN = glm::mat4(1.0f);  // pata trasera derecha (Ñ)


		view = glm::translate(view, glm::vec3(movX, movY, movZ));
		view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projecLoc = glGetUniformLocation(ourShader.Program, "projection");
		GLint colorLoc = glGetUniformLocation(ourShader.Program, "colorPieza");


		glUniformMatrix4fv(projecLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(VAO);

		//// Tabla de la mesa
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(3.0f, 0.1f, 2.0f)); // Ancho, grosor, profundidad
		//model = glm::translate(model, glm::vec3(0.0f, 0.6f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//// Patas de la mesa
		//// Pata 1
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f)); // Tamaño de la pata
		//model = glm::translate(model, glm::vec3(2.9f, -0.6f, 1.9f)); // Posición de la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Pata 2
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f)); // Tamaño de la pata
		//model = glm::translate(model, glm::vec3(-2.9f, -0.6f, 1.9f)); // Posición de la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Pata 3
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f)); // Tamaño de la pata
		//model = glm::translate(model, glm::vec3(-2.9f, -0.6f, -1.9f)); // Posición de la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Pata 4
		//model = glm::mat4(1.0f);
		//model = glm::scale(model, glm::vec3(0.1f, 0.6f, 0.1f)); // Tamaño de la pata
		//model = glm::translate(model, glm::vec3(2.9f, -0.6f, -1.9f)); // Posición de la pata
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		 //Zorro de Minecraft

		 //Jerarquizado:
		//Pieza A (Cuerpo)
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 1.8f, 2.0f));
		modelA = model;                                    // se guarda para los 7 hijos
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza B (Panza blanca)
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));  // deshace A
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 0.2f, 2.0f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza C (Cabeza)
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));  // deshace A
		model = glm::translate(model, glm::vec3(-3.0f, 0.2f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 3.0f));
		modelC = model;                                          // se guarda para D, F y G
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza D (Hocico Naranja Superior)
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));  // deshace C
		model = glm::translate(model, glm::vec3(-1.35f, -0.515f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.3f, 1.0f));
		modelD = model;                                          // se guarda para E
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza E (Hocico Blanco Inferior)
		model = modelD;
		model = glm::scale(model, glm::vec3(1.0f / 0.7f, 1.0f / 0.3f, 1.0f / 1.0f));  // deshace D
		model = glm::translate(model, glm::vec3(0.0f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.3f, 1.0f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pieza F (Oreja izquierda) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.50f, 1.15f, -1.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.75f));
		modelF = model;                                          // se guarda para H
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza H (Punta oreja izquierda) - hija de F
		model = modelF;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace F
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.75f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza G (Oreja derecha) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.50f, 1.15f, 1.125f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.75f));
		modelG = model;                                          // se guarda para I
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza I (Punta oreja derecha) - hija de G
		model = modelG;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace G
		model = glm::translate(model, glm::vec3(0.0f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.75f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//// ---------- RAMA DE LA COLA (cuelga de A) ----------

		//Pieza J (Cola) - hija de A
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));   // deshace A
		model = glm::translate(model, glm::vec3(3.75f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.5f, 1.2f, 1.2));
		modelJ = model;
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza K (Punta de la cola) - hija de J
		model = modelJ;
		model = glm::scale(model, glm::vec3(1.0f / 3.5f, 1.0f / 1.4f, 1.0f / 1.0f));   // deshace J
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.4f, 1.0f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);



		//// ---------- RAMA DE LAS PATAS (las 4 cuelgan de A) ----------

		//Pieza L (Pata delantera izquierda) - hija de A
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));   // deshace A
		model = glm::translate(model, glm::vec3(-1.2f, -1.65f, -0.75f));
		model = glm::scale(model, glm::vec3(0.5f, 1.1f, 0.5f));
		modelL = model;                                          // se guarda para O
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza O (Pezuna de L) - hija de L
		model = modelL;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 1.1f, 1.0f / 0.5f));   // deshace L
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.5f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza M (Pata trasera izquierda) - hija de A
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));   // deshace A
		model = glm::translate(model, glm::vec3(1.2f, -1.65f, -0.75f));
		model = glm::scale(model, glm::vec3(0.5f, 1.1f, 0.5f));
		modelM = model;                                          // se guarda para P
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza P (Pezuna de M) - hija de M
		model = modelM;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 1.1f, 1.0f / 0.5f));   // deshace M
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.5f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza N (Pata delantera derecha) - hija de A
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));   // deshace A
		model = glm::translate(model, glm::vec3(-1.2f, -1.65f, 0.75f));
		model = glm::scale(model, glm::vec3(0.5f, 1.1f, 0.5f));
		modelN = model;                                          // se guarda para Q
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza Q (Pezuna de N) - hija de N
		model = modelN;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 1.1f, 1.0f / 0.5f));   // deshace N
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.5f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza NN (Pata trasera derecha) - hija de A
		model = modelA;
		model = glm::scale(model, glm::vec3(1.0f / 4.0f, 1.0f / 1.8f, 1.0f / 2.0f));   // deshace A
		model = glm::translate(model, glm::vec3(1.2f, -1.65f, 0.75f));
		model = glm::scale(model, glm::vec3(0.5f, 1.1f, 0.5f));
		modelNN = model;                                         // se guarda para R
		glUniform3f(colorLoc, 0.85f, 0.45f, 0.15f);              // Naranja
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza R (Pezuna de NN) - hija de NN
		model = modelNN;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 1.1f, 1.0f / 0.5f));   // deshace NN
		model = glm::translate(model, glm::vec3(0.0f, -0.7f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.3f, 0.5f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Detalles:
		// Ojos:
		//Pieza V (Fondo blanco ojo izquierdo) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.75f, 0.10f, -1.15f));
		model = glm::scale(model, glm::vec3(0.6f, 0.5f, 0.8f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza S (Ojo izquierdo) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.90f, 0.10f, -1.34f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);


		//Pieza W (Fondo blanco ojo derecho) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.75f, 0.10f, 1.15f));
		model = glm::scale(model, glm::vec3(0.6f, 0.5f, 0.8f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza T (Ojo derecho) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(-0.90f, 0.10f, 1.34f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza U (Nariz) - hija de D
		model = modelD;
		model = glm::scale(model, glm::vec3(1.0f / 0.7f, 1.0f / 0.3f, 1.0f / 1.0f));   // deshace D
		model = glm::translate(model, glm::vec3(-0.38f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.4f));
		glUniform3f(colorLoc, 0.12f, 0.12f, 0.12f);              // Negro
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza X (Blanco izquierdo del hocico) - hija de D
		model = modelD;
		model = glm::scale(model, glm::vec3(1.0f / 0.7f, 1.0f / 0.3f, 1.0f / 1.0f));   // deshace D
		model = glm::translate(model, glm::vec3(-0.38f, 0.0f, -0.375f));
		model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.35f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza Y (Blanco derecho del hocico) - hija de D
		model = modelD;
		model = glm::scale(model, glm::vec3(1.0f / 0.7f, 1.0f / 0.3f, 1.0f / 1.0f));   // deshace D
		model = glm::translate(model, glm::vec3(-0.38f, 0.0f, 0.375f));
		model = glm::scale(model, glm::vec3(0.05f, 0.3f, 0.35f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza Z (Cara crema oreja izquierda) - hija de F
		model = modelF;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace F
		model = glm::translate(model, glm::vec3(-0.27f, 0.08f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.5f, 0.85f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza AA (Cara crema oreja derecha) - hija de G
		model = modelG;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace G
		model = glm::translate(model, glm::vec3(-0.27f, 0.08f, 0.0f));
		model = glm::scale(model, glm::vec3(0.08f, 0.5f, 0.85f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza AB (Interior oreja izquierda) - hija de F
		model = modelF;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace F
		model = glm::translate(model, glm::vec3(-0.29f, 0.0f, 0.22f));
		model = glm::scale(model, glm::vec3(0.08f, 0.3f, 0.4f));
		glUniform3f(colorLoc, 0.62f, 0.42f, 0.28f);              // Cafe
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza AC (Interior oreja derecha) - hija de G
		model = modelG;
		model = glm::scale(model, glm::vec3(1.0f / 0.5f, 1.0f / 0.3f, 1.0f / 0.75f));  // deshace G
		model = glm::translate(model, glm::vec3(-0.29f, 0.0f, -0.22f));
		model = glm::scale(model, glm::vec3(0.08f, 0.3f, 0.4f));
		glUniform3f(colorLoc, 0.62f, 0.42f, 0.28f);              // Cafe
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Pieza AF (Parte baja de la cabeza) - hija de C
		model = modelC;
		model = glm::scale(model, glm::vec3(1.0f / 2.0f, 1.0f / 2.0f, 1.0f / 3.0f));   // deshace C
		model = glm::translate(model, glm::vec3(0.0f, -0.98f, 0.0f));
		model = glm::scale(model, glm::vec3(2.04f, 0.15f, 3.04f));
		glUniform3f(colorLoc, 0.94f, 0.92f, 0.87f);              // Blanco crema
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);

	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	glfwTerminate();
	return EXIT_SUCCESS;
}

void Inputs(GLFWwindow* window) {
	// La velocidad se multiplica por deltaTime, asi el movimiento es igual
	// en cualquier equipo sin importar los cuadros por segundo
	float velocidad = 3.0f * deltaTime;   // 3 unidades por segundo
	float velRot = 90.0f * deltaTime;     // 90 grados por segundo

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);

	// Movimiento horizontal
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += velocidad;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= velocidad;

	// Subir y Bajar
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		movY += velocidad;
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		movY -= velocidad;

	// Acercar y Alejar
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= velocidad;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += velocidad;

	// Rotación sobre eje Y
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rot += velRot;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rot -= velRot;

	// Reiniciar la camara a su posicion inicial
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		movX = 0.0f;
		movY = 0.0f;
		movZ = -5.0f;
		rot = 0.0f;
	}
}

