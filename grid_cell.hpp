#pragma once

#include <glm/glm.hpp>

namespace my
{
class GridCell{
private:
    bool alive = false;
    glm::vec3 color = glm::vec3(0.2f, 0.2f, 0.2f);

public:
    bool isAlive() { return alive; }

    void setAlive() { alive = true; }
    void setDead() {alive = false;}

    void updateConwayRule(int aliveNeighbor);

    glm::vec3 getColor() { return color; }

    void setColor();
};

}