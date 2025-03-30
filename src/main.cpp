#include "config.h"
#include "gui.h"

void renderClearColorGui(const ImVec4 clear_color)
{
    // Renderização OpenGL
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, 
        clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}


int main(){
    
    GLFWwindow* window;
    if(!glfwInit()){
        std::cout << "GLFW couldn`t start" << std::endl;
        return -1;
    }
    window = glfwCreateWindow(640, 480, "My window", NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        glfwTerminate();
        return -1;
    }

    Gui gui(window);
    gui.init();


    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {

        gui.beginFrame();

        gui.createMenu();

        const ImVec4 gui_color = gui.getClearColor();

        renderClearColorGui(gui_color);

        gui.endFrame();
        /* code */
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0; 
}