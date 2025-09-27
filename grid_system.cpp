#include "grid_system.hpp"

namespace my
{
    GridSystem::GridSystem(Device& myDevice, int width, int height, int pixelPerCell) 
        : cols{width / pixelPerCell}, rows{height / pixelPerCell}{
        std::shared_ptr<MyModel> squareModel = createSquareModel(myDevice);
    
        // Just resize without trying to create empty MyGameObjects
        curGrid.resize(rows);
        nextGrid.resize(rows);

        // Create objects directly
        for (int i = 0; i < rows; i++){
            curGrid[i].reserve(cols);
            nextGrid[i].reserve(cols);
            
            for (int j = 0; j < cols; j++){
                auto obj = MyGameObject::createGameObject();
                obj.color = glm::vec3(0.2f, 0.2f, 0.2f);
                obj.model = squareModel;

                float cellWidth = 2.0f / cols;  
                float cellHeight = 2.0f / rows;
                
                float worldX = -1.0f + (j + 0.5f) * cellWidth;
                float worldY = -1.0f + (i + 0.5f) * cellHeight;
                
                obj.transform2d.translation = {worldX, worldY};
                obj.transform2d.rotation = 0.0f;

                obj.transform2d.scale = {cellWidth * 0.9f, cellHeight * 0.9f};

                curGrid[i].push_back(std::move(obj));
            }
        }
    }

    std::unique_ptr<MyModel> GridSystem::createSquareModel(Device &myDevice){
        std::vector<MyModel::Vertex> vertices = { {{-0.5f, -0.5f}, {1.f, 1.f, 1.f}}, {{0.5f, 0.5f}, {1.f, 1.f, 1.f}},
                                                  {{-0.5f, 0.5f}, {1.f, 1.f, 1.f}}, {{-0.5f, -0.5f}, {1.f, 1.f, 1.f}},
                                                  {{0.5f, -0.5f}, {1.f, 1.f, 1.f}}, {{0.5f, 0.5f}, {1.f, 1.f, 1.f}}};
        return std::make_unique<MyModel>(myDevice, vertices);
    }

}