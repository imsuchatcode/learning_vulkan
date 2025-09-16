#include <GLFW/glfw3.h>
#include "first_app.hpp"

#include <stdexcept>
#include <array>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace my{

struct SimplePushConstantData{
    glm::mat2 transform{1.f};
    glm::vec2 offset;
    alignas(16) glm::vec3 color;
};

FirstApp::FirstApp(){
    loadGameObjects();
    createPipelineLayout();
    createPipeline();
}

FirstApp::~FirstApp(){
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
    while (!window.shouldClose()){
        glfwPollEvents();

        if (auto commandBuffer = myRenderer.beginFrame()){
            myRenderer.beginSwapChainRenderPass(commandBuffer);
            renderGameObjects(commandBuffer);

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

void FirstApp::createPipelineLayout(){
    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(SimplePushConstantData);
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 1;
    pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
    
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("can not create pipelinelayout");
    }
}

void FirstApp::createPipeline(){
    assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    PipeLine::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = myRenderer.getSwapChainRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    myPipeLine = std::make_unique<PipeLine>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::renderGameObjects(VkCommandBuffer commandBuffer){
    myPipeLine->bind(commandBuffer);

    for (auto &obj : gameObjects){
        obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0001f, glm::two_pi<float>());

        SimplePushConstantData push{};
        push.offset = obj.transform2d.translation;
        push.color = obj.color;
        push.transform = obj.transform2d.mat2();
        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}

}