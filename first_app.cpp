#include <GLFW/glfw3.h>
#include "first_app.hpp"

void FirstApp::run() {
    while (!window.shouldClose())
    {
        glfwPollEvents();
    }
    
}