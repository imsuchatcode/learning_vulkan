#pragma once

#include "gravity_physic_system.hpp"

namespace my{
class Vec2FieldSystem{
public:
    void update(const GravityPhysicSystem& physicSystem, std::vector<MyGameObject>& physicObjects, std::vector<MyGameObject>& vectorField);

    std::unique_ptr<MyModel> createSquareModel(Device& myDevice, glm::vec2 offset);

    std::unique_ptr<MyModel> createCircleModel(Device& myDevice, unsigned int numSides);
};

}