#pragma once

#include <vector>

#include "first_app.hpp"
#include "my_game_object.hpp"

namespace my
{
class GravityPhysicSystem{
public:
    GravityPhysicSystem(float strength);

    // later try to put this in the private part of a class to see what for
    const float strengthGravity;

    void update(std::vector<MyGameObject>& gameObjects, float dt, unsigned int substeps = 1);

    glm::vec2 computeForce(MyGameObject& fromObject, MyGameObject& toObject) const;

private:
    void stepSimulation(std::vector<MyGameObject>& gameObjects, float dt);

};

};