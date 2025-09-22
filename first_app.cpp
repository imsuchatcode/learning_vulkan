#include <GLFW/glfw3.h>
#include "first_app.hpp"
#include "simple_render_system.hpp"
#include "vec2_field_system.hpp"

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
    Vec2FieldSystem vecFieldSystem{};
    std::shared_ptr<MyModel> squareModel = vecFieldSystem.createSquareModel(device, {.5f, .0f}); 
    std::shared_ptr<MyModel> circleModel = vecFieldSystem.createCircleModel(device, 64);

    std::vector<MyGameObject> physicObjects{};
    auto red = MyGameObject::createGameObject();
    red.transform2d.scale = glm::vec2(.05f);
    red.transform2d.translation = {0.3f, 0.0f};     
    red.color = {1.f, 0.f, 0.f};
    red.rigidBody2d.velocity = {0.0f, 0.4f}; 
    red.model = circleModel; 
    physicObjects.push_back(std::move(red));

    auto blue = MyGameObject::createGameObject();
    blue.transform2d.scale = glm::vec2(.05f);
    blue.transform2d.translation = {-0.3f, 0.0f}; 
    blue.color = {0.f, 0.f, 1.f};
    blue.rigidBody2d.velocity = {0.0f, -0.4f};
    blue.model = circleModel;
    physicObjects.push_back(std::move(blue));

    std::vector<MyGameObject> vectorField{};
    int gridPoint = 40;
    for (int i = 0; i < gridPoint; i++){
        for (int j = 0; j < gridPoint; j++){
            auto vfp = MyGameObject::createGameObject();
            vfp.transform2d.scale = glm::vec2(.0025f);
            vfp.transform2d.translation = {-1.0f + (i + 0.5f) * 2.0f / gridPoint, -1.0f + (j + 0.5f) * 2.0f / gridPoint};
            vfp.color = glm::vec3(1.f);
            vfp.model = squareModel;
            vectorField.push_back(std::move(vfp));
        };
    }

    GravityPhysicSystem gravityPhysicSystem{0.81f};
    SimpleRenderSystem simpleRenderSystem{device, myRenderer.getSwapChainRenderPass()};
    
    while (!window.shouldClose()){
        glfwPollEvents();

        if (auto commandBuffer = myRenderer.beginFrame()){
            gravityPhysicSystem.update(physicObjects, 1.f / 4000 , 200);
            vecFieldSystem.update(gravityPhysicSystem, physicObjects, vectorField);

            myRenderer.beginSwapChainRenderPass(commandBuffer);
            simpleRenderSystem.renderGameObjects(commandBuffer, physicObjects);
            simpleRenderSystem.renderGameObjects(commandBuffer, vectorField);

            myRenderer.endSwapChainRenderPass(commandBuffer);
            myRenderer.endFrame();
        }
    }
    vkDeviceWaitIdle(device.device());
}

void FirstApp::loadGameObjects(){
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
}

}
// num of question that need to ask ai
// where does the color and attribute of my_game_object go to renderer
// how can the quareModel and circleModel can be render (render first then spread out ?)
// why use shader instead of manualy use vertex like createsquareModel and createCircleModel ?
// if the two circle dont spin why is that ? because it use the same thing as the triagle