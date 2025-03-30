cmake_minimum_required(VERSION 3.5.0)
project(hello_window VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

set(MODELS_DIR "${CMAKE_SOURCE_DIR}/models")

# Copia os shaders para o diretório de build
file(COPY src/shader_vertex.glsl src/shader_fragment.glsl DESTINATION ${CMAKE_BINARY_DIR})

add_executable(hello_window include/config.h
                        include/Vertex.h
                        include/Material.h
                        include/Mesh.h
                        include/Model.h
                        include/Gui.h
                        include/Shader.h
                        include/Camera.h
                        src/main.cpp
                        src/glad.c
                        src/Gui.cpp
                        src/Mesh.cpp
                        src/Model.cpp
                        src/Shader.cpp
                        src/Camera.cpp
                        # Arquivos do ImGui (ajuste os caminhos conforme sua estrutura)
                        dependencies/imgui/imgui.cpp
                        dependencies/imgui/imgui_demo.cpp
                        dependencies/imgui/imgui_draw.cpp
                        dependencies/imgui/imgui_tables.cpp
                        dependencies/imgui/imgui_widgets.cpp
                        dependencies/imgui/backends/imgui_impl_glfw.cpp
                        dependencies/imgui/backends/imgui_impl_opengl3.cpp
                        )

target_include_directories(hello_window PRIVATE 
                                        dependencies 
                                        include
                                        dependencies/imgui
                                        dependencies/imgui/backends
                                        )

target_link_libraries(hello_window  glfw
                                    OpenGL::GL
                                    glm::glm
                                    )