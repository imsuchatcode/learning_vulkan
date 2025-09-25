#pragma once

#include <memory>
#include "my_model.hpp"
#include "my_game_object.hpp"
#include "device.hpp"

namespace my
{
class GridSystem{
public:
    GridSystem(Device& myDevice, int width, int height, int pixelPerCell);
    ~GridSystem() {};

    std::unique_ptr<MyModel> createSquareModel(Device &myDevice);

    // Return reference instead of copy to avoid copying non-copyable MyGameObject
    std::vector<std::vector<MyGameObject>>& getCurGrid() {return curGrid;}

    int getRows() {return rows;}
    int getCols() {return cols;}

private:
    int rows;
    int cols; 

    std::vector<std::vector<MyGameObject>> curGrid;
    std::vector<std::vector<MyGameObject>> nextGrid;
};

}