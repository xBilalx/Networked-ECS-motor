#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Token/TokenComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../Model/TokenModel.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>


class TokenPlacementSystem {
public:
    void update(Scene& scene) {
        static bool spacePressed = false;
        static int currentPlayer = 1; // 1 = Joueur 1, 2 = Joueur 2

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0);

            if (position && arrow && grid) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
                    spacePressed = true;

                    int row = findAvailableRow(grid, arrow->currentColumn);
                    if (row == -1) {
                        std::cerr << "⚠️ Colonne pleine !" << std::endl;
                        return;
                    }

                    float tokenX = grid->columnPositions[arrow->currentColumn];
                    float tokenY = grid->gridOffsetY + row * grid->cellSize + (grid->cellSize / 2.0f);

                    std::string texturePath = (currentPlayer == 1) ? "../assets/blue_bubble.png" : "../assets/yellow_bubble.png";
                    TokenModel token(scene, texturePath, tokenX, tokenY, currentPlayer);

                    grid->gridState[row][arrow->currentColumn] = currentPlayer; // Mise à jour de la grille
                    checkVictory(grid, row, arrow->currentColumn, currentPlayer); // Vérifier si le joueur a gagné

                    currentPlayer = (currentPlayer == 1) ? 2 : 1;
                }
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacePressed = false;
        }
    }

private:
    int findAvailableRow(GridComponent* grid, int col) {
        for (int row = grid->rows - 1; row >= 0; row--) {
            if (grid->gridState[row][col] == 0) {
                return row;
            }
        }
        return -1;
    }

    void checkVictory(GridComponent* grid, int row, int col, int player) {
        if (checkDirection(grid, row, col, player, 1, 0) ||
            checkDirection(grid, row, col, player, 0, 1) ||
            checkDirection(grid, row, col, player, 1, 1) ||
            checkDirection(grid, row, col, player, 1, -1)) {
            std::cout << "🎉 Joueur " << player << " a gagné !" << std::endl;
        }
    }

    bool checkDirection(GridComponent* grid, int row, int col, int player, int dRow, int dCol) {
        int count = 1;
        count += countTokens(grid, row, col, player, dRow, dCol);
        count += countTokens(grid, row, col, player, -dRow, -dCol);
        return count >= 4;
    }

    int countTokens(GridComponent* grid, int row, int col, int player, int dRow, int dCol) {
        int count = 0;
        for (int i = 1; i < 4; i++) {
            int newRow = row + i * dRow;
            int newCol = col + i * dCol;
            if (newRow < 0 || newRow >= grid->rows || newCol < 0 || newCol >= grid->cols)
                break;
            if (grid->gridState[newRow][newCol] == player) {
                count++;
            } else {
                break;
            }
        }
        return count;
    }
};