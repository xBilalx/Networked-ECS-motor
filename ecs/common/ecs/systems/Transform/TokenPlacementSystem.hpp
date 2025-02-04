#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Token/TokenComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/GameState/GameStateComponent.hpp"
#include "../../Model/TokenModel.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <iostream>



class TokenPlacementSystem {
public:
    void update(Scene& scene) {
        static bool spacePressed = false;
        
        GameStateComponent* gameState = scene.getComponent<GameStateComponent>(0);
        if (!gameState) {
            // std::cerr << "ERREUR: GameStateComponent introuvable !" << std::endl;
            return;
        }
        if (gameState->gameOver) {
            std::cerr << "🎉 La partie est terminée, plus de placements possibles !" << std::endl;
            return;
        }

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0);

            if (position && arrow && grid) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spacePressed) {
                    spacePressed = true;

                    int row = findAvailableRow(grid, arrow->currentColumn);
                    if (row == -1) {
                        std::cerr << "Colonne pleine !" << std::endl;
                        return;
                    }

                    float tokenX = grid->columnPositions[arrow->currentColumn];
                    float tokenY = grid->gridOffsetY + row * grid->cellSize + (grid->cellSize / 2.0f);

                    int currentPlayer = gameState->currentPlayer;
                    std::string texturePath = (currentPlayer == 1) ? "../assets/blue_bubble.png" : "../assets/yellow_bubble.png";
                    TokenModel token(scene, texturePath, tokenX, tokenY, currentPlayer);

                    grid->gridState[row][arrow->currentColumn] = currentPlayer;

                    if (checkVictory(grid, row, arrow->currentColumn, currentPlayer)) {
                        gameState->endGame(currentPlayer);
                        updatePlayerTurnText(scene, "🎉 Player " + std::to_string(currentPlayer) + " won!");
                        return;
                    }

                    gameState->switchPlayer();
                    updatePlayerTurnText(scene, (gameState->currentPlayer == 1) ? "It's Player Blue's turn" : "It's Player Yellow's turn");
                    updateArrowTexture(scene, gameState->currentPlayer);
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

    bool checkVictory(GridComponent* grid, int row, int col, int player) {
        return checkDirection(grid, row, col, player, 1, 0) ||
               checkDirection(grid, row, col, player, 0, 1) ||
               checkDirection(grid, row, col, player, 1, 1) ||
               checkDirection(grid, row, col, player, 1, -1);
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

    void updateArrowTexture(Scene& scene, int currentPlayer) {
        for (auto& entity : scene.entities1) {
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            RenderComponent* render = scene.getComponent<RenderComponent>(entity.first);

            if (arrow && render) {
                std::string newTexture = (currentPlayer == 1) ? "../assets/blue_arrow.png" : "../assets/yellow_arrow.png";
                if (!render->texture.loadFromFile(newTexture)) {
                    std::cerr << "Impossible de charger la texture de la flèche: " << newTexture << std::endl;
                }
                render->sprite.setTexture(render->texture);
            }
        }
    }

    void updatePlayerTurnText(Scene& scene, const std::string& text) {
        for (auto& entity : scene.entities1) {
            TextComponent* textComponent = scene.getComponent<TextComponent>(entity.first);
            if (textComponent) {
                textComponent->text.setString(text);
            }
        }
    }
};