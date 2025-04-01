#include "config.h"
#include "Gui.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
    

//Prototipos
void renderClearColorGui(const ImVec4 clear_color);
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
unsigned int compileShader(unsigned int type, const char* source);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void printBoundingBoxInfo(const Model& model);
void drawModel(Model& model, glm::vec3 position, float scale, float rotation_angle, glm::vec3 rotation_axis ,Shader& shader);

// Configurações
const unsigned int SCR_WIDTH = 1820;
const unsigned int SCR_HEIGHT = 1020;

// Câmera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Mouse treatment
bool mouseRightClick = false;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main(){
    
    GLFWwindow* window;
    
    if(!glfwInit()){
        std::cout << "GLFW couldn`t start" << std::endl;
        return -1;
    }
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Model Visualizer", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    // Na inicialização, após criar a câmera:
    camera.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f)); // 3 unidades atrás da origem
    camera.SetFront(glm::vec3(0.0f, 0.0f, -1.0f));  // Olhando para -Z
    camera.SetUp(glm::vec3(0.0f, 1.0f, 0.0f));      // Vetor up padrão

    // Configura callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Cria o programa de shader
    Shader ourShader("src/shader_vertex.glsl", "src/shader_fragment.glsl");
    Model cubeModel("models/cube.in");
    Model cowModel("models/cow_up.in");

    printBoundingBoxInfo(cubeModel);
    printBoundingBoxInfo(cowModel);

    // Normaliza as escalas
    float cubeScale = 1.0f / cubeModel.getBoundingRadius();
    float cowScale = 1.0f / cowModel.getBoundingRadius();

    // Posições relativas
    glm::vec3 cubePosition(0.0f, -1.0f, -2.0f);  // Cubo mais perto
    glm::vec3 cowPosition(0.0f, -1.0f, 2.0f);    // Vaca mais longe

    // Configura a câmera para centralizar no objeto
    glm::vec3 objectCenter = cubeModel.getCenter();
    float objectRadius = -cubeModel.getBoundingRadius();
    std::cout << "Cube Model Bounding Radius: " << objectRadius << std::endl;
    camera.centerOnObject(glm::vec3(0.0f), objectRadius);

    Gui gui(window);
    gui.init();
    
    glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
    // Configuração OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //glDisable(GL_CULL_FACE); // Desativa culling temporariamente

    while (!glfwWindowShouldClose(window))
    {
        // Calcula delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Processa entrada
        processInput(window);
        if(gui.getLookAtSelection())
        {
            processInput(window);
            // Configura a câmera para centralizar no objeto
            objectCenter = cubeModel.getCenter();
            objectRadius = -cubeModel.getBoundingRadius();
            camera.centerOnObject(objectCenter, objectRadius);
        }
        // Limpa buffers
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //GUI Render
        gui.beginFrame();
        gui.createMenu();
        //const ImVec4 gui_color = gui.getClearColor();
        //renderClearColorGui(gui_color);

        // Ativa o shader
        ourShader.use();
        
        // Configurações comuns a ambos os modelos
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH/(float)SCR_HEIGHT,
            gui.getNearValue(),
            gui.getFarValue()  // Far plane grande para ambos objetos
        );
    
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        //Testing function draw
        if(gui.getCubeSelection())
        {
            float* cubeTranslation = gui.getTranslationVector();
            glm::vec3 translationVector(cubeTranslation[0],cubeTranslation[1],cubeTranslation[2]);
            cubeScale = gui.getScalingValue();
            float rotationAngle = gui.getRotatingAngle();
            float* cubeRotation = gui.getRotationVector();
            glm::vec3 rotationVector(cubeRotation[0],cubeRotation[1],cubeRotation[2]);
            drawModel(cubeModel, translationVector, cubeScale, glfwGetTime() * 0.5f, rotationVector ,ourShader);
        }

        if(gui.getCowSelection())
        {
            float* cowTranslation = gui.getTranslationVector();
            glm::vec3 translationVector(cowTranslation[0],cowTranslation[1],cowTranslation[2]);
            cowScale = gui.getScalingValue();
            float rotationAngle = gui.getRotatingAngle();
            float* cowRotation = gui.getRotationVector();
            glm::vec3 rotationVector(cowRotation[0],cowRotation[1],cowRotation[2]);
            drawModel(cowModel, translationVector, cowScale, rotationAngle, rotationVector ,ourShader);
        }
        
        gui.endFrame();

        /* code */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0; 
}

void renderClearColorGui(const ImVec4 clear_color)
{
    // Renderização OpenGL
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, 
        clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

//Manage input processing
void processInput(GLFWwindow* window) {
    static float rotationSpeed = 20.0f; // graus por segundo
    static double lastPrintTime = 0.0;
    double currentTime = glfwGetTime();
    if (currentTime - lastPrintTime > 0.1) { // Limita a 10 prints por segundo
        std::cout << camera.getPositionInfo() << " | " << camera.getOrientationInfo() << std::endl;
        lastPrintTime = currentTime;
    }

    //Camera Rotation
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.rotateLocalX(rotationSpeed * deltaTime);
        camera.updateCameraRotationOnAxis();
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        camera.rotateLocalY(rotationSpeed * deltaTime);
        camera.updateCameraRotationOnAxis();
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        camera.rotateLocalZ(rotationSpeed * deltaTime);
        camera.updateCameraRotationOnAxis();
    }

    //Check if mouse was clicked
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        std::cout << "Menu mouse enabled" << std::endl;
        mouseRightClick = !mouseRightClick;
        // Alterna entre modos de cursor
        glfwSetInputMode(window, GLFW_CURSOR, 
            mouseRightClick ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    if(!mouseRightClick)
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(0, deltaTime); // Frente
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(1, deltaTime); // Trás
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(2, deltaTime); // Esquerda
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(3, deltaTime); // Direita
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.ProcessKeyboard(4, deltaTime); // Cima
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.ProcessKeyboard(5, deltaTime); // Baixo
        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            camera.centerOnObject(glm::vec3(0.0f), -1.73205f); // Reposition camera - Need to adjust this
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    // Verifica clique direito
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        mouseRightClick = !mouseRightClick;
        
        // Alterna entre modos de cursor
        glfwSetInputMode(window, GLFW_CURSOR, 
            mouseRightClick ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    //If mouse is active, stop camera activity
    if(mouseRightClick)
        return;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // invertido
    
    lastX = xpos;
    lastY = ypos;
    
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(yoffset);
}

void printBoundingBoxInfo(const Model& model) {
    std::cout << "=== Bounding Box Info ===" << std::endl;
    std::cout << "Min Bounds: (" << model.getMinBounds().x << ", " 
                                << model.getMinBounds().y << ", " 
                                << model.getMinBounds().z << ")" << std::endl;
    std::cout << "Max Bounds: (" << model.getMaxBounds().x << ", " 
                                << model.getMaxBounds().y << ", " 
                                << model.getMaxBounds().z << ")" << std::endl;
    std::cout << "Center: (" << model.getCenter().x << ", " 
                            << model.getCenter().y << ", " 
                            << model.getCenter().z << ")" << std::endl;
    std::cout << "Bounding Radius: " << model.getBoundingRadius() << std::endl;
}

void drawModel(Model& model, glm::vec3 position, float scale, float rotation_angle, glm::vec3 rotation_axis ,Shader& shader)
{
    // Renderiza os objetos 3D carregados
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));
    //modelMatrix = glm::rotate(modelMatrix, rotation_angle, rotation_axis);
    shader.setMat4("model", modelMatrix);
    model.Draw(shader);
}
