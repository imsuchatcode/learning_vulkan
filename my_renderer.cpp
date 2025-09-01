//#include <GLFW/glfw3.h>
#include "my_renderer.hpp"

#include <stdexcept>
#include <array>
#include <memory>

namespace my{

struct SimplePushConstantData;

MyRenderer::MyRenderer(Window &window, Device &device) : myWindow{window}, myDevice{device} {
    recreateSwapChain();
    createCommandBuffers();
}

MyRenderer::~MyRenderer(){ freeCommandBuffers(); }

void MyRenderer::recreateSwapChain(){
    auto extend = myWindow.getExtend();
    while (extend.width == 0 || extend.height == 0){
        extend = myWindow.getExtend();
        glfwWaitEvents();
    } 

    vkDeviceWaitIdle(myDevice.device());
    if (mySwapChain == nullptr){
        mySwapChain = std::make_unique<SwapChain>(myDevice, extend);
    }
    else {
        mySwapChain = std::make_unique<SwapChain>(myDevice, extend, std::move(mySwapChain));
        if (mySwapChain->imageCount() != commandBuffers.size()){
            freeCommandBuffers();
            recreateSwapChain();
        }
    }
    // create pipeline here 
}

void MyRenderer::createCommandBuffers(){
    commandBuffers.resize(mySwapChain->imageCount());
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = myDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());
    
    if (vkAllocateCommandBuffers(myDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS){
        throw std::runtime_error("unable to create(allocate) command buffer");
    }
}

void MyRenderer::freeCommandBuffers(){
    vkFreeCommandBuffers(myDevice.device(), myDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
    commandBuffers.clear();
}

void MyRenderer::recordCommandBuffer(int imageIndex){
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("faied to begin recording command buffer");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = mySwapChain->getRenderPass();
    renderPassInfo.framebuffer = mySwapChain->getFrameBuffer(imageIndex);

    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = mySwapChain->getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(mySwapChain->getSwapChainExtent().width);
    viewport.height = static_cast<float>(mySwapChain->getSwapChainExtent().height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor{{0, 0}, mySwapChain->getSwapChainExtent()};
    vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
    vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

    renderGameObjects(commandBuffers[imageIndex]);

    vkCmdEndRenderPass(commandBuffers[imageIndex]);

    if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS)
    {
        throw std::runtime_error("fail to end command buffer");
    }
}

void MyRenderer::drawFrame(){
    uint32_t imageIndex;
    auto result = mySwapChain->acquireNextImage(&imageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to aquire swap chain image");
    }

    recordCommandBuffer(imageIndex);
    result = mySwapChain->submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.wasWindowResized() == true){
        window.resetWindowResizedFlag();
        recreateSwapChain();
        return;
    }
    if (result != VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image");
    }
}

VkCommandBuffer MyRenderer::beginFrame(){
    assert(!isFrameStarted && "cant call beginFrame while already in progress");
    auto result = mySwapChain->acquireNextImage(&currentImageIndex);
    
    if (result == VK_ERROR_OUT_OF_DATE_KHR){
        recreateSwapChain();
        return nullptr;
    }

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR){
        throw std::runtime_error("failed to aquire swap chain image");
    }

    isFrameStarted = true;

    auto commandBuffer = getCurrentCommandBuffer();
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
    {
        throw std::runtime_error("faied to begin recording command buffer");
    }

    return commandBuffer;
}

void MyRenderer::endFrame(){
    assert(isFrameStarted && "can't not end frame when frame is not in progress");
    auto commandBuffer = getCurrentCommandBuffer();

    if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
    {
        throw std::runtime_error("fail to end command buffer");
    }

    auto result = mySwapChain->submitCommandBuffers(&commandBuffer, &currentImageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || myWindow.wasWindowResized() == true){
        myWindow.resetWindowResizedFlag();
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS){
        throw std::runtime_error("failed to present swap chain image");
    }

    isFrameStarted = false;
}
void MyRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer){}
void MyRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer){}
}