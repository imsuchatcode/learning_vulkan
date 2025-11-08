#include "my_model.hpp"

#include <cassert>
#include <cstring>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace my{
    MyModel::MyModel(Device &device, const MyModel::Builder &builder) : myDevice{device} {
        createVertexBuffers(builder.vertices);
        createIndexBuffers(builder.indicies);
    }

    MyModel::~MyModel(){
        vkDestroyBuffer(myDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(myDevice.device(), vertexBufferMemory, nullptr);

        if (haveIndexBuffer){
            vkDestroyBuffer(myDevice.device(), indexBuffer, nullptr);
            vkFreeMemory(myDevice.device(), indexBufferMemory, nullptr);
        }
    }

    void MyModel::createVertexBuffers(const std::vector<Vertex> &vertices){
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "vertex count must at least be 3");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        myDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(myDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(myDevice.device(), stagingBufferMemory);

        myDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        myDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(myDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(myDevice.device(), stagingBufferMemory, nullptr);
    }

    void MyModel::createIndexBuffers(const std::vector<uint32_t> &indicies){
        indexCount = static_cast<uint32_t>(indicies.size());
        haveIndexBuffer = indexCount > 0;
        
        if (!haveIndexBuffer){
            return;
        }

        VkDeviceSize bufferSize = sizeof(indicies[0]) * indexCount;

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;

        myDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(myDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indicies.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(myDevice.device(), stagingBufferMemory);

        myDevice.createBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        myDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(myDevice.device(), stagingBuffer, nullptr);
        vkFreeMemory(myDevice.device(), stagingBufferMemory, nullptr);
    }

    void MyModel::draw(VkCommandBuffer commandBuffer){
        if (haveIndexBuffer){
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else{
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void MyModel::bind(VkCommandBuffer commandBuffer){
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);

        if (haveIndexBuffer){
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }
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