#include "window.hpp"
#include <string>
#include <stdexcept>

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

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface){
    if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
        throw std::runtime_error("fail to create surface");
    }
}
