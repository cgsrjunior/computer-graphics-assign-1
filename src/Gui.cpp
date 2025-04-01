#include "Gui.h"

Gui::Gui(GLFWwindow* window):
    m_Window(window),
    m_ShowDemoWindow(false),
    m_SliderValue(0.5f),
    m_ClearColor(ImVec4(0.0f, 0.0f, 0.0f, 1.00f))
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
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // Estilo (opcional)
    ImGui::StyleColorsDark();
    // Backends do ImGui
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
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
    // Obtém o tamanho da viewport
    ImGuiIO& io = ImGui::GetIO();
    float windowWidth = 400.0f; // Largura desejada para sua janela

    // Configura a posição para o canto superior direito
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - windowWidth, 20.0f), ImGuiCond_Always);

    // Configura o tamanho (opcional)
    ImGui::SetNextWindowSize(ImVec2(windowWidth, io.DisplaySize.y - 40.0f), ImGuiCond_Always);

    // Flags para melhor comportamento
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("OpenGL Visualizer Options");

            // Texto
            ImGui::Text("Welcome to eletric bogaloo visualizer");
            
            // Slider
            //ImGui::SliderFloat("Valor do Slider", &m_SliderValue, 0.0f, 1.0f);

            // Checkboxes
            ImGui::Checkbox("Select Cube", &selectCube);
            ImGui::Checkbox("Select Cow", &selectCow);
            ImGui::Checkbox("Look At Camera", &selectLookAtCamera);

            ImGui::InputFloat3("Translation XYZ", translationPos);
            ImGui::InputFloat("Scaling Value", &scalingValue);
            ImGui::InputFloat("Rotation Angle", &rotationAngle);
            ImGui::InputFloat3("Rotation XYZ", rotationPos);

            ImGui::InputFloat("Near Value", &nearValue);
            ImGui::InputFloat("Far Angle", &farValue);
            
            // Botão
            //if (ImGui::Button("Botão"))
            //    std::cout << "Botão pressionado! Valor do slider: " << m_SliderValue << std::endl;
            
            // Seletor de cor
            ImGui::ColorEdit3("Model Color", (float*)&m_ClearColor);
            
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