//Práctica 2
//Bojorquez Covarrubias Evans Martin
//Fecha de entrega: 28 de Agosto del 2026
//No. Cuenta: 321203018

#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

// Macros para normalizar coordenadas en base a tu imagen de 934x1466
#define X_GL(x) (((x) / 934.0f) * 2.0f - 1.0f)
#define Y_GL(y) (1.0f - ((y) / 1466.0f) * 2.0f)

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 467, HEIGHT = 733;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Bojorquez Covarrubias Evans Martin", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// PEGAR ESTO (Tus nuevos vértices e índices de Deadpool):
	float vertices[] = {
		// --- POLÍGONO A ---
		X_GL(349), Y_GL(631), 0.0f,   0.8f, 0.0f, 0.0f, // 0 (Vértice 1)
		X_GL(490), Y_GL(620), 0.0f,   0.8f, 0.0f, 0.0f, // 1 (Vértice 2)
		X_GL(326), Y_GL(483), 0.0f,   0.8f, 0.0f, 0.0f, // 2 (Vértice 3)
		X_GL(426), Y_GL(411), 0.0f,   0.8f, 0.0f, 0.0f, // 3 (Vértice 4)
		X_GL(497), Y_GL(480), 0.0f,   0.8f, 0.0f, 0.0f, // 4 (Vértice 5)

		// --- POLÍGONO B ---
		X_GL(349), Y_GL(631), 0.0f,   0.6f, 0.0f, 0.0f, // 5 (Vértice 6)
		X_GL(326), Y_GL(483), 0.0f,   0.6f, 0.0f, 0.0f, // 6 (Vértice 7)
		X_GL(204), Y_GL(425), 0.0f,   0.6f, 0.0f, 0.0f, // 7 (Vértice 8)
		X_GL(237), Y_GL(302), 0.0f,   0.6f, 0.0f, 0.0f, // 8 (Vértice 9)
		X_GL(206), Y_GL(161), 0.0f,   0.6f, 0.0f, 0.0f, // 9 (Vértice 10)
		X_GL(263), Y_GL(99),  0.0f,   0.6f, 0.0f, 0.0f, // 10 (Vértice 11)
		X_GL(270), Y_GL(168), 0.0f,   0.6f, 0.0f, 0.0f, // 11 (Vértice 12)

		// Polígono C
		X_GL(490), Y_GL(620), 0.0f,   0.6f, 0.0f, 0.0f, // 12 (Vértice 13)
		X_GL(497), Y_GL(480), 0.0f,   0.6f, 0.0f, 0.0f, // 13 (Vértice 14)
		X_GL(578), Y_GL(303), 0.0f,   0.6f, 0.0f, 0.0f, // 14 (Vértice 15)
		X_GL(527), Y_GL(162), 0.0f,   0.6f, 0.0f, 0.0f, // 15 (Vértice 16)
		X_GL(529), Y_GL(93), 0.0f,   0.6f, 0.0f, 0.0f, // 16 (Vértice 17)
		X_GL(589), Y_GL(210),  0.0f,   0.6f, 0.0f, 0.0f, // 17 (Vértice 18)
		X_GL(600), Y_GL(382), 0.0f,   0.6f, 0.0f, 0.0f, // 18 (Vértice 19)

		// Polígono D 
		X_GL(263), Y_GL(99), 0.0f,   0.6f, 0.0f, 0.0f, // 19 (Vértice 20)
		X_GL(529), Y_GL(93), 0.0f,   0.6f, 0.0f, 0.0f, // 20 (Vértice 21)
		X_GL(394), Y_GL(38), 0.0f,   0.6f, 0.0f, 0.0f, // 21 (Vértice 22)

		// Polígono E
		X_GL(419), Y_GL(95), 0.0f,   0.6f, 0.0f, 0.0f, // 22 (Vértice 23)
		X_GL(529), Y_GL(93), 0.0f,   0.6f, 0.0f, 0.0f, // 23 (Vértice 24)
		X_GL(527), Y_GL(162), 0.0f,   0.6f, 0.0f, 0.0f, // 24 (Vértice 25)

		// Polígono F
		X_GL(326), Y_GL(483), 0.0f,   0.6f, 0.0f, 0.0f, // 25 (Vértice 26)
		X_GL(405), Y_GL(322), 0.0f,   0.6f, 0.0f, 0.0f, // 26 (Vértice 27)
		X_GL(425), Y_GL(330), 0.0f,   0.6f, 0.0f, 0.0f, // 27 (Vértice 28)
		X_GL(426), Y_GL(411), 0.0f,   0.6f, 0.0f, 0.0f, // 28 (Vértice 29)

		// Polígono G
		X_GL(426), Y_GL(411), 0.0f,   0.6f, 0.0f, 0.0f, // 29 (Vértice 30)
		X_GL(425), Y_GL(330), 0.0f,   0.6f, 0.0f, 0.0f, // 30 (Vértice 31)
		X_GL(436), Y_GL(326), 0.0f,   0.6f, 0.0f, 0.0f, // 31 (Vértice 32)
		X_GL(515), Y_GL(443), 0.0f,   0.6f, 0.0f, 0.0f, // 32 (Vértice 33)
		X_GL(497), Y_GL(480), 0.0f,   0.6f, 0.0f, 0.0f, // 33 (Vértice 34)

		// Polígono H
		X_GL(515), Y_GL(443), 0.0f,   0.1f, 0.12f, 0.18f, // 34 (Vértice 35)
		X_GL(514), Y_GL(371), 0.0f,   0.1f, 0.12f, 0.18f, // 35 (Vértice 36)
		X_GL(436), Y_GL(326), 0.0f,   0.1f, 0.12f, 0.18f, // 36 (Vértice 37)

		// Polígono I
		X_GL(326), Y_GL(483), 0.0f,   0.1f, 0.12f, 0.18f, // 37 (Vértice 38)
		X_GL(295), Y_GL(364), 0.0f,   0.1f, 0.12f, 0.18f, // 38 (Vértice 39)
		X_GL(360), Y_GL(362), 0.0f,   0.1f, 0.12f, 0.18f, // 39 (Vértice 40)
		X_GL(405), Y_GL(322), 0.0f,   0.1f, 0.12f, 0.18f, // 40 (Vértice 41)

		// Polígono J
		X_GL(515), Y_GL(443), 0.0f,   0.15f, 0.15f, 0.18f, // 41 (Vértice 42)
		X_GL(514), Y_GL(371), 0.0f,   0.15f, 0.15f, 0.18f, // 42 (Vértice 43)
		X_GL(561), Y_GL(323), 0.0f,   0.15f, 0.15f, 0.18f, // 43 (Vértice 44)
		X_GL(529), Y_GL(279), 0.0f,   0.15f, 0.15f, 0.18f, // 44 (Vértice 45)
		X_GL(527), Y_GL(162), 0.0f,   0.15f, 0.15f, 0.18f, // 45 (Vértice 46)
		X_GL(578), Y_GL(304), 0.0f,   0.15f, 0.15f, 0.18f, // 46 (Vértice 47)

		// Polígono K
		X_GL(326), Y_GL(483), 0.0f,   0.15f, 0.15f, 0.18f, // 47 (Vértice 43)
		X_GL(237), Y_GL(302), 0.0f,   0.15f, 0.15f, 0.18f, // 48 (Vértice 44)
		X_GL(270), Y_GL(168), 0.0f,   0.15f, 0.15f, 0.18f, // 49 (Vértice 45)
		X_GL(295), Y_GL(282), 0.0f,   0.15f, 0.15f, 0.18f, // 50 (Vértice 46)
		X_GL(271), Y_GL(329), 0.0f,   0.15f, 0.15f, 0.18f, // 51 (Vértice 47)
		X_GL(295), Y_GL(364), 0.0f,   0.15f, 0.15f, 0.18f, // 52 (Vértice 48)

		// Polígono L
		X_GL(295), Y_GL(282), 0.0f,   0.1f, 0.12f, 0.18f, // 53 (Vértice 49)
		X_GL(270), Y_GL(168), 0.0f,   0.1f, 0.12f, 0.18f, // 54 (Vértice 50)
		X_GL(365), Y_GL(203), 0.0f,   0.1f, 0.12f, 0.18f, // 55 (Vértice 51)
		X_GL(405), Y_GL(322), 0.0f,   0.1f, 0.12f, 0.18f, // 56 (Vértice 52)

		// Polígono M
		X_GL(436), Y_GL(326), 0.0f,   0.1f, 0.12f, 0.18f, // 57 (Vértice 53)
		X_GL(483), Y_GL(180), 0.0f,   0.1f, 0.12f, 0.18f, // 58 (Vértice 54)
		X_GL(527), Y_GL(162), 0.0f,   0.1f, 0.12f, 0.18f, // 59 (Vértice 55)
		X_GL(529), Y_GL(279), 0.0f,   0.1f, 0.12f, 0.18f, // 60 (Vértice 56)

		// Polígono N
		X_GL(425), Y_GL(330), 0.0f,   0.6f, 0.0f, 0.0f, // 61 (Vértice 57)
		X_GL(405), Y_GL(322), 0.0f,   0.6f, 0.0f, 0.0f, // 62 (Vértice 58)
		X_GL(365), Y_GL(203), 0.0f,   0.6f, 0.0f, 0.0f, // 63 (Vértice 59)
		X_GL(270), Y_GL(168), 0.0f,   0.6f, 0.0f, 0.0f, // 64 (Vértice 60)
		X_GL(263), Y_GL(99), 0.0f,   0.6f, 0.0f, 0.0f, // 65 (Vértice 61)
		X_GL(419), Y_GL(95),  0.0f,   0.6f, 0.0f, 0.0f, // 66 (Vértice 62)
		X_GL(527), Y_GL(162), 0.0f,   0.6f, 0.0f, 0.0f, // 67 (Vértice 63)
		X_GL(483), Y_GL(180),  0.0f,   0.6f, 0.0f, 0.0f, // 68 (Vértice 64)
		X_GL(436), Y_GL(326), 0.0f,   0.6f, 0.0f, 0.0f, // 69 (Vértice 65)



	};
	//color negro metálico 0.15f, 0.15f, 0.18f
	// color negro sombra 0.1f, 0.12f, 0.18f
	// color negro mas sombra 0.22f, 0.22f, 0.22f
	unsigned int indices[] = {
		// POLÍGONO A (3 triángulos = 9 índices)
		0, 1, 4,
		0, 4, 3,
		0, 3, 2,

		// POLÍGONO B (5 triángulos = 15 índices)
		5, 7, 6,  // Conecta abajo (v6, v8, v7)
		6, 7, 8,  // Conecta el borde inferior (v7, v8, v9)
		7, 8, 9, // Conecta el centro (v8, v9, v10)
		11, 8, 9, // Conecta el borde superior (v12, v9, v10)
		11, 9, 10, // Conecta la punta de arriba (v12, v10, v11)

		// POLÍGONO C 
		12, 13, 18,
		13, 14, 18,
		15, 17, 18,
		17, 15, 16,

		// POLÍGONO D
		19, 20, 21,

		// POLÍGONO E
		22, 23, 24,

		// POLÍGONO F
		25, 26, 27,
		25, 27, 28,

		// POLÍGONO G
		29, 30, 31,
		29, 31, 32,
		29, 32, 33,

		// POLÍGONO H
		34, 35, 36,

		// POLÍGONO I
		37, 38, 39,
		37, 39, 40,

		// POLÍGONO J
		41, 42, 43,
		41, 43, 46,
		43, 44, 45,
		43, 45, 46,

		// POLÍGONO K
		47, 48, 52,
		52, 48, 51,
		51, 48, 49,
		49, 50, 51,

		// POLÍGONO L
		53, 54, 55,
		53, 55, 56,

		// POLÍGONO M
		57, 58, 59,
		57, 59, 60,

		// POLÍGONO N
		61, 62, 63,
		63, 64,65,
		63, 65, 66,
		66, 67, 68,
		63, 66, 68,
		68, 63, 69, 
		69, 61, 63
		
		

	};
	//para calcular la cantidad de indices
	unsigned int cantidadIndices = sizeof(indices) / sizeof(indices[0]); 


	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
        ourShader.Use();
        glBindVertexArray(VAO);


        //glPointSize(10);
		//glDrawArrays(GL_POINTS,0,1);
        
		//glDrawArrays(GL_LINES,0,2);
		//glDrawArrays(GL_LINE_LOOP,0,4);
        
		//glDrawArrays(GL_TRIANGLES,0,3);
        glDrawElements(GL_TRIANGLES, cantidadIndices,GL_UNSIGNED_INT,0);

        
        
        glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}