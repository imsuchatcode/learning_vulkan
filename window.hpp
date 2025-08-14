#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h> 
#include <string>
#include <vulkan/vulkan.h>

class Window
{
public:
    Window(int w, int h, std::string name);
    ~Window();

    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;
    
    bool shouldClose(){return glfwWindowShouldClose(window);}
    VkExtent2D getExtend() {return {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};}

    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    void initwindow();

    const int width;
    const int height;
    std::string windowName;

    GLFWwindow *window;
};