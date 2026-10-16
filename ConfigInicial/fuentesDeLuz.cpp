// Practica #9
// Bojorquez Covarrubias Evans Martin
// Fecha de entrega: 15 de octubre de 2026
// 321203018


// Std. Includes
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>

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
const GLuint WIDTH = 1280, HEIGHT = 720;   // resolucion de la ventana
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Caja que envuelve a un modelo (minimo y maximo de sus vertices)
struct Bounds
{
    glm::vec3 min;
    glm::vec3 max;
};

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void CursorEnterCallback(GLFWwindow* window, int entered);
void DoMovement();
Bounds GetOBJBounds(const char* path);
GLfloat GroundY(const Bounds& b, GLfloat scale, GLfloat floorTop);
GLuint LoadTexture(const char* path, int& width, int& height, GLint wrap);
GLuint CreateQuadVAO(const float* data, size_t size);
glm::vec3 GetTopCenter(const char* path, const glm::mat4& transform, GLfloat band, GLfloat& topY);
void SetPointLight(Shader& shader, int index, glm::vec3 position, glm::vec3 color, bool on);
void SetSpotLight(Shader& shader, int index, glm::vec3 position, glm::vec3 direction, glm::vec3 color,
    GLfloat cutOff, GLfloat outerCutOff, bool on);


