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
        static std::unordered_map<int, bool> keyPressedState;
        std::cerr << "🛠 Mise à jour de PaddleMovementSystem\n";

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            PaddleComponent* paddle = scene.getComponent<PaddleComponent>(entity.first);
            PlayerInputComponent* input = scene.getComponent<PlayerInputComponent>(entity.first); // ✅ Remplace `InputComponent`

            if (position && paddle && input) {
                float paddleSpeed = paddle->speed;
                float paddleLeft = position->position.x;
                float paddleRight = position->position.x + paddle->width;
                float paddleTop = position->position.y;
                float paddleBottom = position->position.y + paddle->height;

                std::cerr << "🔹 Paddle détecté (ID: " << paddle->playerId 
                          << ") | X: " << position->position.x 
                          << " | Y: " << position->position.y << "\n";

                // ✅ Vérifier les entrées en fonction du `playerId`
                bool moveUp = false, moveDown = false, moveLeft = false, moveRight = false;

                // 🔹 Joueur 1 : "A" (gauche), "D" (droite), "W" (haut), "S" (bas)
                if (input->playerId == 1) {
                    moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
                    moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
                    moveUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
                    moveDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
                }

                // 🔹 Joueur 2 : Flèche gauche (gauche), Flèche droite (droite), Flèche haut (haut), Flèche bas (bas)
                if (input->playerId == 2) {
                    moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                    moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                    moveUp    = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                    moveDown  = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                }

                // ✅ Appliquer les mouvements UNIQUEMENT au bon joueur
                if (moveLeft) {
                    position->position.x -= paddleSpeed;
                    std::cerr << "⬅️ Joueur " << paddle->playerId << " va à gauche\n";
                }
                if (moveRight) {
                    position->position.x += paddleSpeed;
                    std::cerr << "➡️ Joueur " << paddle->playerId << " va à droite\n";
                }
                if (moveUp) {
                    position->position.y -= paddleSpeed;
                    std::cerr << "⬆️ Joueur " << paddle->playerId << " monte\n";
                }
                if (moveDown) {
                    position->position.y += paddleSpeed;
                    std::cerr << "⬇️ Joueur " << paddle->playerId << " descend\n";
                }
            }
        }
    }
};