#pragma once

#include "../../ecs/common/ecs/components/Grid/GridComponent.hpp"
#include "../../ecs/common/ecs/systems/Grid/GridSystem.hpp"
#include <cstdint>
#include <vector>
#include <utility>
#include <string>

class Power4LogicSystem {
public:
    // Vérifie si un joueur a gagné en connectant un certain nombre de jetons
    bool checkWin(const GridSystem& gridSystem, const GridComponent& grid, int playerId, size_t connectCount = 4) {
        winningCells.clear(); // Réinitialise les cellules gagnantes
        size_t rows = grid.getRows();
        size_t cols = grid.getCols();

        // Parcours la grille pour vérifier les alignements
        for (size_t row = 0; row < rows; ++row) {
            for (size_t col = 0; col < cols; ++col) {
                // Vérification horizontale
                if (checkDirection(grid, row, col, 0, 1, playerId, connectCount)) {
                    return true;
                }
                // Vérification verticale
                if (checkDirection(grid, row, col, 1, 0, playerId, connectCount)) {
                    return true;
                }
                // Vérification diagonale (\\)
                if (checkDirection(grid, row, col, 1, 1, playerId, connectCount)) {
                    return true;
                }
                // Vérification diagonale (/)
                if (checkDirection(grid, row, col, -1, 1, playerId, connectCount)) {
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
        winningCells.clear();
    }

    // Retourne les cellules gagnantes
    const std::vector<std::pair<size_t, size_t>>& getWinningCells() const {
        return winningCells;
    }

    // Remplace les bulles gagnantes par le sprite "bubble_pop"
    void replaceWinningBubbles(GridComponent& grid) {
        for (const auto& cell : winningCells) {
            grid.setCell(cell.first, cell.second, -1); // -1 représente les cellules "popées"
        }
    }

    // Affiche un message de victoire
    std::string getWinMessage(int playerId) const {
        return (playerId == 1) ? "Player Blue won the game!" : "Player Yellow won the game!";
    }

private:
    std::vector<std::pair<size_t, size_t>> winningCells; // Liste des cellules gagnantes

    // Vérifie une direction spécifique pour un alignement gagnant
    bool checkDirection(const GridComponent& grid, size_t startRow, size_t startCol, int dRow, int dCol, int playerId, size_t connectCount) {
        size_t rows = grid.getRows();
        size_t cols = grid.getCols();
        std::vector<std::pair<size_t, size_t>> tempCells;

        for (size_t i = 0; i < connectCount; ++i) {
            size_t row = startRow + i * dRow;
            size_t col = startCol + i * dCol;

            if (row >= rows || col >= cols || grid.getCell(row, col) != playerId) {
                return false;
            }
            tempCells.emplace_back(row, col);
        }

        // Si alignement trouvé, enregistrer les cellules gagnantes
        winningCells = tempCells;
        return true;
    }
};

enum class Power4MessageType : uint8_t {
    ACTION_PLACE_TOKEN,  // Client -> Serveur : Joueur place un jeton
    GAME_STATE_UPDATE,   // Serveur -> Client : Mise à jour de l'état du jeu
};
