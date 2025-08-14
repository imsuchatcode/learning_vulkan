#pragma once

#include "window.hpp"
#include "pipeline.hpp"
#include "device.hpp"
#include "swap_chain.hpp"

class FirstApp{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        Window window{WIDTH, HEIGHT, "cpp is hard"};
        Device device{window};
        SwapChain swapChain{device, window.getExtend()};
        PipeLine pipeLine{device,"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", PipeLine::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
};
