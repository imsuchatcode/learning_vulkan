#pragma once

#include "window.hpp"
#include "my_game_object.hpp"
#include "device.hpp"
#include "my_renderer.hpp"
#include "grid_system.hpp"

#include <memory>
#include <vector>

namespace my{

class FirstApp{
    public:
        static constexpr int WIDTH = 1200;
        static constexpr int HEIGHT = 800;

        FirstApp();
        ~FirstApp();

        FirstApp(const FirstApp &) = delete;
        FirstApp& operator=(const FirstApp &) = delete;

        void run();

    private:
        void loadGameObjects();

        Window window{WIDTH, HEIGHT, "cpp is hard"};
        Device device{window};
        MyRenderer myRenderer{window, device};
        // currently using mailbox not vsync(fifo) will change if run into error

        // use pointer to easily delete and recreate for window resize
        std::vector<MyGameObject> gameObjects;
        std::unique_ptr<GridSystem> gridSystem;
};

}