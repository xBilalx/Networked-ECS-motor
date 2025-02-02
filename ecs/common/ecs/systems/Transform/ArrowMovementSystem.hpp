#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"

class ArrowMovementSystem {
public:
    void update(Scene& scene) {
        static std::unordered_map<int, bool> keyPressedState; // Stocke l'état des touches

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0); // Supposons que la grille est stockée avec `ID=0`
            InputComponent* input = scene.getComponent<InputComponent>(entity.first);

            if (position && arrow && grid && input) {
                // Désactiver le mouvement vertical
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    return;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    if (!keyPressedState[sf::Keyboard::Left] && arrow->currentColumn > 0) {
                        arrow->currentColumn--;
                        keyPressedState[sf::Keyboard::Left] = true;
                    }
                } else {
                    keyPressedState[sf::Keyboard::Left] = false;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    if (!keyPressedState[sf::Keyboard::Right] && arrow->currentColumn < arrow->maxColumns - 1) {
                        arrow->currentColumn++;
                        keyPressedState[sf::Keyboard::Right] = true;
                    }
                } else {
                    keyPressedState[sf::Keyboard::Right] = false;
                }

                // Mise à jour de la position X uniquement
                position->position.x = grid->columnPositions[arrow->currentColumn];
            }
        }
    }
};
