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
                obj.color = glm::vec3(1.f);
                obj.model = squareModel;

                float worldX = (i - rows * 0.5f) * (2.0f / rows);
                float worldY = (j - cols * 0.5f) * (2.0f / cols);
                obj.transform2d.translation = {worldX, worldY};
                obj.transform2d.scale = {0.8f / rows, 0.8f / cols};

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