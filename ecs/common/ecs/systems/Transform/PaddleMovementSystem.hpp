#pragma once

#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Paddle/PaddleComponent.hpp"
#include "../../scene/sceneManager.hpp"
#include "../../entitiesManager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "../../systems/Render/RenderSystem.hpp" 
#include "../../systems/Render/RenderSystem.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/PlayerInput/PlayerInputComponent.hpp"


class PaddleMovementSystem {
public:
    void update(Scene& scene) {
        std::cerr << "🛠 Mise à jour de PaddleMovementSystem\n";

        // Parcours de chaque entité possédant les composants nécessaires
        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            PaddleComponent* paddle = scene.getComponent<PaddleComponent>(entity.first);
            PlayerInputComponent* input = scene.getComponent<PlayerInputComponent>(entity.first);

            if (position && paddle && input) {
                float paddleSpeed = paddle->speed;

                std::cerr << "🔹 Paddle détecté (ID: " << paddle->playerId 
                          << ") | X: " << position->position.x 
                          << " | Y: " << position->position.y << "\n";

                bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

                // Association des touches en fonction du joueur
                if (input->playerId == 1) {
                    moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
                    moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
                    moveUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
                    moveDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
                }
                else if (input->playerId == 2) {
                    moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                    moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                    moveUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                    moveDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                }

                // Déplacement horizontal
                if (moveLeft && position->position.x > paddle->minX) {
                    position->position.x -= paddleSpeed;
                    std::cerr << "⬅️ Joueur " << paddle->playerId << " va à gauche\n";
                }
                if (moveRight && position->position.x < paddle->maxX) {
                    position->position.x += paddleSpeed;
                    std::cerr << "➡️ Joueur " << paddle->playerId << " va à droite\n";
                }

                // Déplacement vertical
                // La position représente le haut du paddle ; 
                // pour que le bas touche le bas de la fenêtre, la position Y peut aller jusqu'à maxY = windowSize.y - height.
                if (moveUp && position->position.y > paddle->minY) {
                    position->position.y -= paddleSpeed;
                    std::cerr << "⬆️ Joueur " << paddle->playerId << " monte\n";
                }
                if (moveDown && position->position.y < paddle->maxY) {
                    position->position.y += paddleSpeed;
                    std::cerr << "⬇️ Joueur " << paddle->playerId << " descend\n";
                }

                // Application finale du clamp pour s'assurer que la position reste dans les limites autorisées
                position->position.x = std::clamp(position->position.x, paddle->minX, paddle->maxX);
                position->position.y = std::clamp(position->position.y, paddle->minY, paddle->maxY);
            }
        }
    }
};