#include "grid_cell.hpp"
namespace my{
    void GridCell::updateConwayRule(int aliveNeighbor){
        if (alive){
            if (aliveNeighbor == 3 || aliveNeighbor == 2){
                alive = true;
            }
            else{
                alive = false;
            }
        }
        else {
            if (aliveNeighbor == 3){
                alive = true;
            }
            else{
                alive = false;
            }
        }
    }

    void GridCell::setColor(){
        if (alive){
            color = glm::vec3(0.f, 1.f, 0.f);
        }
        else{
            color = glm::vec3(0.2f, 0.2f, 0.2f);
        }
    }
}