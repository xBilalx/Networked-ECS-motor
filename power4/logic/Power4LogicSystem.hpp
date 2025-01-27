#pragma once

#include "../../ecs/common/ecs/components/Grid/GridComponent.hpp"
#include "../../ecs/common/ecs/systems/Grid/GridSystem.hpp"
#include <cstdint>

class Power4LogicSystem {
public:
    // Vérifie si un joueur a gagné en connectant un certain nombre de jetons
    bool checkWin(const GridSystem& gridSystem, const GridComponent& grid, int playerId, size_t connectCount = 4) {
        size_t rows = grid.getRows();
        size_t cols = grid.getCols();

        // Parcours la grille pour vérifier les alignements
        for (size_t row = 0; row < rows; ++row) {
            for (size_t col = 0; col < cols; ++col) {
                // Vérification horizontale
                if (gridSystem.checkPattern(grid, row, col, 0, 1, std::vector<int>(connectCount, playerId))) {
                    return true;
                }
                // Vérification verticale
                if (gridSystem.checkPattern(grid, row, col, 1, 0, std::vector<int>(connectCount, playerId))) {
                    return true;
                }
                // Vérification diagonale (\\)
                if (gridSystem.checkPattern(grid, row, col, 1, 1, std::vector<int>(connectCount, playerId))) {
                    return true;
                }
                // Vérification diagonale (/)
                if (gridSystem.checkPattern(grid, row, col, -1, 1, std::vector<int>(connectCount, playerId))) {
                    return true;
                }
            }
        }

        return false;
    }

    // Place un jeton pour un joueur et retourne si c'était un placement valide
    bool placeToken(GridSystem& gridSystem, GridComponent& grid, size_t col, int playerId) {
        // Parcours les lignes de bas en haut pour trouver une case vide
        for (size_t row = grid.getRows(); row > 0; --row) {
            if (gridSystem.isValidCell(grid, row - 1, col) && grid.getCell(row - 1, col) == 0) {
                grid.setCell(row - 1, col, playerId);
                return true;
            }
        }
        return false; // Colonne pleine
    }

    // Réinitialise le plateau de jeu
    void resetGame(GridSystem& gridSystem, GridComponent& grid) {
        gridSystem.resetGrid(grid);
    }
};

enum class Power4MessageType : uint8_t {
    ACTION_PLACE_TOKEN,  // Client -> Serveur : Joueur place un jeton
    GAME_STATE_UPDATE,   // Serveur -> Client : Mise à jour de l'état du jeu
};
