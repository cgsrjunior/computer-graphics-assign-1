#pragma once
#include <imgui/imgui.h>
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <iostream>
#include <filesystem>


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
    inline ImVec4 getClearColor() { return m_ClearColor; };

    //Getters
    inline float* getTranslationVector() {  return translationPos;  };
    inline float* getLightPositionVector() {  return lightPos;  };
    inline float  getScalingValue() {  return scalingValue;  };
    inline float  getRotatingAngle() {  return rotationAngle;  };
    inline float  getRotationXAxis() {  if (rotationXaxis) return 1; else return 0;  };
    inline float  getRotationYAxis() {  if (rotationYaxis) return 1; else return 0;  };
    inline float  getRotationZAxis() {  if (rotationZaxis) return 1; else return 0;  };

    inline bool getCubeSelection() { return selectCube; };
    inline bool getCowSelection() { return selectCow; };
    inline bool getCubeTranslation() { return translateCube; };
    inline bool getCowTranslation() { return translateCow; };

    inline bool getLookAtSelection() { return selectLookAtCamera; };
    inline bool getWireframeSelection() { return selectWireframe; };
    inline bool getPointSelection() { return selectPoint; };
    inline bool getSolidSelection() { return selectSolid; };
    inline bool getCwSelection() { return selectCw; };
    inline bool getCcwSelection() { return selectCcw; };
    inline float getNearValue() { return nearValue; };
    inline float getFarValue() { return farValue; };

    //RGB color of the model
    inline float getRcolor() { return m_ClearColor.x; }; 
    inline float getGcolor() { return m_ClearColor.y; };
    inline float getBcolor() { return m_ClearColor.z; };

    //Light color of the model
    inline float getLightRcolor() { return m_LightColor.x; }; 
    inline float getLightGcolor() { return m_LightColor.y; };
    inline float getLightBcolor() { return m_LightColor.z; };

    inline int getGlSelected() { return glSelected; };
    inline int getLightModeSelected() { return lightModeSelected; };
    inline std::string getModelPath() { return completePath; };
    inline bool isModelSelected() { return modelSelected; };
    inline void resetModelSelected() { modelSelected = false; };

    //Public variables
    std::string currentPath = std::filesystem::current_path().string();
    bool showFileDialog = false;          // Controle para mostrar/ocultar o diálogo
    bool modelSelected = false;             // Flag para indicar se um modelo foi carregado
    std::string selectedFile = "";    // Nome do modelo atual

private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;
    float m_SliderValue;

    std::string completePath = std::filesystem::current_path().string();

    //Need two bools, to select the cow or the cube and send to them the transformations
    bool selectCube = true;
    bool selectCow = false;
    bool translateCube = true;
    bool translateCow = false;
    bool selectLookAtCamera = false;
    bool selectSolid = true;
    bool selectWireframe = false;
    bool selectPoint = false;
    bool selectCcw = true;
    bool selectCw = false;
    float translationPos[3] = {0.0f, 0.0f, 0.0f};
    float lightPos[3] = {2.0f, 2.0f, 2.0f};
    float scalingValue = 1;
    float rotationAngle = 0;
    bool rotationXaxis = true;
    bool rotationYaxis = false;
    bool rotationZaxis = false;
    bool showFileBrowser = false;
    float nearValue=1;
    float farValue=1000;
    int glSelected = 0;
    int lightModeSelected = 0;
    ImVec4 m_ClearColor;
    ImVec4 m_LightColor = {0.8f, 0.8f, 0.8f, 1.0f};
};