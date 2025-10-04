#pragma once

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace my{
    //copy vertex data from the cpu and transfer it to the gpu
    class MyModel{
        public:
            struct Vertex{
                glm::vec3 position;
                glm::vec3 color;

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            };
            
            MyModel(Device &device, const std::vector<Vertex> &vertices);
            ~MyModel();

            // must delete copy because it MyModel mangage buffer and memory object
            MyModel(const MyModel &) = delete;
            MyModel &operator=(const MyModel &) = delete;

            void bind(VkCommandBuffer commandBuffer);
            void draw(VkCommandBuffer commandBuffer);
        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);

            Device& myDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
    };
}