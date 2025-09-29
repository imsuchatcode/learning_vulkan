#include "grid_cell.hpp"
namespace my{
    //void GridCell::update(int updateNeighbor){}

    void GridCell::setColor(){
        if (alive){
            color = glm::vec3(0.f, 1.f, 0.f);
        }
        else{
            color = glm::vec3(0.2f, 0.2f, 0.2f);
        }
    }
}