#pragma once
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"

#include "../scene/sceneManager.hpp"
#include <vector>
#include <iostream>


class GridModel {
public:
    GridModel(Scene& scene, const std::string& backgroundTexture, int rows = 6, int cols = 7, float cellSize = 80.0f) 
        : rows(rows), cols(cols), cellSize(cellSize) {
        
        gridEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(gridEntity, 0, 0);
        scene.addComponent<RenderComponent>(gridEntity, backgroundTexture, true);

        // Récupération du RenderSystem depuis SceneManager
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
        
        float gridWidth = cols * cellSize;
        float gridHeight = rows * cellSize;
        float offsetX = (windowSize.x - gridWidth) / 2.0f;
        float offsetY = (windowSize.y - gridHeight) / 2.0f;

        std::cout << "✅ Grille initialisée avec " << rows << " lignes et " << cols << " colonnes." << std::endl;
        std::cout << "📏 Taille des cellules: " << cellSize << " px" << std::endl;
        std::cout << "📍 Position de la grille: X=" << offsetX << ", Y=" << offsetY << std::endl;

        for (int row = 0; row < rows; ++row) {
            for (int col = 0; col < cols; ++col) {
                float cellX = offsetX + col * cellSize;
                float cellY = offsetY + row * cellSize;
                std::size_t cellEntity = scene.createEntity();
                scene.addComponent<PositionComponent>(cellEntity, cellX, cellY);
                scene.addComponent<RectangleComponent>(cellEntity, cellX, cellY, cellSize - 5, cellSize - 5, sf::Color(200, 200, 200, 200));
                gridCells.push_back(cellEntity);
            }
        }
    }

private:
    std::size_t gridEntity;
    int rows, cols;
    float cellSize;
    std::vector<std::size_t> gridCells;
};