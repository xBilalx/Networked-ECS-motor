#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"
#include "../../components/Box/HoverComponent.hpp"
#include "../../components/Text/TextComponent.hpp"
#include "../Transform/BounceSystem.hpp"
#include "../Transform/ArrowMovementSystem.hpp"
#include "../Transform/TokenPlacementSystem.hpp"


#include <SFML/Graphics.hpp>

// Pour une futur classe
class ISystem
{
    virtual void update(Scene& scene, float dt) = 0;
};

class RenderSystem {
public:
    void createWindow(unsigned int modeWidth, unsigned int modeHeight, std::string windowName) {
        window.create(sf::VideoMode(modeWidth, modeHeight), windowName);
        window.setFramerateLimit(60);
    }

    sf::RenderWindow& getWindow() {
        return window;
    }

    void update(Scene& scene) {
        window.clear();
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        // BounceSystem bounceSystem;
        // ArrowMovementSystem arrowMovementSystem;
        // TokenPlacementSystem tokenPlacementSystem;

        std::vector<std::pair<RenderComponent*, PositionComponent*>> arrows;
        std::vector<std::pair<RenderComponent*, PositionComponent*>> tokens;

        // Première passe : afficher le fond et la grille en premier
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            RenderComponent* render = scene.getComponent<RenderComponent>(it->first);
            PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
            TokenComponent* token = scene.getComponent<TokenComponent>(it->first);

            if (render) {
                if (position) {
                    render->sprite.setPosition(position->position.x, position->position.y);
                } else {
                    render->sprite.setPosition(0, 0);
                }

                if (render->pathTexture.toAnsiString().find("arrow") != std::string::npos) {
                    arrows.push_back({render, position});
                } else if (token) {
                    tokens.push_back({render, position}); // Stocker les jetons pour les afficher en dernier
                } else {
                    window.draw(render->sprite); 
                }
            }
        }

        // Deuxième passe : afficher les boutons et le texte après
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
            HoverComponent* hover = scene.getComponent<HoverComponent>(it->first);
            TextComponent* textComp = scene.getComponent<TextComponent>(it->first);

            if (rect) {
                sf::RectangleShape buttonShape(sf::Vector2f(rect->width, rect->height));
                buttonShape.setPosition(rect->x, rect->y);

                if (hover) {
                    sf::FloatRect buttonBounds(rect->x, rect->y, rect->width, rect->height);
                    hover->isHovered = buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                    buttonShape.setFillColor(hover->isHovered ? hover->hoverColor : rect->color);
                } else {
                    buttonShape.setFillColor(rect->color);
                }

                window.draw(buttonShape);
            }

            if (textComp) {
                if (textComp->text.getFont()) {
                    textComp->text.setPosition(rect->x + rect->width / 4, rect->y + rect->height / 4);
                    window.draw(textComp->text);
                }
            }
        }

        // Troisième passe : afficher les jetons avant la flèche
        for (auto& token : tokens) {
            if (token.second) {
                token.first->sprite.setPosition(token.second->position.x, token.second->position.y);
            }
            window.draw(token.first->sprite);
        }

        // Quatrième passe : afficher les flèches en dernier
        for (auto& arrow : arrows) {
            if (arrow.second) {
                arrow.first->sprite.setPosition(arrow.second->position.x, arrow.second->position.y);
            }
            window.draw(arrow.first->sprite);
        }
        window.display();
    }

private:
    sf::RenderWindow window;
};
