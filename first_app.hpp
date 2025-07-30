#pragma once

#include "window.hpp"
#include "pipeline.hpp"

class FirstApp{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        void run();
    private:
        Window window{WIDTH, HEIGHT, "cpp is hard"};
        PipeLine pipeLine{"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
};
