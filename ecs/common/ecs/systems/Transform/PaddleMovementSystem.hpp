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

class PaddleMovementSystem {
public:
    void update(Scene& scene, sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        float minY = 0;                 // Bord supérieur
        float maxY = windowSize.y - 100; // Taille du paddle (100px)

        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            PaddleComponent* paddle = scene.getComponent<PaddleComponent>(entity.first);

            if (position && paddle) {
                float speed = 6.0f; // Vitesse du paddle

                // Contrôles du joueur 1 (W / S)
                if (paddle->playerId == 1) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                        position->position.y -= speed;
                        if (position->position.y < minY) {
                            position->position.y = minY; // Bloque au bord supérieur
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                        position->position.y += speed;
                        if (position->position.y > maxY) {
                            position->position.y = maxY; // Bloque au bord inférieur
                        }
                    }
                }

                // Contrôles du joueur 2 (↑ / ↓)
                if (paddle->playerId == 2) {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        position->position.y -= speed;
                        if (position->position.y < minY) {
                            position->position.y = minY; // Bloque au bord supérieur
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        position->position.y += speed;
                        if (position->position.y > maxY) {
                            position->position.y = maxY; // Bloque au bord inférieur
                        }
                    }
                }
            }
        }
    }
};