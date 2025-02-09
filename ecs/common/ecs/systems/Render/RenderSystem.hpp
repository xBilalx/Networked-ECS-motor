#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"
#include "../../components/Box/HoverComponent.hpp"
#include "../../components/Text/TextComponent.hpp" // ⚠️ Faudrait un pour les 2 TextComponent / TextFieldComponent
#include "../../components/Text/TextFieldComponent.hpp"
#include "../../components/Circle/CircleComponent.hpp"

#include "../Transform/BounceSystem.hpp"
#include "../Transform/ArrowMovementSystem.hpp"
#include "../Transform/TokenPlacementSystem.hpp"
#include "../Transform/PaddleMovementSystem.hpp"
#include "../Transform/BallMovementSystem.hpp"

#include <SFML/Graphics.hpp>

class RenderSystem {
public:
    void createWindow(unsigned int modeWidth, unsigned int modeHeight, std::string windowName) {
        window.create(sf::VideoMode(modeWidth, modeHeight), windowName);
        window.setFramerateLimit(30);
    }

    sf::RenderWindow& getWindow() {
        return window;
    }

    void update(Scene& scene) {
        window.clear();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        std::vector<std::pair<std::size_t, char>> drawEntities;

        for (auto& entity : scene.entities1) {
            RenderComponent* render = scene.getComponent<RenderComponent>(entity.first);
                
            char zIndex = render ? render->zIndex : 0;
            if (render) {
                drawEntities.push_back({entity.first, zIndex});
            }
        }

        std::sort(drawEntities.begin(), drawEntities.end(), [](std::pair<std::size_t, char> &e1, std::pair<std::size_t, char> &e2){ return e1.second < e2.second; });

        for (auto& entity : drawEntities) {
            RectangleComponent* rect = scene.getComponent<RectangleComponent>(entity.first);
            CircleComponent* circle = scene.getComponent<CircleComponent>(entity.first);
            TextComponent* textComp = scene.getComponent<TextComponent>(entity.first);
            TextFieldComponent* textField = scene.getComponent<TextFieldComponent>(entity.first);
            // PaddleComponent* paddle = scene.getComponent<PaddleComponent>(entity.first);
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            SpriteComponent* sprite = scene.getComponent<SpriteComponent>(entity.first); // Sprite faudrait l'appeller

            if (rect) {
                sf::RectangleShape shape(sf::Vector2f(rect->width, rect->height));
                if (!position) { // C'est pas bon ca
                    shape.setPosition(rect->x, rect->y);
                } else {
                    shape.setPosition(position->position.x, position->position.y);
                }
                shape.setFillColor(rect->color);
                window.draw(shape);
            }
            if (circle) {
                if (position) {
                    circle->circle.setPosition(position->position.x, position->position.y);
                }
                window.draw(circle->circle);
            }
            if (sprite) {
                if (position) {
                    sprite->sprite.setPosition(position->position.x, position->position.y);
                }
                window.draw(sprite->sprite);
            }
            if (textField) {
                if (textField->text.getFont() != nullptr) {
                    window.draw(textField->text);
                }
            }
            if (textComp) {
                if (textComp->text.getFont() != nullptr) {
                    if (position) {
                        textComp->text.setPosition(position->position.x, position->position.y);
                    }
                    window.draw(textComp->text);
                }
            }
        }
        window.display();
    }

private:
    sf::RenderWindow window;
};
