#pragma once

#include "window.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

#include <memory>
#include <vector>
#include <cassert>

namespace my{

class MyRenderer{
    public:
        MyRenderer(Window& window, Device& device);
        ~MyRenderer();

        MyRenderer(const MyRenderer &) = delete;
        MyRenderer& operator=(const MyRenderer &) = delete;

        VkRenderPass getSwapChainRenderPass() const {return mySwapChain->getRenderPass();}
        bool isFrameInProgress() const {return isFrameStarted;}
        VkCommandBuffer getCurrentCommandBuffer() const {
            assert(isFrameStarted && "cannot get framebuffer when frame not in progress");
            return commandBuffers[currentFrameIndex];
        }

        int getFrameIndex() const {
            assert(isFrameStarted && "cannot get frame index when frame not in progress");
            return currentFrameIndex; 
        }

        VkCommandBuffer beginFrame();
        void endFrame();
        void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
        void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

    private:
        void createCommandBuffers();
        void freeCommandBuffers();
        void recreateSwapChain();

        Window& myWindow;
        Device& myDevice;
        // currently using mailbox not vsync(fifo) will change if run into error
        // use pointer to easily delete and recreate for window resize

        std::unique_ptr<SwapChain> mySwapChain;
        std::vector<VkCommandBuffer> commandBuffers;

        uint32_t currentImageIndex;
        int currentFrameIndex;
        bool isFrameStarted = false;
};


}