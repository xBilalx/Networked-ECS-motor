#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"

class ArrowMovementSystem {
public:
    void update(Scene& scene) {
        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0);
            InputComponent* input = scene.getComponent<InputComponent>(entity.first);
            ActionKeyBind* actionKey = scene.getComponent<ActionKeyBind>(entity.first);

            if (position && arrow && grid && input && actionKey) {
                // Vérifie si la touche "avant" ou "arrière" est pressée
                if (input->isKeyPressed(actionKey->forward) || input->isKeyPressed(actionKey->back)) {
                    return;
                }

                // Gestion de la touche "gauche"
                if (input->isKeyPressed(actionKey->left)) {
                    if (!input->keyPressedState[actionKey->left] && arrow->currentColumn > 0) {
                        arrow->currentColumn--;
                        input->keyPressedState[actionKey->left] = true;
                    }
                } else {
                    input->keyPressedState[actionKey->left] = false;
                }

                // Gestion de la touche "droite"
                if (input->isKeyPressed(actionKey->right)) {
                    if (!input->keyPressedState[actionKey->right] && arrow->currentColumn < arrow->maxColumns - 1) {
                        std::cout << "PROUT\n";
                        arrow->currentColumn++;
                        input->keyPressedState[actionKey->right] = true;
                    }
                } else {
                    input->keyPressedState[actionKey->right] = false;
                }

                position->position.x = grid->columnPositions[arrow->currentColumn];
            }
        }
    }
};
