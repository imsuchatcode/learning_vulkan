#include <GLFW/glfw3.h>
#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>
#include <memory>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace my{

struct SimplePushConstantData{
    glm::mat4 transform{1.f};
    glm::mat4 modelMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(Device &device, VkRenderPass renderPass) : myDevice{device}{
    createPipelineLayout();
    createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem(){
    vkDestroyPipelineLayout(myDevice.device(), pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(){
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
    
    if (vkCreatePipelineLayout(myDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("can not create pipelinelayout");
    }
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass){
    assert(pipelineLayout != nullptr && "cannot create pipeline before pipeline layout");

    PipelineConfigInfo pipelineConfig{};
    PipeLine::defaultPipelineConfigInfo(pipelineConfig);
    pipelineConfig.renderPass = renderPass;
    pipelineConfig.pipelineLayout = pipelineLayout;
    myPipeLine = std::make_unique<PipeLine>(myDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}

void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<MyGameObject> &gameObjects, const MyCamera& camera){
    myPipeLine->bind(commandBuffer);

    auto projectionView = camera.getProjectionMatrix() * camera.getView();

    for (auto &obj : gameObjects){
        //obj.transform.rotation.y = glm::mod(obj.transform.rotation.y + 0.0001f, glm::two_pi<float>());
        //obj.transform.rotation.x = glm::mod(obj.transform.rotation.x + 0.00005f, glm::two_pi<float>());
        SimplePushConstantData push{};
        auto modelMatrix = obj.transform.mat4();
        push.transform = projectionView * modelMatrix;
        push.modelMatrix = modelMatrix;

        vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

        obj.model->bind(commandBuffer);
        obj.model->draw(commandBuffer);
    }
}
}