#pragma once

#include "../Component.hpp"

#include <vector>
#include <iostream>

// Un composant générique pour gérer une grille 2D
class GridComponent : public Component {
public:
    GridComponent(size_t rows, size_t cols, int defaultValue = 0)
        : rows(rows), cols(cols), grid(rows, std::vector<int>(cols, defaultValue)) {}

    // Accéder à une cellule de la grille
    int getCell(size_t row, size_t col) const {
        if (isValidCell(row, col)) {
            return grid[row][col];
        } else {
            std::cerr << "Invalid cell access: (" << row << ", " << col << ")" << std::endl;
            return -1; // Valeur par défaut pour erreur
        }
    }

    // Mettre à jour une cellule de la grille
    void setCell(size_t row, size_t col, int value) {
        if (isValidCell(row, col)) {
            grid[row][col] = value;
        } else {
            std::cerr << "Invalid cell update: (" << row << ", " << col << ")" << std::endl;
        }
    }

    // Réinitialiser la grille avec une valeur par défaut
    void resetGrid(int defaultValue = 0) {
        for (auto& row : grid) {
            std::fill(row.begin(), row.end(), defaultValue);
        }
    }

    // Afficher la grille pour le débogage
    void printGrid() const {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << cell << " ";
            }
            std::cout << std::endl;
        }
    }

    size_t getRows() const { return rows; }
    size_t getCols() const { return cols; }

private:
    size_t rows;
    size_t cols;
    std::vector<std::vector<int>> grid;

    // Vérifier si une cellule est valide
    bool isValidCell(size_t row, size_t col) const {
        return row < rows && col < cols;
    }
};
