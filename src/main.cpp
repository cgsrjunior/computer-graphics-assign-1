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

// Configurações
const unsigned int SCR_WIDTH = 1820;
const unsigned int SCR_HEIGHT = 1020;

// Câmera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

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
    glm::vec3 cubePosition(0.0f, 0.0f, -2.0f);  // Cubo mais perto
    glm::vec3 cowPosition(0.0f, 0.0f, 2.0f);    // Vaca mais longe

    // Configura a câmera para centralizar no objeto
    glm::vec3 objectCenter = cubeModel.getCenter();
    float objectRadius = -cubeModel.getBoundingRadius();
    camera.centerOnObject(glm::vec3(0.0f), objectRadius);

    Gui gui(window);
    gui.init();
    
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    // Configuração OpenGL
    glEnable(GL_DEPTH_TEST);
    //glDisable(GL_CULL_FACE); // Desativa culling temporariamente

    while (!glfwWindowShouldClose(window))
    {
        // Calcula delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Processa entrada
        processInput(window);
        // Limpa buffers
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //GUI Render
        gui.beginFrame();
        gui.createMenu();
        const ImVec4 gui_color = gui.getClearColor();
        renderClearColorGui(gui_color);

        // Ativa o shader
        ourShader.use();
        
        // Configurações comuns a ambos os modelos
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f),
            (float)SCR_WIDTH/(float)SCR_HEIGHT,
            0.1f,
            1000.0f  // Far plane grande para ambos objetos
        );
    
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        // 1. Renderiza o cubo primeiro
        glm::mat4 cubeModelMatrix = glm::mat4(1.0f);
        cubeModelMatrix = glm::translate(cubeModelMatrix, cubePosition);
        cubeModelMatrix = glm::scale(cubeModelMatrix, glm::vec3(cubeScale));
        cubeModelMatrix = glm::rotate(cubeModelMatrix, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        
        ourShader.setMat4("model", cubeModelMatrix);
        cubeModel.Draw(ourShader);
        
        // 2. Renderiza a vaca depois
        glm::mat4 cowModelMatrix = glm::mat4(1.0f);
        cowModelMatrix = glm::translate(cowModelMatrix, cowPosition);
        cowModelMatrix = glm::scale(cowModelMatrix, glm::vec3(cowScale));
        cowModelMatrix = glm::rotate(cowModelMatrix, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));
        
        ourShader.setMat4("model", cowModelMatrix);
        cowModel.Draw(ourShader);

        
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
    static double lastPrintTime = 0.0;
    double currentTime = glfwGetTime();
    if (currentTime - lastPrintTime > 0.1) { // Limita a 10 prints por segundo
        std::cout << camera.getPositionInfo() << " | " << camera.getOrientationInfo() << std::endl;
        lastPrintTime = currentTime;
    }

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
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
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