#include "config.h"
#include "Gui.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
    

//Prototipos
void renderClearColorGui(const ImVec4 clear_color);
unsigned int createShaderProgram(const char* vertexSource, const char* fragmentSource);
unsigned int compileShader(unsigned int type, const char* source);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void printBoundingBoxInfo(const Model& model);
void drawModel(Model& model, glm::vec3 position, float scale, float rotation_angle, glm::vec3 rotation_axis ,Shader& shader);
void debugColor(const int& rcolor, const int& gcolor, const int& bcolor);
void debugVector(const glm::vec3 vector);
void settingModelAndDraw(Model& model, Gui& gui ,Shader& shader);
void translateToFrontCamera(Model& model, Camera& camera ,Shader& shader);
void openGlProcessingMode(Model& currentModel, Gui& gui, Shader& ourShader, bool finishedObjectLoaded);
void close2GlProcessingMode();

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

//Shader variable controller - Inicializado em -1 pra fazer uma primeira selecao com base na predefinicao da interface
int lightModePreviouslySelected = -1;

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
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Cria o programa de shader
    Shader ourShader("phong", "src/phong_vertex.glsl", "src/phong_fragment.glsl");
    ourShader.loadShader("gouraud_ad", "src/gouraud_ad_vertex.glsl", "src/gouraud_ad_fragment.glsl");
    ourShader.loadShader("gouraud_ads", "src/gouraud_ads_vertex.glsl", "src/gouraud_ads_fragment.glsl");
    Model currentModel;
    float objectScale;
    glm::vec3 objectPosition;
    glm::vec3 objectCenter;
    float objectRadius;
    bool finishedObjectLoaded = false;

    Gui gui(window);
    gui.init();
    
    glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
    // Configuração OpenGL
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while (!glfwWindowShouldClose(window))
    {
        // Calcula delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // Processa entrada
        processInput(window);
        //First we need to check if the model is loaded,
        //setup the files and after that, enable a flag to allow us to work on
        if(gui.isModelSelected())
        {
            currentModel = Model();
            std::cout << "Path: " << gui.getModelPath() << std::endl;
            currentModel.loadModelFromFile(gui.getModelPath());
            objectScale = 1.0f / currentModel.getBoundingRadius();
            finishedObjectLoaded = true;
            gui.resetModelSelected();

            //Now we need to translate the model to appear in the front of camera
            translateToFrontCamera(currentModel, camera, ourShader);
        }

        // Limpa buffers
        glClearColor(255.0f, 255.0f, 255.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if(gui.getSolidSelection())
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        if(gui.getWireframeSelection())
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        if(gui.getPointSelection())
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

        if(gui.getCcwSelection())
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CCW);
        }
        if(gui.getCwSelection())
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            glFrontFace(GL_CW);
        }
        if(lightModePreviouslySelected != gui.getLightModeSelected())
        {
            /*Update the current selected shader*/
            lightModePreviouslySelected = gui.getLightModeSelected();
            switch (gui.getLightModeSelected())
            {
            case 0:
                /* Phong Shading*/
                ourShader.use("phong");
                break;
            case 1:
                /* Gourard AD shader */
                ourShader.use("gouraud_ad");
                break;
            case 2:
                /* Gourard ADS shader */
                ourShader.use("gouraud_ads");
                break;
            default:
                std::cout << "[ERROR] - GUI sent a lighting option not avaliable" << std::endl;
                break;
            }
        }
        

        //GUI Render
        gui.beginFrame();
        gui.createMenu();

        // Passa as uniforms para o shader
        float* lightingPos = gui.getLightPositionVector();
        glm::vec3 lightPos = glm::vec3(lightingPos[0],lightingPos[1],lightingPos[2]);
        glm::vec3 viewPos = camera.GetPosition(); // Posição da câmera
        glm::vec3 lightColor = glm::vec3(gui.getLightRcolor(), gui.getLightGcolor(), gui.getLightBcolor());
        glm::vec3 rgb = glm::vec3(gui.getRcolor(), gui.getGcolor(), gui.getBcolor());
        glUniform3fv(glGetUniformLocation(ourShader.ID, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniform3fv(glGetUniformLocation(ourShader.ID, "viewPos"), 1, glm::value_ptr(viewPos));
        glUniform3fv(glGetUniformLocation(ourShader.ID, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform3fv(glGetUniformLocation(ourShader.ID, "rgb"),1, glm::value_ptr(rgb));
        
        //Here we need to check if we have selected OpenGL or Close2GL
        switch (gui.getGlSelected())
        {
        case 0:
            /* code */
            openGlProcessingMode(currentModel, gui, ourShader, finishedObjectLoaded);
            break;
        case 1:
            break;
        default:
            std::cout << "[ERROR] Gui GL selection not working" << std::endl;
            break;
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
        //camera.updateCameraRotationOnAxis();
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
    if (glfwGetKey(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        std::cout << "Menu mouse enabled" << std::endl;
        mouseRightClick = true;
        // Alterna entre modos de cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if(mouseRightClick)
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
            camera.centerOnObject(glm::vec3(0.0f), -1.0f); // Reposition camera - Need to adjust this
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if( (button==GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_PRESS) )
    {
        // ... some code
        std::cout << "Menu mouse disabled" << std::endl;
        mouseRightClick = true;
        // Alterna entre modos de cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } 
    else if( (button==GLFW_MOUSE_BUTTON_RIGHT) && (action == GLFW_RELEASE))
    {
        // ... some code
        std::cout << "Menu mouse enabled" << std::endl;
        mouseRightClick = false;
        // Alterna entre modos de cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    //If mouse is active, stop camera activity
    if(!mouseRightClick)
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
    modelMatrix = glm::rotate(modelMatrix, rotation_angle, rotation_axis);
    shader.setMat4("model", modelMatrix);
    model.Draw(shader);
}

void debugColor(const int &rcolor, const int &gcolor, const int &bcolor)
{
    std::cout << "R Color: " << rcolor << std::endl;
    std::cout << "G Color: " << gcolor << std::endl;
    std::cout << "B Color: " << bcolor << std::endl;
}

void debugVector(const glm::vec3 vector)
{
    std::cout << "Vector: " << vector.x << 
                        " " << vector.y <<
                        " " << vector.z << std::endl;
}

void settingModelAndDraw(Model& model, Gui& gui, Shader& shader)
{
    float* modelTransalation = gui.getTranslationVector();
    glm::vec3 translationVector(modelTransalation[0],modelTransalation[1],modelTransalation[2]);
    float modelScale = gui.getScalingValue() / model.getBoundingRadius();;
    float rotationAngle = gui.getRotatingAngle();
    float rotationXaxis = gui.getRotationXAxis();
    float rotationYaxis = gui.getRotationYAxis();
    float rotationZaxis = gui.getRotationZAxis();
    glm::vec3 rotationVector(rotationXaxis, rotationYaxis, rotationZaxis);
    drawModel(model, translationVector, modelScale, rotationAngle, rotationVector, shader);
}

void translateToFrontCamera(Model& model, Camera& camera, Shader& shader)
{
    std::cout << "[INIT] - Translate to the front camera" << std::endl;
    glm::vec3 objectPos = camera.GetPosition() + camera.GetFront() * 10.0f; //Last parameter is distance from the camera
    glm::vec3 translation = objectPos - model.getCenter();
    glm::mat4 modelMatrix = glm::mat4(1.0f); // Matriz identidade
    modelMatrix = glm::translate(modelMatrix, translation);
}

void openGlProcessingMode(Model& currentModel, Gui& gui, Shader& ourShader, bool finishedObjectLoaded)
{
    // Configurações de projecao
    glm::mat4 projection = glm::perspective(
        glm::radians(45.0f),
        (float)SCR_WIDTH/(float)SCR_HEIGHT,
        gui.getNearValue(),
        gui.getFarValue()  // Far plane grande para ambos objetos
    );

    glm::mat4 view;
    //Detect if need to update the camera in a lookat format or in a free camera mode
    if(gui.getLookAtSelection() && finishedObjectLoaded)
    {
        std::cout << "Object Center: " << glm::to_string(currentModel.getCenter()) << std::endl;
        std::cout << "Bounding Radius: " << currentModel.getBoundingRadius() << std::endl;
        camera.centerOnObject(currentModel.getCenter(), -currentModel.getBoundingRadius());
        view = camera.GetViewMatrix(currentModel.getCenter());
    }
    else
        view = camera.GetViewMatrix();

    ourShader.setMat4("projection", projection);
    ourShader.setMat4("view", view);
    
    //Testing function draw
    if(finishedObjectLoaded)
    {
        settingModelAndDraw(currentModel, gui, ourShader);
    }
}
