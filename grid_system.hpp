#pragma once

#include <memory>

#include "my_model.hpp"
#include "my_game_object.hpp"
#include "device.hpp"
#include "grid_cell.hpp"

namespace my
{
class GridSystem{
public:
    GridSystem(Device& myDevice, int width, int height, int pixelPerCell);
    ~GridSystem() {};

    std::unique_ptr<MyModel> createSquareModel(Device &myDevice);

    std::vector<MyGameObject> & getGrid() {return gameObjects;}

    void makeRanCellAlive();
    void updateGrid(); 

private:
    int rows;
    int cols; 

    int cellToGameObjects(int row, int col) const {return row * cols + col;}
    int countAliveNeighbor(int row, int col);
    bool checkInBound(int row, int col);
    void updateGameObjectsColor();

    std::vector<std::vector<std::unique_ptr<GridCell>>> curGrid;
    std::vector<std::vector<std::unique_ptr<GridCell>>> nextGrid;

    std::vector<MyGameObject> gameObjects;
};

}