#pragma once

#include "device.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
#include <vector>

namespace my{

//copy vertex data from the cpu and transfer it to the gpu
class MyModel{
    public:
        struct Vertex{
            glm::vec3 position;
            glm::vec3 color;
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex &other) const {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Builder{
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indicies{};

            void loadModel(const std::string &filepath);
        };
        
        MyModel(Device &device, const MyModel::Builder &builder);
        ~MyModel();

        // must delete copy because it MyModel mangage buffer and memory object
        MyModel(const MyModel &) = delete;
        MyModel &operator=(const MyModel &) = delete;

        static std::unique_ptr<MyModel> createModelFromFile(Device &device, const std::string filepath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);
    private:

        void createVertexBuffers(const std::vector<Vertex> &vertices);
        void createIndexBuffers(const std::vector<uint32_t> &indicies);

        Device& myDevice;

        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        uint32_t vertexCount;

        bool haveIndexBuffer;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        uint32_t indexCount;
};

}