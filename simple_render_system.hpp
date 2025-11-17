#pragma once

#include "pipeline.hpp"
#include "my_game_object.hpp"
#include "device.hpp"
#include "my_camera.hpp"
#include "my_frame_info.hpp"

#include <memory>
#include <vector>

namespace my{

class SimpleRenderSystem{
public:
        SimpleRenderSystem(Device &device, VkRenderPass renderPass);
        ~SimpleRenderSystem();

        SimpleRenderSystem(const SimpleRenderSystem &) = delete;
        SimpleRenderSystem& operator=(const SimpleRenderSystem &) = delete;

        void renderGameObjects(FrameInfo &frameInfo, std::vector<MyGameObject> &gameObjects);
    private:
        void createPipelineLayout();
        void createPipeline(VkRenderPass renderPass);

        Device &myDevice;

        std::unique_ptr<PipeLine> myPipeLine;
        VkPipelineLayout pipelineLayout;
};

}