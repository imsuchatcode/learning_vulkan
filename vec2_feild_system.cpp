#include "vec2_field_system.hpp"

namespace my{
    void Vec2FieldSystem::update(const GravityPhysicSystem &physicSystem, std::vector<MyGameObject> &physicObjects, std::vector<MyGameObject> &vectorField){
        for (auto& vf : vectorField){
            glm::vec2 direction{};
            for (auto& obj : physicObjects){
                direction += physicSystem.computeForce(obj, vf);
            }
            // to strech the point to a line
            vf.transform2d.scale.x = 0.005f + 0.045f * glm::clamp(glm::log(glm::length(direction) + 1) / 3.f, 0.f, 0.4f);
            vf.transform2d.rotation = atan2(direction.y, direction.x);
        }
    }

    std::unique_ptr<MyModel> Vec2FieldSystem::createSquareModel(Device& myDevice, glm::vec2 offset){
        std::vector<MyModel::Vertex> vertices = { {{-0.5f, -0.5f}}, {{0.5f, 0.5f}}, {{-0.5f, 0.5f}},
                                                  {{-0.5f, -0.5f}}, {{0.5f, -0.5f}}, {{0.5f, 0.5f}} };
        for (auto& v : vertices){
            v.position += offset;
        }
        return std::make_unique<MyModel>(myDevice, vertices);
    }

    std::unique_ptr<MyModel> Vec2FieldSystem::createCircleModel(Device& myDevice, unsigned int numSides){
        std::vector<MyModel::Vertex> uniqueVertices{};
        for (int i = 0; i < numSides; i++){
            float angle = i * glm::two_pi<float>() / numSides;
            uniqueVertices.push_back({{glm::cos(angle), glm::sin(angle)}});
        }
        uniqueVertices.push_back({});

        std::vector<MyModel::Vertex> vertices{};
        for (int i = 0; i < numSides; i++){
            vertices.push_back(uniqueVertices[i]);
            vertices.push_back(uniqueVertices[(i + 1) % numSides]);
            vertices.push_back(uniqueVertices[numSides]);
        }
        return std::make_unique<MyModel>(myDevice, vertices);
    }
}