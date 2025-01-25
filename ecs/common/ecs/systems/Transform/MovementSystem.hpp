#pragma once


class MovementSystem {
public:
    void update(EntityManager& entityManager) {
        for (auto it = entityManager.entities1.begin(); it != entityManager.entities1.end(); it++) {
            InputComponent* input = entityManager.getComponentTest<InputComponent>(it->first);
            PositionComponent* position = entityManager.getComponentTest<PositionComponent>(it->first);

            if (input && position) {
                if (input->moveLeft)
                    position->position.x -= 3.0f;
                if (input->moveRight)
                    position->position.x += 3.0f;
                if (input->moveUp)
                    position->position.y -= 3.0f;
                if (input->moveDown)
                    position->position.y += 3.0f;
            }
        }
    }
};