// Camera
Camera camera(glm::vec3(0.0f, 0.7f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Sensibilidad del mouse (entre mas pequeno, mas lenta gira la camara)
const GLfloat MOUSE_SENSITIVITY = 0.15f;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// ---------------- LUCES ----------------
// [0] direccional (ventana)   [1] reflector pintura 1   [2] reflector pintura 2
// [3] puntual de la lampara   [4] reflector linterna de la camara
bool lightOn[5] = { true, true, true, true, false };
const char* LIGHT_NAMES[5] = { "Direccional (ventana)", "Reflector pintura 1", "Reflector pintura 2",
                               "Puntual (lampara)", "Linterna de la camara" };


// ---------------- ESCENARIO ----------------
// Rutas de los archivos (relativas a la carpeta ConfigInicial)
const char* PISO_PATH = "Models/Piso/piso.png";      // imagen del piso de madera (cambia el nombre si tu imagen se llama distinto)
const char* PARED_PATH = "Models/Pared/pared.png";   // imagen de la pared con ventana (PNG con la ventana transparente)
const char* PARED_LATERAL_PATH = "Models/Pared/pared_lateral.png";   // paredes de los lados
const char* TECHO_PATH = "Models/Pared/techo.png";                   // techo

// Direccion del sol: entra por la ventana (atras a la izquierda) hacia el frente, la derecha y abajo
const glm::vec3 SUN_DIR = glm::normalize(glm::vec3(0.35f, -0.75f, 0.6f));
const glm::vec3 SUN_COLOR(1.0f, 0.9f, 0.7f);


const GLfloat FLOOR_SIZE = 4.5f;   // ancho y largo del piso en unidades
const GLfloat FLOOR_TILES = 3.0f;  // cuantas veces se repite la imagen del piso en cada direccion
const GLfloat WALL_Z = -1.6f;      // posicion de la pared en Z (justo detras de las pinturas)

// Riel de reflectores (truss) colgado cerca del techo, frente a la pared
const GLfloat TRUSS_SCALE = 0.9f;     // el modelo mide 5 de largo -> queda de 4.5, igual que el piso
const GLfloat TRUSS_TOP_Y = 2.15f;    // altura de la parte superior del riel
const GLfloat TRUSS_Z = -0.6f;        // distancia del riel a la camara (la pared esta en -1.6)
const GLfloat SPOT_SCALE = 1.2f;      // escala del reflector y su soporte
const GLfloat HOLDER_PIVOT = 0.28f;   // distancia del tope del soporte al eje del reflector (medida del .obj)

// Lampara de piso (entre el perro y la paleta, un poco hacia atras)
const char* LAMP_PATH = "Models/Lampara/lampara.obj";
const GLfloat LAMP_HEIGHT = 1.3f;                      // altura final de la lampara en unidades
const glm::vec3 LAMP_POS(-0.5f, 0.0f, 0.0f);           // posicion de su base en el piso
const bool LAMP_UPSIDE_DOWN = true;                    // el modelo viene de cabeza: se voltea 180 grados

// Cuadro para la pared (posicion, normal, coordenadas de textura)
// La coordenada V va invertida porque SOIL carga la imagen de arriba hacia abajo
float wallVertices[] = {
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f
};

// Cuadro para el piso, acostado en el plano XZ con la normal hacia arriba
// Las coordenadas de textura llegan hasta FLOOR_TILES para que la madera se repita
float floorVertices[] = {
    -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,        FLOOR_TILES,
     0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   FLOOR_TILES, FLOOR_TILES,
     0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   FLOOR_TILES, 0.0f,
     0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   FLOOR_TILES, 0.0f,
    -0.5f, 0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f,        0.0f,
    -0.5f, 0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f,        FLOOR_TILES
};



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

    // Sincronizacion vertical: limita los FPS a la frecuencia del monitor
    glfwSwapInterval(1);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetCursorEnterCallback(window, CursorEnterCallback);

    // El mouse queda dentro de la escena (oculto) para mirar libremente; ESC cierra el programa
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

    // Shader de iluminacion del previo 9
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");

    // Load models
    // PERRITO
    Model dog((char*)"Models/RedDog.obj");
    // PINTURAS
    Model pintura1((char*)"Models/Pintura1/pintura1.obj");
    Model pintura2((char*)"Models/Pintura2/pintura2.obj");
    // CABALLETE, PINCEL Y PALETA
    Model caballete((char*)"Models/Caballete/caballete.obj");
    Model pincel((char*)"Models/Pincel/pincel.obj");
    // REFLECTORES: riel, soporte y cuerpo del reflector
    Model truss((char*)"Models/Spotlight/SpotlightStructure.obj");
    Model holder((char*)"Models/Spotlight/SpotlightHolder.obj");
    Model spot((char*)"Models/Spotlight/Spotlight.obj");
    // LAMPARA DE PISO
    Model lampara((char*)LAMP_PATH);
    Model paleta((char*)"Models/Paleta/paleta.obj");


    // ---------- Dimensiones de los modelos ----------
    // Se leen los vertices de cada .obj para saber su tamano real y colocarlos al ras del piso.
    // Las medidas se imprimen en la consola.
    std::cout << "\n===== Dimensiones de los modelos (sin escalar) =====" << std::endl;
    Bounds bDog = GetOBJBounds("Models/RedDog.obj");
    Bounds bCaballete = GetOBJBounds("Models/Caballete/caballete.obj");
    Bounds bPaleta = GetOBJBounds("Models/Paleta/paleta.obj");
    Bounds bPincel = GetOBJBounds("Models/Pincel/pincel.obj");
    Bounds bPintura1 = GetOBJBounds("Models/Pintura1/pintura1.obj");
    Bounds bPintura2 = GetOBJBounds("Models/Pintura2/pintura2.obj");
    Bounds bTruss = GetOBJBounds("Models/Spotlight/SpotlightStructure.obj");
    Bounds bSpot = GetOBJBounds("Models/Spotlight/Spotlight.obj");
    Bounds bLampara = GetOBJBounds(LAMP_PATH);
    std::cout << "====================================================\n" << std::endl;

    // Altura de la superficie del piso (aqui se apoyan los objetos)
    GLfloat floorTop = 0.0f;

    // Escalas de cada objeto (las mismas de la practica 6)
    const GLfloat dogScale = 1.0f;
    const GLfloat caballeteScale = 0.5f;
    const GLfloat paletaScale = 0.004f;
    const GLfloat pincelScale = 0.12f;

    // Altura a la que hay que trasladar cada modelo para que su parte mas baja toque el piso
    GLfloat dogGround = GroundY(bDog, dogScale, floorTop);
    GLfloat caballeteGround = GroundY(bCaballete, caballeteScale, floorTop);
    GLfloat paletaGround = GroundY(bPaleta, paletaScale, floorTop);
    GLfloat pincelGround = GroundY(bPincel, pincelScale, floorTop);

    // Posicion y escala de las pinturas (las mismas de la practica 6)
    const glm::vec3 pintura1Pos(0.8f, floorTop + 0.6f, -1.5f);
    const glm::vec3 pintura2Pos(-0.4f, floorTop + 0.5f, -1.5f);
    const GLfloat pintura1Scale = 0.00025f;
    const GLfloat pintura2Scale = 0.006f;

    // Centro real de cada pintura en el mundo (a donde apuntara cada reflector)
    glm::vec3 pinturaCenter[2] = {
        pintura1Pos + (bPintura1.min + bPintura1.max) * 0.5f * pintura1Scale,
        pintura2Pos + (bPintura2.min + bPintura2.max) * 0.5f * pintura2Scale
    };


    // ---------- Riel y reflectores ----------
    // El riel se traslada para que su parte superior quede en TRUSS_TOP_Y
    GLfloat trussY = TRUSS_TOP_Y - bTruss.max.y * TRUSS_SCALE;
    GLfloat trussBottom = trussY + bTruss.min.y * TRUSS_SCALE;   // de aqui cuelgan los soportes

    // Centro del cuerpo del reflector en sus coordenadas locales (para girarlo sobre su centro)
    glm::vec3 spotCenterLocal = (bSpot.min + bSpot.max) * 0.5f;

    // Para cada reflector: punto donde gira (eje del soporte), direccion hacia su pintura y angulos
    glm::vec3 spotPivot[2], spotDir[2];
    GLfloat spotYaw[2], spotPitch[2];
    for (int i = 0; i < 2; i++)
    {
        spotPivot[i] = glm::vec3(pinturaCenter[i].x, trussBottom - HOLDER_PIVOT * SPOT_SCALE, TRUSS_Z);
        spotDir[i] = glm::normalize(pinturaCenter[i] - spotPivot[i]);
        // El lente del modelo mira hacia +Z: se gira en Y (izquierda/derecha) y luego en X (hacia abajo)
        spotYaw[i] = atan2(spotDir[i].x, spotDir[i].z);
        spotPitch[i] = asin(-spotDir[i].y);
    }


    // ---------- Lampara de piso ----------
    glm::vec3 lampSize = bLampara.max - bLampara.min;
    // Si el modelo viene acostado (su eje mas largo es Z), se levanta girandolo -90 grados en X
    bool lampZUp = lampSize.z > lampSize.y * 1.2f;
    GLfloat lampTall = lampZUp ? lampSize.z : lampSize.y;
    GLfloat lampScale = (lampTall > 0.0f) ? LAMP_HEIGHT / lampTall : 1.0f;
    // Despues del giro, la Y minima del modelo pasa a ser su Z minima
    GLfloat lampMinY = lampZUp ? bLampara.min.z : bLampara.min.y;
    // Si se voltea 180 grados, la parte que queda abajo es la que antes era la Y maxima
    if (LAMP_UPSIDE_DOWN) lampMinY = -(lampZUp ? bLampara.max.z : bLampara.max.y);
    glm::vec3 lampCenter = (bLampara.min + bLampara.max) * 0.5f;
    // Matriz de la lampara (no se mueve, se calcula una sola vez)
    glm::mat4 lampModel(1);
    lampModel = glm::translate(lampModel, glm::vec3(LAMP_POS.x, floorTop - lampMinY * lampScale, LAMP_POS.z));
    if (LAMP_UPSIDE_DOWN) lampModel = glm::rotate(lampModel, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (lampZUp) lampModel = glm::rotate(lampModel, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    lampModel = glm::scale(lampModel, glm::vec3(lampScale));
    // Centra la lampara en X y en su eje de profundidad para que LAMP_POS sea el centro de la base
    if (lampZUp) lampModel = glm::translate(lampModel, glm::vec3(-lampCenter.x, -lampCenter.y, 0.0f));
    else         lampModel = glm::translate(lampModel, glm::vec3(-lampCenter.x, 0.0f, -lampCenter.z));

    // Foco de la lampara: se busca el centro de la pantalla (la parte mas alta del modelo ya colocado)
    // y la luz se pone un poco abajo de su borde superior, adentro de la pantalla
    GLfloat lampTopY = 0.0f;
    glm::vec3 shadeCenter = GetTopCenter(LAMP_PATH, lampModel, LAMP_HEIGHT * 0.15f, lampTopY);
    glm::vec3 lampBulb(shadeCenter.x, lampTopY - LAMP_HEIGHT * 0.12f, shadeCenter.z);

    // La luz de cada reflector sale del frente del lente (el lente esta en +Z del modelo)
    glm::vec3 spotLens[2];
    for (int i = 0; i < 2; i++)
        spotLens[i] = spotPivot[i] + spotDir[i] * ((bSpot.max.z - spotCenterLocal.z) * SPOT_SCALE);

    std::cout << "Controles de luces: 1 direccional | 2 y 3 reflectores | 4 lampara | 5 linterna" << std::endl;
    for (int i = 0; i < 5; i++)
        std::cout << "   Luz " << (i + 1) << " - " << LIGHT_NAMES[i] << ": " << (lightOn[i] ? "ENCENDIDA" : "APAGADA") << std::endl;
    std::cout << "Lampara: escala " << lampScale << (lampZUp ? " (venia acostada, se giro -90 en X)" : "") << std::endl;


    // ---------- Piso y pared (planos con imagen) ----------
    GLuint floorVAO = CreateQuadVAO(floorVertices, sizeof(floorVertices));
    GLuint wallVAO = CreateQuadVAO(wallVertices, sizeof(wallVertices));

    int floorImgW = 1, floorImgH = 1;
    GLuint floorTexture = LoadTexture(PISO_PATH, floorImgW, floorImgH, GL_REPEAT);       // se repite como mosaico
    // Si no existe como .png, se intenta como .jpg y .jpeg
    if (floorTexture == 0) floorTexture = LoadTexture("Models/Piso/piso.jpg", floorImgW, floorImgH, GL_REPEAT);
    if (floorTexture == 0) floorTexture = LoadTexture("Models/Piso/piso.jpeg", floorImgW, floorImgH, GL_REPEAT);

    int wallImgW = 1, wallImgH = 1;
    GLuint wallTexture = LoadTexture(PARED_PATH, wallImgW, wallImgH, GL_CLAMP_TO_EDGE);  // una sola vez

    // La pared mide lo mismo que el ancho del piso; su altura respeta la proporcion de la imagen
    GLfloat wallWidth = FLOOR_SIZE;
    GLfloat wallHeight = wallWidth * (GLfloat)wallImgH / (GLfloat)wallImgW;

    // Paredes laterales y techo
    int sideImgW = 1, sideImgH = 1, ceilImgW = 1, ceilImgH = 1;
    GLuint sideTexture = LoadTexture(PARED_LATERAL_PATH, sideImgW, sideImgH, GL_CLAMP_TO_EDGE);
    GLuint ceilTexture = LoadTexture(TECHO_PATH, ceilImgW, ceilImgH, GL_REPEAT);



    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Unidades de textura del material
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

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

        // Color de fondo: es el "cielo" que se ve a traves de la ventana
        glClearColor(0.75f, 0.88f, 1.0f, 1.0f);   // cielo de dia, claro
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();

        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"),
            camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // ===== LUZ 1: DIRECCIONAL (sol que entra por la ventana) =====
        // Viene desde la ventana (atras a la izquierda) hacia el frente, la derecha y abajo.
        // Si esta apagada queda un ambiente minimo para que el cuarto no sea totalmente negro.
        // El ambiental tambien es calido (color del sol) para que toda la sala se sienta iluminada por la ventana
        glm::vec3 dirAmbient = lightOn[0] ? SUN_COLOR * 0.35f : glm::vec3(0.03f);
        glm::vec3 dirDiffuse = lightOn[0] ? SUN_COLOR * 1.8f : glm::vec3(0.0f);
        glm::vec3 dirSpecular = lightOn[0] ? SUN_COLOR * 0.4f : glm::vec3(0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), SUN_DIR.x, SUN_DIR.y, SUN_DIR.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), dirAmbient.x, dirAmbient.y, dirAmbient.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), dirDiffuse.x, dirDiffuse.y, dirDiffuse.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), dirSpecular.x, dirSpecular.y, dirSpecular.z);

        // ===== LUCES 2 y 3: REFLECTORES sobre las pinturas =====
        // Salen del lente de cada reflector y apuntan al centro de su pintura (cono de 14 a 20 grados)
        for (int i = 0; i < 2; i++)
            SetSpotLight(lightingShader, i, spotLens[i], spotDir[i], glm::vec3(1.0f, 0.95f, 0.85f), 14.0f, 20.0f, lightOn[1 + i]);

        // ===== LUZ 4: PUNTUAL en el foco de la lampara (luz calida) =====
        // Luz azul turquesa para que contraste con el sol calido de la ventana.
        // El color va arriba de 1 para que sea un foco intenso que ilumine el piso, el perro y el caballete
        SetPointLight(lightingShader, 0, lampBulb, glm::vec3(0.2f, 1.6f, 2.0f), lightOn[3]);
        // Las otras puntuales del shader no se usan
        for (int i = 1; i < 4; i++)
            SetPointLight(lightingShader, i, glm::vec3(0.0f), glm::vec3(0.0f), false);

        // ===== LUZ 5: REFLECTOR LINTERNA (sale de la camara hacia donde se mira) =====
        SetSpotLight(lightingShader, 2, camera.GetPosition(), camera.GetFront(), glm::vec3(1.5f, 1.5f, 1.4f), 10.0f, 15.0f, lightOn[4]);

        // Brillo del material
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // Los modelos del estudio son opacos
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);


        // Matrices de camara
        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        glm::mat4 model(1);

        // Piso de madera (su borde trasero queda pegado a la pared)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, floorTop, WALL_Z + FLOOR_SIZE * 0.5f));
        model = glm::scale(model, glm::vec3(FLOOR_SIZE, 1.0f, FLOOR_SIZE));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Perrito (al centro, apoyado en el piso)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, dogGround, 0.5f));
        model = glm::scale(model, glm::vec3(dogScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // Pintura 1 (horizontal, colgada en la pared a la derecha)
        model = glm::mat4(1);
        model = glm::translate(model, pintura1Pos);
        model = glm::scale(model, glm::vec3(pintura1Scale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pintura1.Draw(lightingShader);

        // Pintura 2 (vertical, colgada en la pared a la izquierda)
        model = glm::mat4(1);
        model = glm::translate(model, pintura2Pos);
        model = glm::scale(model, glm::vec3(pintura2Scale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pintura2.Draw(lightingShader);

        // Caballete (a la izquierda, girado para mirar a la camara)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.3f, caballeteGround, -0.3f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(caballeteScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        caballete.Draw(lightingShader);

        // Paleta (en el piso, junto al caballete)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.7f, paletaGround, 0.6f));
        model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(paletaScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        paleta.Draw(lightingShader);

        // Pincel (en el piso, junto a la paleta)
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.35f, pincelGround, 0.9f));
        model = glm::rotate(model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(pincelScale));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        pincel.Draw(lightingShader);

        // Riel de reflectores
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, trussY, TRUSS_Z));
        model = glm::scale(model, glm::vec3(TRUSS_SCALE));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        truss.Draw(lightingShader);

        // Dos reflectores, cada uno apuntando a su pintura
        for (int i = 0; i < 2; i++)
        {
            // Soporte: cuelga del riel y solo gira en Y para quedar de frente a la pintura
            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(spotPivot[i].x, trussBottom, TRUSS_Z));
            model = glm::rotate(model, spotYaw[i], glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(SPOT_SCALE));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            holder.Draw(lightingShader);

            // Cuerpo del reflector: gira en Y y luego se inclina en X hacia la pintura
            model = glm::mat4(1);
            model = glm::translate(model, spotPivot[i]);
            model = glm::rotate(model, spotYaw[i], glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, spotPitch[i], glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(SPOT_SCALE));
            model = glm::translate(model, -spotCenterLocal);   // gira sobre su propio centro
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            spot.Draw(lightingShader);
        }

        // Lampara de piso (su base queda sobre el piso en LAMP_POS)
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(lampModel));
        lampara.Draw(lightingShader);


        // Paredes laterales: el mismo plano de la pared girado 90 grados para que mire hacia adentro
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sideTexture);
        glBindVertexArray(wallVAO);
        for (int lado = -1; lado <= 1; lado += 2)
        {
            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(lado * FLOOR_SIZE * 0.5f, floorTop + wallHeight * 0.5f, WALL_Z + FLOOR_SIZE * 0.5f));
            model = glm::rotate(model, glm::radians(-90.0f * lado), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(FLOOR_SIZE, wallHeight, 1.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // Techo: el plano del piso volteado (mirando hacia abajo) a la altura de la pared
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, floorTop + wallHeight, WALL_Z + FLOOR_SIZE * 0.5f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(FLOOR_SIZE, 1.0f, FLOOR_SIZE));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glBindTexture(GL_TEXTURE_2D, ceilTexture);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Pared con ventana (se dibuja al final porque tiene transparencia)
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, floorTop + wallHeight * 0.5f, WALL_Z));
        model = glm::scale(model, glm::vec3(wallWidth, wallHeight, 1.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, wallTexture);
        glBindVertexArray(wallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

        glDisable(GL_BLEND);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}


// Lee los vertices ("v x y z") de un archivo .obj y regresa su caja envolvente.
// Tambien imprime en consola el tamano del modelo en cada eje.
Bounds GetOBJBounds(const char* path)
{
    Bounds b;
    b.min = glm::vec3(1e9f);
    b.max = glm::vec3(-1e9f);

    std::ifstream file(path);
    if (!file.is_open())
    {
        std::cout << "No se pudo abrir " << path << std::endl;
        b.min = glm::vec3(0.0f);
        b.max = glm::vec3(0.0f);
        return b;
    }

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

    glm::vec3 size = b.max - b.min;
    std::cout << path << "\n   tamano X: " << size.x << "   Y: " << size.y << "   Z: " << size.z
        << "   (Y minima: " << b.min.y << ")" << std::endl;
    return b;
}

// Altura a la que se traslada un modelo para que su punto mas bajo quede sobre el piso
GLfloat GroundY(const Bounds& b, GLfloat scale, GLfloat floorTop)
{
    return floorTop - b.min.y * scale;
}

// Recorre los vertices de un .obj ya transformados al mundo y regresa el centro (en X y Z)
// de los que estan en la franja mas alta del modelo. topY regresa la altura maxima.
glm::vec3 GetTopCenter(const char* path, const glm::mat4& transform, GLfloat band, GLfloat& topY)
{
    std::vector<glm::vec3> pts;
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line))
    {
        if (line.size() > 2 && line[0] == 'v' && line[1] == ' ')
        {
            std::istringstream ss(line.substr(2));
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            pts.push_back(glm::vec3(transform * glm::vec4(v, 1.0f)));
        }
    }
    topY = -1e9f;
    for (const glm::vec3& p : pts) topY = std::max(topY, p.y);

    glm::vec3 sum(0.0f);
    int n = 0;
    for (const glm::vec3& p : pts)
        if (p.y > topY - band) { sum += p; n++; }
    return (n > 0) ? sum / (float)n : glm::vec3(0.0f, topY, 0.0f);
}

