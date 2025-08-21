#pragma once

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <vector>

namespace my{
    //copy vertex data from the cpu and transfer it to the gpu
    class myModel{
        public:
            struct Vertex{
                glm::vec2 position;

                static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
                static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
            };
            
            myModel(Device &device, const std::vector<Vertex> &vertices);
            ~myModel();

            myModel(const myModel &) = delete;
            myModel &operator=(const myModel &) = delete;

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