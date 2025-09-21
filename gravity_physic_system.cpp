#include "gravity_physic_system.hpp"

namespace my
{
    GravityPhysicSystem::GravityPhysicSystem(float strength) : strengthGravity{strength} {}

    void GravityPhysicSystem::update(std::vector<MyGameObject>& gameObjects, float dt, unsigned int substeps = 1){
        const float stepDelta = dt / substeps;
        for (int i = 0; i < substeps; i++){
            stepSimulation(gameObjects, stepDelta);
        }
    }

    glm::vec2 GravityPhysicSystem::computeForce(MyGameObject& fromObject, MyGameObject& toObject) const {
        auto offset = fromObject.transform2d.translation - toObject.transform2d.translation;
        float distanceSquared = glm::dot(offset, offset);

        if (glm::abs(distanceSquared) < 1e-10f){
            return {.0f, .0f};
        }

        float force = strengthGravity * toObject.rigidBody2d.mass * fromObject.rigidBody2d.mass / distanceSquared;
        return force * offset / glm::sqrt(distanceSquared);
    }

    void GravityPhysicSystem::stepSimulation(std::vector<MyGameObject>& gameObjects, float dt){
        for (auto iterA = gameObjects.begin(); iterA != gameObjects.end(); iterA++){
            auto& objA = *iterA;
            for(auto iterB = iterA; iterB != gameObjects.end(); iterB++){
                if (iterA == iterB){
                    continue;
                }
                auto& objB = *iterB;

                auto force = computeForce(objA, objB);
                objA.rigidBody2d.velocity = dt * -force / objA.rigidBody2d.mass;
                objB.rigidBody2d.velocity = dt * force / objB.rigidBody2d.mass;

            }
        }
        for (auto &obj : gameObjects){
            obj.transform2d.translation += dt * obj.rigidBody2d.velocity;
        }
    }
}

