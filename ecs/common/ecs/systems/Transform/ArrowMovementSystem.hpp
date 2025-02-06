#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"

class ArrowMovementSystem : public ISystem{
public:
    void update(Scene& scene, float dt) {
        // std::cout << "--- [DEBUG] ArrowMovement ---\n";

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(entity.first);
            GridComponent* grid = scene.getComponent<GridComponent>(0);
            InputComponent* input = scene.getComponent<InputComponent>(entity.first);
            ActionKeyBind* actionKey = scene.getComponent<ActionKeyBind>(entity.first);

            InputComponent* inputDEbu = scene.getComponent<InputComponent>(44);

            if (position && arrow && grid && input && actionKey) {

                if (input->isKeyReleased(actionKey->forward) || input->isKeyReleased(actionKey->back)) {
                    return;
                }
                if (input->isKeyReleased(actionKey->left)) {

                    if (arrow->currentColumn > 0) {
                        arrow->currentColumn--;
                    }
                } 

                if (input->isKeyReleased(actionKey->right)) {
                    if (arrow->currentColumn < arrow->maxColumns - 1) {
                        arrow->currentColumn++;
                    }
                }
                position->position.x = grid->columnPositions[arrow->currentColumn];
            }
        }
    }
};
