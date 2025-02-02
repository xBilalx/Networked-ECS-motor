#pragma once


class MovementSystem {
public:
    void update(Scene& scene) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            InputComponent* input = scene.getComponent<InputComponent>(it->first);
            PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
            ArrowComponent* arrow = scene.getComponent<ArrowComponent>(it->first); // Vérifier si c'est une flèche

            if (input && position) {
                if (arrow) {
                    // Si c'est une flèche, ne pas modifier Y
                    if (input->moveLeft)
                        position->position.x -= 3.0f;
                    if (input->moveRight)
                        position->position.x += 3.0f;
                } else {
                    // Si ce n'est PAS une flèche, permettre le mouvement normal
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
    }
};