#include "gui.h"

Gui::Gui(GLFWwindow* window):
    m_Window(window),
    m_ShowDemoWindow(false),
    m_SliderValue(0.5f),
    m_ClearColor(ImVec4(0.45f, 0.55f, 0.60f, 1.00f))
{
}

Gui::~Gui()
{
    shutdown();
}

void Gui::init()
{
    // Configuração do ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // Estilo (opcional)
    ImGui::StyleColorsDark();
    // Backends do ImGui
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    // Variáveis de exemplo para o ImGui
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    float slider_value = 0.5f;
}

void Gui::beginFrame()
{
    // Inicia o frame do ImGui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Gui::endFrame()
{
    // Renderiza ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Gui::createMenu()
{
    ImGui::Begin("Minha Primeira Janela ImGui");

            // Texto
            ImGui::Text("Olá, mundo com ImGui!");
            
            // Checkbox
            ImGui::Checkbox("Mostrar Janela de Demo", &m_ShowDemoWindow);
            
            // Slider
            ImGui::SliderFloat("Valor do Slider", &m_SliderValue, 0.0f, 1.0f);
            
            // Botão
            if (ImGui::Button("Botão"))
                std::cout << "Botão pressionado! Valor do slider: " << m_SliderValue << std::endl;
            
            // Seletor de cor
            ImGui::ColorEdit3("Cor de fundo", (float*)&m_ClearColor);
            
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            
            ImGui::End();
}

void Gui::renderGL()
{
    
}

void Gui::shutdown()
{
    // Limpeza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}