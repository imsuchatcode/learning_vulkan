#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace my{

class MyCamera{
public:
    void setOrthographicProjection(float left, float right, float near, float far, float top, float bottom);

    void setPerspectiveProjection(float fovy, float aspect, float near, float far);

    const glm::mat4 & getProjectionMatrix() const {return projectionMatrix;}
private:
    glm::mat4 projectionMatrix{1.f};
};

}