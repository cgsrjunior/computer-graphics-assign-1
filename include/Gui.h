#pragma once
#include <imgui/imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>

class Gui
{
public:
    Gui(GLFWwindow* window);
    ~Gui();
    void init();
    void shutdown();
    void beginFrame();
    void endFrame();
    void createMenu();
    void renderGL();
    inline ImVec4 getClearColor() { return m_ClearColor; };

    //Getters
    inline float* getTranslationVector() {  return translationPos;  };
    inline float* getTranslationVectorCube() {  return translationPosCube;  };
    inline float  getScalingValue() {  return scalingValue;  };
    inline float  getRotatingAngle() {  return rotationAngle;  };
    inline float getRotationXAxis() {  return rotationXaxis;  };
    inline float getRotationYAxis() {  return rotationYaxis;  };
    inline float getRotationZAxis() {  return rotationZaxis;  };

    inline bool getCubeSelection() { return selectCube; };
    inline bool getCowSelection() { return selectCow; };
    inline bool getLookAtSelection() { return selectLookAtCamera; };
    inline bool getWireframeSelection() { return selectWireframe; };
    inline bool getPointSelection() { return selectPoint; };
    inline bool getSolidSelection() { return selectSolid; };
    inline bool getCwSelection() { return selectCw; };
    inline bool getCcwSelection() { return selectCcw; };
    inline float getNearValue() { return nearValue; };
    inline float getFarValue() { return farValue; };
    inline float getRcolor() { return m_ClearColor.x; }; 
    inline float getGcolor() { return m_ClearColor.y; };
    inline float getBcolor() { return m_ClearColor.z; };

private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;
    float m_SliderValue;

    //Need two bools, to select the cow or the cube and send to them the transformations
    bool selectCube = true;
    bool selectCow = false;
    bool selectLookAtCamera = false;
    bool selectSolid = true;
    bool selectWireframe = false;
    bool selectPoint = false;
    bool selectCcw = true;
    bool selectCw = false;

    //void drawModel(Model& model, glm::vec3 position, float scale, float rotation_angle, glm::vec3 rotation_axis ,Shader& shader)
    // Posições relativas
    //glm::vec3 cubePosition(0.0f, -1.0f, -2.0f);  // Cubo mais perto
    //glm::vec3 cowPosition(0.0f, -1.0f, 2.0f);    // Vaca mais longe
    float translationPos[3] = {0.0f,-1.0f, 2.0f};
    float translationPosCube[3] = {0.0f,-1.0f, -2.0f};
    float scalingValue = 1;
    float rotationAngle = 0;
    bool rotationXaxis = true;
    bool rotationYaxis = false;
    bool rotationZaxis = false;

    float nearValue=1;
    float farValue=1000;


    ImVec4 m_ClearColor;
};