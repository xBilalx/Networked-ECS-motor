#pragma once

#include "../../ecs/common/ecs/components/Grid/GridComponent.hpp"
#include "../../ecs/common/ecs/components/GameState/GameStateComponent.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include "../logic/Power4LogicSystem.hpp"
#include "../../ecs/common/ecs/systems/Grid/GridSystem.hpp"

class Power4Scene {
public:
    Power4Scene()
        : gridComponent(6, 7), // Initialisation de la grille avec 6 lignes et 7 colonnes
          currentPlayer(1) {}

    // Initialisation de la scène
    void initialize() {
        // Réinitialise la grille
        logicSystem.resetGame(gridSystem, gridComponent);
        // Configure l'état du jeu
        gameState.setGameOver(false);
        gameState.setCurrentPlayerId(currentPlayer);

        std::cout << "Power4Scene initialized." << std::endl;
    }

    // Mise à jour de la scène
    void update() {
        if (gameState.getGameOver()) {
            std::cout << "Game Over! Player " << currentPlayer << " wins!" << std::endl;
            return;
        }

        displayGrid();

        // Gestion des entrées : choix de la colonne
        size_t column;
        std::cout << "Player " << currentPlayer << ", choose a column (0-6): ";
        std::cin >> column;

        if (!logicSystem.placeToken(gridSystem, gridComponent, column, currentPlayer)) {
            std::cout << "Invalid move. Try again." << std::endl;
            return;
        }

        // Vérifie si le joueur courant a gagné
        if (logicSystem.checkWin(gridSystem, gridComponent, currentPlayer)) {
            gameState.setGameOver(true);
            return;
        }

        // Passe au joueur suivant
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        gameState.setCurrentPlayerId(currentPlayer);
    }

private:
    GridComponent gridComponent;
    GameStateComponent gameState;
    GridSystem gridSystem;
    Power4LogicSystem logicSystem;
    int currentPlayer;

    // Affiche la grille dans la console
    void displayGrid() const {
        for (size_t row = 0; row < gridComponent.getRows(); ++row) {
            for (size_t col = 0; col < gridComponent.getCols(); ++col) {
                int cell = gridComponent.getCell(row, col);
                if (cell == 0) {
                    std::cout << ". ";
                } else {
                    std::cout << cell << " ";
                }
            }
            std::cout << std::endl;
        }
    }
};
