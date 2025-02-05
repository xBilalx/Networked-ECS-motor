#pragma once

#include "../ISystem.hpp"

class MovementSystem : public ISystem {
public:
    void update(Scene& scene, float dt) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            // InputComponent* input = scene.getComponent<InputComponent>(it->first);
            // PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
            // ArrowComponent* arrow = scene.getComponent<ArrowComponent>(it->first);

            // if (input && position) {
            //     if (arrow) {
            //         if (input->moveLeft)
            //             position->position.x -= 3.0f;
            //         if (input->moveRight)
            //             position->position.x += 3.0f;
            //     } else {
            //         if (input->moveLeft)
            //             position->position.x -= 3.0f;
            //         if (input->moveRight)
            //             position->position.x += 3.0f;
            //         if (input->moveUp)
            //             position->position.y -= 3.0f;
            //         if (input->moveDown)
            //             position->position.y += 3.0f;
            //     }
            // }
        }
    }

};