#include <GLFW/glfw3.h>
#include "first_app.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "my_buffer.hpp"

#include <chrono>
#include <stdexcept>
#include <array>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace my{

struct GlobalUbo{
    glm::mat4 projectionView{1.f};
    glm::vec3 lightDirection = glm::normalize(glm::vec3(1.0, -3.0, -1.0));
};

FirstApp::FirstApp(){ loadGameObjects(); }

FirstApp::~FirstApp() {}

void FirstApp::run() {
    MyBuffer globalUboBuffer{
        device,
        sizeof(GlobalUbo), 
        SwapChain::MAX_FRAMES_IN_FLIGHT,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT,
        device.properties.limits.minUniformBufferOffsetAlignment
    };
    globalUboBuffer.map();

    SimpleRenderSystem simpleRenderSystem{device, myRenderer.getSwapChainRenderPass()};
    MyCamera camera{};

    // camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.7f, 0.f, 1.f));
    camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

    auto viewerObject = MyGameObject::createGameObject();
    KeyboardMovementController cameraController{};

    auto currentTime = std::chrono::high_resolution_clock::now();
           
    while (!window.shouldClose()){
        glfwPollEvents();

        auto newTime = std::chrono::high_resolution_clock::now();
        auto frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
        currentTime = newTime;

        // may add smallest time frame to prevent frame skipping

        cameraController.moveInPlaneXZ(window.getWindow(), frameTime, viewerObject);
        camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

        float aspect = myRenderer.getAspectRatio();
        //camera.setOrthographicProjection(-aspect, aspect, -1, 1, -1, 1);
        camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

        if (auto commandBuffer = myRenderer.beginFrame()){
            int frameIndex = myRenderer.getFrameIndex();
            // line below to update 
            GlobalUbo ubo{};
            ubo.projectionView = camera.getProjectionMatrix() * camera.getView();
            globalUboBuffer.writeToIndex(&ubo, frameIndex);
            globalUboBuffer.flushIndex(frameIndex);

            // line below to render 
            myRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);

            myRenderer.endSwapChainRenderPass(commandBuffer);
            myRenderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}


void FirstApp::loadGameObjects(){
    std::shared_ptr<MyModel> gameModel = MyModel::createModelFromFile(device, "models/smooth_vase.obj");    
    auto gameObj = MyGameObject::createGameObject();

    gameObj.model = gameModel;
    gameObj.transform.translation = {0.0f, 0.0f, 2.5f};
    gameObj.transform.scale = glm::vec3(3.f);
    gameObjects.push_back(std::move(gameObj));
}

}