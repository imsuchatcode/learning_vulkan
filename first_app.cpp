#include <GLFW/glfw3.h>
#include "first_app.hpp"
#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace my{

FirstApp::FirstApp(){
    loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
    SimpleRenderSystem simpleRenderSystem{device, myRenderer.getSwapChainRenderPass()};
    
    while (!window.shouldClose()){
        glfwPollEvents();

        if (auto commandBuffer = myRenderer.beginFrame()){
            myRenderer.beginSwapChainRenderPass(commandBuffer);
            
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);

            myRenderer.endSwapChainRenderPass(commandBuffer);
            myRenderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}


void FirstApp::loadGameObjects(){
    const int PIXEL_PER_CELL = 10;
    gridSystem = std::make_unique<GridSystem>(device, WIDTH, HEIGHT, PIXEL_PER_CELL);
    
    std::vector<std::vector<MyGameObject>>& grid = gridSystem->getCurGrid();
    
    for (int i = 0; i < gridSystem->getRows(); i++){
        for (int j = 0; j < gridSystem->getCols(); j++){
            gameObjects.push_back(std::move(grid[i][j]));
        }
    }

/*
    std::vector<MyModel::Vertex> vertices = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}}, 
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},  
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
    auto myModel = std::make_shared<MyModel>(device, vertices);

    auto triangle = MyGameObject::createGameObject();
    triangle.model = myModel;
    triangle.color = {.1f, .8f, .1f};
    triangle.transform2d.translation.x = .2f;
    triangle.transform2d.scale = {2.f, .5f};
    triangle.transform2d.rotation = .25f * glm::two_pi<float>();

    gameObjects.push_back(std::move(triangle));
*/
}

}