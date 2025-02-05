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
                if (input->isKeyReleased(actionKey->forward) || input->isKeyReleased(actionKey->back)) {
                    return;
                }
                // Gestion de la touche "gauche"
                if (input->isKeyReleased(actionKey->left)) {
                    // std::cout << "Touche left released\n";
                    if (arrow->currentColumn > 0) {
                        arrow->currentColumn--;
                    }
                } 

                // Gestion de la touche "droite"
                if (input->isKeyReleased(actionKey->right)) {
                    // std::cout << "Touche Right released\n";
                    if (arrow->currentColumn < arrow->maxColumns - 1) {
                        arrow->currentColumn++;
                    }
                }

                position->position.x = grid->columnPositions[arrow->currentColumn];
            }
        }
    }
};