// Envia al shader los valores de una luz puntual. Apagada = componentes en cero.
void SetPointLight(Shader& shader, int index, glm::vec3 position, glm::vec3 color, bool on)
{
    std::string base = "pointLights[" + std::to_string(index) + "].";
    glm::vec3 ambient = glm::vec3(0.0f);   // sin ambiental: solo ilumina lo que tiene cerca
    glm::vec3 diffuse = on ? color : glm::vec3(0.0f);
    glm::vec3 specular = on ? color * 0.4f : glm::vec3(0.0f);

    glUniform3f(glGetUniformLocation(shader.Program, (base + "position").c_str()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "ambient").c_str()), ambient.x, ambient.y, ambient.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "specular").c_str()), specular.x, specular.y, specular.z);
    // Atenuacion de alcance corto (aproximadamente 7 unidades): la luz se concentra alrededor
    // de la lampara y se desvanece rapido, asi no pinta todo el cuarto de su color
    glUniform1f(glGetUniformLocation(shader.Program, (base + "constant").c_str()), 1.0f);
    glUniform1f(glGetUniformLocation(shader.Program, (base + "linear").c_str()), 0.7f);
    glUniform1f(glGetUniformLocation(shader.Program, (base + "quadratic").c_str()), 1.8f);
}

// Envia al shader los valores de un reflector (cono de luz con borde suave entre cutOff y outerCutOff)
void SetSpotLight(Shader& shader, int index, glm::vec3 position, glm::vec3 direction, glm::vec3 color,
    GLfloat cutOff, GLfloat outerCutOff, bool on)
{
    std::string base = "spotLights[" + std::to_string(index) + "].";
    glm::vec3 diffuse = on ? color : glm::vec3(0.0f);
    glm::vec3 specular = on ? color * 0.6f : glm::vec3(0.0f);

    glUniform3f(glGetUniformLocation(shader.Program, (base + "position").c_str()), position.x, position.y, position.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "direction").c_str()), direction.x, direction.y, direction.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "ambient").c_str()), 0.0f, 0.0f, 0.0f);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "diffuse").c_str()), diffuse.x, diffuse.y, diffuse.z);
    glUniform3f(glGetUniformLocation(shader.Program, (base + "specular").c_str()), specular.x, specular.y, specular.z);
    // Atenuacion de alcance medio (aproximadamente 50 unidades)
    glUniform1f(glGetUniformLocation(shader.Program, (base + "constant").c_str()), 1.0f);
    glUniform1f(glGetUniformLocation(shader.Program, (base + "linear").c_str()), 0.09f);
    glUniform1f(glGetUniformLocation(shader.Program, (base + "quadratic").c_str()), 0.032f);
    glUniform1f(glGetUniformLocation(shader.Program, (base + "cutOff").c_str()), glm::cos(glm::radians(cutOff)));
    glUniform1f(glGetUniformLocation(shader.Program, (base + "outerCutOff").c_str()), glm::cos(glm::radians(outerCutOff)));
}

