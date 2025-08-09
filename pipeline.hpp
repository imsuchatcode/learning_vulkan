#pragma once

#include "device.hpp"
#include <string>
#include <vector>

struct PipelineConfigInfo {};

class PipeLine
{
public:
    PipeLine(Device& device,const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);
    ~PipeLine() {}

    PipeLine(const PipeLine&) = delete;
    void operator = (const PipeLine&) = delete;

    static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);
    
private:
    static std::vector<char> readFile (const std::string& filepath);

    void createGraphicsPipeLine(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    Device& device;
    VkPipeline graphicsPipeLine;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
};

