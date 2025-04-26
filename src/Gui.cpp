#include "Gui.h"
#include <bits/algorithmfwd.h>
#include <vector>

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
    float windowWidth = 650.0f; // Largura desejada para sua janela

    // Configura a posição para o canto superior direito
    ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x - windowWidth, 20.0f), ImGuiCond_Always);

    // Configura o tamanho (opcional)
    ImGui::SetNextWindowSize(ImVec2(windowWidth, io.DisplaySize.y - 40.0f), ImGuiCond_Always);

    // Flags para melhor comportamento
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove | 
                                ImGuiWindowFlags_NoResize | 
                                ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("OpenGL/Close2GL Visualizer");

    // Texto
    ImGui::Text("Welcome to eletric bogaloo visualizer");

    
    ImGui::Text("Graphic API Options Avaliable: ");
    ImGui::RadioButton("OpenGL", &glSelected, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Close2GL", &glSelected, 1);
    ImGui::Separator();
    ImGui::Spacing();

    //Begin - Import Model option
    ImGui::Text("Model Import:");
    // Botão para abrir o file browser
    if (ImGui::Button("Abrir Navegador")) {
        showFileBrowser = true;
        currentPath = std::filesystem::current_path().string();
        selectedFile = "";
    }

    // File Browser integrado no menu
    if (showFileBrowser) {
        ImGui::BeginChild("FileBrowserWindow", ImVec2(500, 300), true, ImGuiWindowFlags_None);
        {
            // Barra de navegação
            if (currentPath != std::filesystem::path(currentPath).root_path()) {
                if (ImGui::Button("← Voltar")) {
                    currentPath = std::filesystem::path(currentPath).parent_path().string();
                }
                ImGui::SameLine();
            }
            ImGui::Text("Caminho: %s", currentPath.c_str());
            ImGui::Separator();

            // Lista de diretórios e arquivos
            try {
                // Pastas
                for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
                    if (entry.is_directory()) {
                        if (ImGui::Selectable(("> " + entry.path().filename().string()).c_str())) {
                            currentPath = entry.path().string();
                        }
                    }
                }

                // Arquivos
                for (const auto& entry : std::filesystem::directory_iterator(currentPath)) {
                    if (!entry.is_directory()) {
                        bool isSelected = (selectedFile == entry.path().filename().string());
                        if (ImGui::Selectable(entry.path().filename().string().c_str(), isSelected)) {
                            selectedFile = entry.path().filename().string();
                        }
                    }
                }
            } catch (...) {
                ImGui::TextColored(ImVec4(1,0,0,1), "Erro ao acessar diretório");
            }

            ImGui::Separator();
            if (ImGui::Button("Carregar") && !selectedFile.empty()) {
                std::string fullPath = (std::filesystem::path(currentPath) / selectedFile).string();
                completePath = fullPath;
                modelSelected = true;
                showFileBrowser = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancelar")) {
                showFileBrowser = false;
            }
        }
        ImGui::EndChild();
    }
   

    //End - Import Model option
    
    //Camera Settings
    ImGui::Text("Camera Settings: ");
    ImGui::Checkbox("Look At Camera", &selectLookAtCamera);
    ImGui::Spacing();

    ImGui::Text("Object Material Rendering Mode: ");
    ImGui::Checkbox("Solid", &selectSolid);
    ImGui::SameLine();
    ImGui::Checkbox("Wireframe", &selectWireframe);
    ImGui::SameLine();
    ImGui::Checkbox("Point", &selectPoint);
    ImGui::Spacing();

    ImGui::Text("Triangle Vertexes Orientation: ");
    ImGui::Checkbox("CCW", &selectCcw);
    ImGui::SameLine();
    ImGui::Checkbox("CW", &selectCw);
    ImGui::Spacing();
    
    // Base Model Checkbox options
    ImGui::Text("Translation Options: ");
    ImGui::Spacing();
    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f); // Largura dos campos
    ImGui::InputFloat("X", &translationPos[0], 0.1f, 1.0f, "%.2f");
    ImGui::SameLine();
    ImGui::InputFloat("Y", &translationPos[1], 0.1f, 1.0f, "%.2f");
    ImGui::SameLine();
    ImGui::InputFloat("Z", &translationPos[2], 0.1f, 1.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::Spacing();

    ImGui::InputFloat(
        "Scaling Value",  // Rótulo
        &scalingValue,    // Variável float
        0.01f,           // Step (incremento/decremento de 0.01)
        0.1f,            // Step rápido (Shift + clique)
        "%.2f"           // Formato (2 casas decimais)
    );
    ImGui::Spacing();

    ImGui::InputFloat(
        "Rotation Angle",  // Rótulo
        &rotationAngle,    // Variável de referência (float)
        0.1f,             // Step (incremento/decremento ao clicar nas setas)
        1.0f,             // Step rápido (quando segurar Shift)
        "%.1f",           // Formato (1 casa decimal)
        ImGuiInputTextFlags_None // Flags adicionais (opcional)
    );
    ImGui::Spacing();

    ImGui::Checkbox("Rotation X", &rotationXaxis);
    ImGui::SameLine();
    ImGui::Checkbox("Rotation Y", &rotationYaxis);
    ImGui::SameLine();
    ImGui::Checkbox("Rotation Z", &rotationZaxis);
    ImGui::Spacing();

    ImGui::InputFloat("Near Value", &nearValue);
    ImGui::InputFloat("Far Angle", &farValue);
    
    // Seletor de cor
    ImGui::ColorEdit3("Model Color", (float*)&m_ClearColor);
    
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    
    ImGui::End();
}

void Gui::shutdown()
{
    // Limpeza
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}