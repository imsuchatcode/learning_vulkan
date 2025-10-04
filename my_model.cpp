#include "my_model.hpp"

#include <cassert>
#include <cstring>

namespace my{
    MyModel::MyModel(Device &device, const std::vector<Vertex> &vertices) : myDevice{device} {
        createVertexBuffers(vertices);
    }
    MyModel::~MyModel(){
        vkDestroyBuffer(myDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(myDevice.device(), vertexBufferMemory, nullptr);
    }

    void MyModel::createVertexBuffers(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must at least be 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        myDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

        void *data;
        vkMapMemory(myDevice.device(), vertexBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(myDevice.device(), vertexBufferMemory);
    }

    void MyModel::draw(VkCommandBuffer commandBuffer){
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0); 

    }

    void MyModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    }

    std::vector<VkVertexInputBindingDescription> MyModel::Vertex::getBindingDescriptions(){
        std::vector<VkVertexInputBindingDescription> bindingDescription(1);
        bindingDescription[0].binding = 0;
        bindingDescription[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescription[0].stride = sizeof(Vertex);
        return bindingDescription;
    }
    std::vector<VkVertexInputAttributeDescription> MyModel::Vertex::getAttributeDescriptions(){
        std::vector<VkVertexInputAttributeDescription> attributeDescription(2);
        //binding is like a number of vector<Vertex>
        //location is the location use in the vertex shader
        //
        attributeDescription[0].binding = 0;
        // renember to change if change position to coreresponeding vector
        attributeDescription[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[0].location = 0;
        attributeDescription[0].offset = offsetof(Vertex, position);

        attributeDescription[1].binding = 0;
        attributeDescription[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescription[1].location = 1;
        attributeDescription[1].offset = offsetof(Vertex, color);
        return attributeDescription;
    }
}