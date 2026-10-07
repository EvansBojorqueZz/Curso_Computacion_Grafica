// Practica #8
// Bojorquez Covarrubias Evans Martin
// Fecha de entrega: 7 de octubre de 2026
// 321203018


// Std. Includes
#include <string>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void CursorEnterCallback(GLFWwindow* window, int entered);
void DoMovement();
GLuint LoadTexture(const char* path);


// Camera
Camera camera(glm::vec3(0.0f, 0.7f, 3.0f));
bool keys[1024];
GLfloat lastX = 640, lastY = 360;
bool firstMouse = true;

// Sensibilidad del mouse (entre mas pequeno, mas lenta gira la camara)
const GLfloat MOUSE_SENSITIVITY = 0.15f;

// Ciclo de dia y noche
// El sol y la luna giran en una orbita circular alrededor del escenario, siempre en lados opuestos.
// Manteniendo T avanza el tiempo y manteniendo G retrocede.
GLfloat anguloCiclo = glm::radians(45.0f);   // angulo del sol en la orbita (0 = horizonte derecho, 90 = mediodia)
const GLfloat RADIO_ORBITA = 2.5f;           // que tan lejos del centro gira el sol y la luna
const GLfloat Z_ORBITA = 0.0f;               // profundidad del plano en el que giran
const GLfloat VELOCIDAD_CICLO = 0.6f;        // radianes por segundo

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    glfwSetCursorEnterCallback(window, CursorEnterCallback);

    // GLFW Options
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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models
    // PERRITO
    Model dog((char*)"Models/RedDog.obj");
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    // PINTURAS
    Model pintura1((char*)"Models/Pintura1/pintura1.obj");
    Model pintura2((char*)"Models/Pintura2/pintura2.obj");

    // CABALLETE, PINCEL Y PALETA
    Model caballete((char*)"Models/Caballete/caballete.obj");
    Model pincel((char*)"Models/Pincel/pincel.obj");
    Model paleta((char*)"Models/Paleta/paleta.obj");

    // SOL Y LUNA (cada uno lleva asociada una fuente de luz)
    Model sol((char*)"Models/Sol/sol.obj");
    Model luna((char*)"Models/Luna/luna.obj");


    // Vertices del suelo (posicion, normal y coordenadas de textura)
    // Es un plano de 6 x 6 unidades a la altura del piso (y = -0.01 para no encimarse con la paleta y el pincel)
    // Las coordenadas de textura van de 0 a 3 para que la textura se repita 3 veces por lado
    float floorVertices[] = {
        // Posiciones             // Normales           // Coordenadas de textura
        -3.0f, -0.01f, -3.0f,     0.0f, 1.0f, 0.0f,     0.0f, 3.0f,
         3.0f, -0.01f, -3.0f,     0.0f, 1.0f, 0.0f,     3.0f, 3.0f,
         3.0f, -0.01f,  3.0f,     0.0f, 1.0f, 0.0f,     3.0f, 0.0f,

         3.0f, -0.01f,  3.0f,     0.0f, 1.0f, 0.0f,     3.0f, 0.0f,
        -3.0f, -0.01f,  3.0f,     0.0f, 1.0f, 0.0f,     0.0f, 0.0f,
        -3.0f, -0.01f, -3.0f,     0.0f, 1.0f, 0.0f,     0.0f, 3.0f
    };

    // VAO y VBO del suelo
    GLuint floorVBO, floorVAO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);

    // Load textures
    stbi_set_flip_vertically_on_load(true);
    // Textura del suelo
    GLuint floorTexture = LoadTexture("Models/Piso/piso.jpg");
    // Texturas del sol y de la luna (sus modelos no traen imagen propia, se cargan aparte)
    GLuint solTexture = LoadTexture("Models/Sol/sol.jpg");
    GLuint lunaTexture = LoadTexture("Models/Luna/luna.jpg");


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();


        // Posicion del sol y de la luna en la orbita
        // El sol esta en anguloCiclo y la luna en el lado opuesto (anguloCiclo + 180 grados)
        glm::vec3 posSol(RADIO_ORBITA * cos(anguloCiclo), RADIO_ORBITA * sin(anguloCiclo), Z_ORBITA);
        glm::vec3 posLuna(-posSol.x, -posSol.y, Z_ORBITA);

        // Que tanto es de dia (1) o de noche (0), segun la altura del sol
        // La transicion es suave cuando el sol esta cerca del horizonte (amanecer y atardecer)
        float factorDia = glm::clamp(0.5f + 2.0f * (float)sin(anguloCiclo), 0.0f, 1.0f);
        float factorNoche = 1.0f - factorDia;


        // Clear the colorbuffer
        // El cielo pasa de casi negro (noche) a azul (dia)
        glm::vec3 cieloNoche(0.02f, 0.02f, 0.08f);
        glm::vec3 cieloDia(0.53f, 0.81f, 0.92f);
        glm::vec3 cielo = glm::mix(cieloNoche, cieloDia, factorDia);
        glClearColor(cielo.r, cielo.g, cielo.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // Set lights properties
        // Luz del sol (calida, simula el modo dia). Su intensidad depende de que tan alto este el sol
        glm::vec3 solAmbient = glm::vec3(0.3f, 0.27f, 0.2f) * factorDia;
        glm::vec3 solDiffuse = glm::vec3(1.0f, 0.85f, 0.6f) * factorDia;
        glm::vec3 solSpecular = glm::vec3(0.6f, 0.55f, 0.4f) * factorDia;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), posSol.x, posSol.y, posSol.z);
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.ambient"), 1, glm::value_ptr(solAmbient));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1, glm::value_ptr(solDiffuse));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light.specular"), 1, glm::value_ptr(solSpecular));

        // Second light properties
        // Luz de la luna (tenue y azulada, simula el modo noche). Se enciende cuando el sol se oculta
        glm::vec3 lunaAmbient = glm::vec3(0.12f, 0.13f, 0.2f) * factorNoche;
        glm::vec3 lunaDiffuse = glm::vec3(0.45f, 0.5f, 0.7f) * factorNoche;
        glm::vec3 lunaSpecular = glm::vec3(0.3f, 0.35f, 0.5f) * factorNoche;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), posLuna.x, posLuna.y, posLuna.z);
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 1, glm::value_ptr(lunaAmbient));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 1, glm::value_ptr(lunaDiffuse));
        glUniform3fv(glGetUniformLocation(lightingShader.Program, "light2.specular"), 1, glm::value_ptr(lunaSpecular));



        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.3f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);


        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        glm::mat4 model(1);

        // Suelo (plano con textura debajo de todo el escenario)
        model = glm::mat4(1);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "texture_diffuse"), 0);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Perrito (al centro, apoyado en el suelo)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, 0.3335f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Pintura 1 (horizontal, colgada al fondo a la derecha)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.8f, 0.6f, -1.5f));
        model = glm::scale(model, glm::vec3(0.00025f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pintura1.Draw(lightingShader);

        // Pintura 2 (vertical, colgada al fondo a la izquierda)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.4f, 0.5f, -1.5f));
        model = glm::scale(model, glm::vec3(0.006f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pintura2.Draw(lightingShader);

        // Caballete (a la izquierda, girado para mirar a la camara)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.3f, 0.0f, -0.3f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caballete.Draw(lightingShader);

        // Paleta (en el piso, junto al caballete)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.7f, 0.0f, 0.6f));
        model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.004f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        paleta.Draw(lightingShader);

        // Pincel (en el piso, junto a la paleta)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.35f, 0.0f, 0.9f));
        model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.12f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pincel.Draw(lightingShader);




        // Sol y luna (se dibujan con el shader de modelos, que muestra la textura sin calculo de iluminacion,
        // por eso se ven con brillo propio, como si emitieran luz)
        // Cuando bajan del horizonte quedan debajo del suelo y dejan de dibujarse
        shader.Use();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glActiveTexture(GL_TEXTURE0);

        // Sol (en la misma posicion que la primera fuente de luz)
        if (posSol.y > 0.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, posSol);
            model = glm::scale(model, glm::vec3(0.1f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, solTexture);
            sol.Draw(shader);
        }

        // Luna (en la misma posicion que la segunda fuente de luz)
        if (posLuna.y > 0.0f)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, posLuna);
            model = glm::scale(model, glm::vec3(0.1f));
            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glBindTexture(GL_TEXTURE_2D, lunaTexture);
            luna.Draw(shader);
        }
        glBindTexture(GL_TEXTURE_2D, 0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);

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

    // Avanzar el ciclo de dia y noche manteniendo T (el sol y la luna se trasladan por su orbita)
    if (keys[GLFW_KEY_T])
    {
        anguloCiclo += VELOCIDAD_CICLO * deltaTime;
    }

    // Retroceder el ciclo de dia y noche manteniendo G
    if (keys[GLFW_KEY_G])
    {
        anguloCiclo -= VELOCIDAD_CICLO * deltaTime;
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
}

// Cuando el cursor entra a la ventana, se reinicia la referencia del mouse
// para que la camara no de un salto brusco
void CursorEnterCallback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        firstMouse = true;
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
        return;   // no mover la camara en el primer evento
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    // Reducir la sensibilidad
    xOffset *= MOUSE_SENSITIVITY;
    yOffset *= MOUSE_SENSITIVITY;

    camera.ProcessMouseMovement(xOffset, yOffset);
}


// Carga una imagen como textura de OpenGL y regresa su ID
// (se usa para el suelo, el sol y la luna)
GLuint LoadTexture(const char* path)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int textureWidth, textureHeight, nrChannels;
    unsigned char* image = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
    if (image)
    {
        // Si la imagen tiene transparencia (4 canales) se carga como RGBA, si no como RGB
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << path << std::endl;
    }
    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return textureID;
}