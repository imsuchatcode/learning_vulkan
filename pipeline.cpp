#include "pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

PipeLine::PipeLine(Device& device, const std::string &vertFilepath, const std::string &fragFilepath, const PipelineConfigInfo &configInfo) : Device{device} {
    createGraphicsPipeLine(vertFilepath, fragFilepath);
}

// a function that read frag file and vertex file
std::vector<char> PipeLine::readFile(const std::string& filepath){

    std::ifstream file(filepath, std::ios::ate | std::ios::binary);

    if(!file.is_open()){
        throw std::runtime_error("faid to open file: " + filepath);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

//output formated fragment and vertex file
void PipeLine::createGraphicsPipeLine(const std::string &vertFilepath, const std::string &fragFilepath){
    auto vertCode = readFile(vertFilepath);
    auto fragCode = readFile(fragFilepath);

    std::cout << "Vertex Shader Code Size: " << vertCode.size() << '\n';
    std::cout << "Fragment Shader Code Size: " << fragCode.size() << '\n';
}