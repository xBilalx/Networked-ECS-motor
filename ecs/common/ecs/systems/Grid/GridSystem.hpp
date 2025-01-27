#pragma once


#include "../../components/Grid/GridComponent.hpp"
#include "../../components/GameState/GameStateComponent.hpp"
#include "../../components/Player/PlayerComponent.hpp"
#include <iostream>
#include <vector>

class GridSystem {
public:
    // Vérifie si une cellule est valide dans une grille
    bool isValidCell(const GridComponent& grid, size_t row, size_t col) const {
        return row < grid.getRows() && col < grid.getCols();
    }

    // Retourne les cellules dans une direction donnée (outil pour détecter des motifs)
    std::vector<int> getLine(const GridComponent& grid, size_t startRow, size_t startCol, int dRow, int dCol, size_t length) const {
        std::vector<int> line;
        for (size_t i = 0; i < length; ++i) {
            size_t row = startRow + i * dRow;
            size_t col = startCol + i * dCol;
            if (isValidCell(grid, row, col)) {
                line.push_back(grid.getCell(row, col));
            } else {
                break;
            }
        }
        return line;
    }

    // Vérifie si un motif existe dans une direction donnée
    bool checkPattern(const GridComponent& grid, size_t startRow, size_t startCol, int dRow, int dCol, const std::vector<int>& pattern) const {
        for (size_t i = 0; i < pattern.size(); ++i) {
            size_t row = startRow + i * dRow;
            size_t col = startCol + i * dCol;
            if (!isValidCell(grid, row, col) || grid.getCell(row, col) != pattern[i]) {
                return false;
            }
        }
        return true;
    }

    // Réinitialise la grille
    void resetGrid(GridComponent& grid) {
        grid.resetGrid();
    }
};
