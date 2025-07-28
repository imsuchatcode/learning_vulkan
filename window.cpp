#include "window.hpp"
#include <string>

Window::Window(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
    initwindow();
}

Window::~Window(){
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::initwindow(){
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