// Crea el VAO de un plano de 6 vertices (posicion, normal, coordenadas de textura)
GLuint CreateQuadVAO(const float* data, size_t size)
{
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    // Posicion
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    return vao;
}

// Carga una imagen (PNG o JPG) como textura de OpenGL
GLuint LoadTexture(const char* path, int& width, int& height, GLint wrap)
{
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGBA);
    if (!image)
    {
        std::cout << "No se pudo cargar la imagen " << path << std::endl;
        width = 1;
        height = 1;
        return 0;
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
    return texture;
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

    // Encender / apagar cada luz con 1-5 (solo al presionar, no al mantener la tecla)
    // Funciona con los numeros de arriba del teclado y con los del teclado numerico
    int i = -1;
    if (key >= GLFW_KEY_1 && key <= GLFW_KEY_5) i = key - GLFW_KEY_1;
    if (key >= GLFW_KEY_KP_1 && key <= GLFW_KEY_KP_5) i = key - GLFW_KEY_KP_1;
    if (action == GLFW_PRESS && i >= 0)
    {
        lightOn[i] = !lightOn[i];
        std::cout << "Luz " << (i + 1) << " - " << LIGHT_NAMES[i] << ": " << (lightOn[i] ? "ENCENDIDA" : "APAGADA") << std::endl;
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