#pragma once

#include "../Component.hpp"
#include <vector>

struct GridComponent : public Component {
    int rows;
    int cols;
    float cellSize;
    float gridOffsetX;
    float gridOffsetY;
    std::vector<float> columnPositions;
    std::vector<std::vector<int>> gridState; // 0 = vide, 1 = Joueur 1, 2 = Joueur 2

    GridComponent(int rows, int cols, float cellSize, float offsetX, float offsetY)
        : rows(rows), cols(cols), cellSize(cellSize), gridOffsetX(offsetX), gridOffsetY(offsetY) {
        for (int i = 0; i < cols; i++) {
            columnPositions.push_back(gridOffsetX + (i * cellSize) + (cellSize / 2.0f));
        }
        gridState.resize(rows, std::vector<int>(cols, 0)); // Initialisation vide
    }
};
