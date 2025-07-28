#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> 

#include <string>

class Window
{
public:
    Window(int w, int h, std::string name);
    ~Window();

    bool shouldClose(){return glfwWindowShouldClose(window);}

private:
    void initwindow();

    const int width;
    const int height;
    std::string windowName;

    GLFWwindow *window;
};