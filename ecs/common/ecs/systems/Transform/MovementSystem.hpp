#pragma once

#include "../ISystem.hpp"
#include "../../components/Transform/VelocityComponent.hpp"


// MovementSystem permet de gérer les mouvements des entités en fonction des entrées Clavier
// Peut marcher avec les composants suivant :
// Requis pour le fonctionnement du systeme : 
//  - position : permet de mettre à jour la position de l'entité
//  - input : contient l'état des inputs de l'entité (Key Pressed / Key Released)
//
// Pas Requis :
//  - bindKey : Défini des touches spécifique pour le mouvement (left / right / back/ /forward ). Par défaut ce sont les touches fléchés qui sont bind.
//  - velocity : Permet d'ajouter une velocity spécifique. Par défaut: velocity = 200.0f
//  - limits : Permet de mettre une limite X et Y au mouvement
class MovementSystem : public ISystem {
public:
    void update(Scene& scene, float dt) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            InputComponent* input = scene.getComponent<InputComponent>(it->first);
            PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
            ActionKeyBind* bindKey = scene.getComponent<ActionKeyBind>(it->first);
            VelocityComponent* velocity = scene.getComponent<VelocityComponent>(it->first);
            LimitMovementComponent* limits = scene.getComponent<LimitMovementComponent>(it->first);

            float moveSpeedX = (velocity) ? velocity->velocity.x : 200.0f;
            float moveSpeedY = (velocity) ? velocity->velocity.y: 200.0f;
            if (input && position && bindKey) { // Touche prédéfini
                if (input->isKeyPressed(bindKey->left)){
                    position->position.x -= moveSpeedX * dt;
                }
                if (input->isKeyPressed(bindKey->right))
                    position->position.x += moveSpeedX * dt;
                if (input->isKeyPressed(bindKey->forward))
                    position->position.y -= moveSpeedY * dt;
                if (input->isKeyPressed(bindKey->back))
                    position->position.y += moveSpeedY * dt;
            } else if (input && position) { // Touche par défaut
                if (input->isKeyPressed(sf::Keyboard::Left)){
                    position->position.x -= moveSpeedX * dt;
                }
                if (input->isKeyPressed(sf::Keyboard::Right))
                    position->position.x += moveSpeedX * dt;
                if (input->isKeyPressed(sf::Keyboard::Up))
                    position->position.y -= moveSpeedY * dt;
                if (input->isKeyPressed(sf::Keyboard::Down))
                    position->position.y += moveSpeedY * dt;
            }
            if (limits) {
                position->position.x = std::clamp(position->position.x, limits->minX, limits->maxX);
                position->position.y = std::clamp(position->position.y, limits->minY, limits->maxY);
            }
        }
    }

};