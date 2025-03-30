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

private:
    GLFWwindow* m_Window;
    bool m_ShowDemoWindow;
    float m_SliderValue;
    ImVec4 m_ClearColor;
};