#include <GLFW/glfw3.h>
#include "first_app.hpp"

#include <stdexcept>

FirstApp::FirstApp(){
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}

FirstApp::~FirstApp(){
    vkDestroyPipelineLayout(device.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
    while (!window.shouldClose())
    {
        glfwPollEvents();
    }

}
void FirstApp::createPipelineLayout(){
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;
    
    if (vkCreatePipelineLayout(device.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS){
        throw std::runtime_error("can not create pipelinelayout");
    }
}

void FirstApp::createPipeline(){
    auto pipelineConfig = PipeLine::defaultPipelineConfigInfo(swapChain.width(),swapChain.height());
    pipelineConfig.renderPass = swapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    // using the same name can lead to confusion
    myPipeLine = std::make_unique<PipeLine>(device, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
}

void FirstApp::createCommandBuffers(){
    commandBuffer.resize(swapChain.imageCount());
    VkCommandBufferAllocateInfo allocInfo{};

}

void FirstApp::drawFrame(){}