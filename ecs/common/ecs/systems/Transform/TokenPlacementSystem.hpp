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

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0);

            if (position && arrow && grid) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
                    spacePressed = true;

                    // Trouver la ligne disponible
                    int row = findAvailableRow(grid, arrow->currentColumn);
                    if (row == -1) {
                        std::cerr << "⚠️ Colonne pleine !" << std::endl;
                        return;
                    }

                    // ✅ Correction du placement vertical des jetons
                    float tokenX = grid->columnPositions[arrow->currentColumn];
                    float tokenY = grid->gridOffsetY + row * grid->cellSize + (grid->cellSize / 2.0f);

                    int currentPlayer = (turn % 2 == 0) ? 1 : 2;
                    std::string texturePath = (currentPlayer == 1) ? "../assets/blue_bubble.png" : "../assets/yellow_bubble.png";

                    TokenModel token(scene, texturePath, tokenX, tokenY, currentPlayer);
                    grid->gridState[row][arrow->currentColumn] = currentPlayer; 
                    turn++;
                }
            }
        }

        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            spacePressed = false;
        }
    }

private:
    int turn = 0;

    int findAvailableRow(GridComponent* grid, int col) {
        for (int row = grid->rows - 1; row >= 0; row--) {
            if (grid->gridState[row][col] == 0) {
                return row;
            }
        }
        return -1;
    }
};