#pragma once

#include "my_model.hpp"

#include <memory>

namespace my
{
struct Transform2dComponent {
    glm::vec2 translation{};

    glm::mat2 mat2() { return glm::mat2{1.f}; } 
};

class MyGameObject{
    public:
    using id_t = unsigned int;

    static MyGameObject createGameObject() {
        static id_t currentId = 0;
        return MyGameObject{currentId++};
    }

    MyGameObject(const MyGameObject &) = delete;
    MyGameObject &operator = (const MyGameObject &) = delete;
    MyGameObject(MyGameObject &&) = default;
    MyGameObject &operator = (MyGameObject &&) = default;

    id_t const getId() { return id;};

    std::shared_ptr<MyModel> model{}; 
    glm::vec3 color{};   
    Transform2dComponent transform2d{};

    private:
    MyGameObject(id_t objId) : id{objId} {}

    id_t id;
};

}