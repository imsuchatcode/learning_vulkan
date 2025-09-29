#include "grid_system.hpp"
#include <random>

namespace my
{
    GridSystem::GridSystem(Device& myDevice, int width, int height, int pixelPerCell) 
        : cols{width / pixelPerCell}, rows{height / pixelPerCell}{
        std::shared_ptr<MyModel> squareModel = createSquareModel(myDevice);
    
        curGrid.resize(rows);
        nextGrid.resize(rows);

        for (int i = 0; i < rows; i++){
            curGrid[i].resize(cols);
            nextGrid[i].resize(cols);
            
            for (int j = 0; j < cols; j++){
                curGrid[i][j] = std::make_unique<GridCell>();

                auto obj = MyGameObject::createGameObject();
                obj.color = glm::vec3(0.2f, 0.2f, 0.2f);
                obj.model = squareModel;

                float cellWidth = 2.0f / cols;  
                float cellHeight = 2.0f / rows;
                
                float worldX = -1.0f + (j + 0.5f) * cellWidth;
                float worldY = -1.0f + (i + 0.5f) * cellHeight;
                
                obj.transform2d.translation = {worldX, worldY};

                obj.transform2d.scale = {cellWidth * 0.9f, cellHeight * 0.9f};

                gameObjects.push_back(std::move(obj));
            }
        }
    }
           
    std::unique_ptr<MyModel> GridSystem::createSquareModel(Device &myDevice){
        std::vector<MyModel::Vertex> vertices = { {{-0.5f, -0.5f}, {1.f, 1.f, 1.f}}, {{0.5f, 0.5f}, {1.f, 1.f, 1.f}},
                                                  {{-0.5f, 0.5f}, {1.f, 1.f, 1.f}}, {{-0.5f, -0.5f}, {1.f, 1.f, 1.f}},
                                                  {{0.5f, -0.5f}, {1.f, 1.f, 1.f}}, {{0.5f, 0.5f}, {1.f, 1.f, 1.f}}};
        return std::make_unique<MyModel>(myDevice, vertices);
    }

    void GridSystem::makeRanCellAlive(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distrib(0, 10);

        for (int i = 0; i < curGrid.size(); i++){
            for (int j = 0; j < curGrid[i].size(); j++){

                int randomNum = distrib(gen);
                if (randomNum == 1 || randomNum == 2 || randomNum == 3){
                    curGrid[i][j]->setAlive();
                    curGrid[i][j]->setColor();
                }
            }
        }
        updateGameObjectsColor();
    }

    int GridSystem::countAliveNeighbor(int row, int col){
        int count = 0;

        for (int di = -1; di <= 1; di++){
            for (int dj = -1; dj <= 1; dj++){
                if (di == 0 && dj == 0){
                    continue;
                }

                int rowNeighbor = row + di;
                int colNeighbor = col + dj;

                if (checkInBound(rowNeighbor, colNeighbor) && curGrid[rowNeighbor][colNeighbor]->isAlive()){
                    count++;
                }
            }
        }
        
        return count;
    }

    bool GridSystem::checkInBound(int row, int col){
        return row >= 0 && row < rows && col >= 0 && col < cols;
    }

    void GridSystem::updateGameObjectsColor(){
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                int gameObjectIndex = cellToGameObjects(i, j);
                gameObjects[gameObjectIndex].color = curGrid[i][j]->getColor();
            }
        }
    }
}