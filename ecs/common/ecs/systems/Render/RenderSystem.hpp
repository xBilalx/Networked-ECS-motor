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
    
            // ----- Première passe : dessiner les fonds -----
            // On dessine ici toutes les entités dont le RenderComponent indique qu'il s'agit d'un background.
            for (auto& entity : scene.entities1) {
                RenderComponent* render = scene.getComponent<RenderComponent>(entity.first);
                PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
                if (render && render->isBackground) {
                    if (position)
                        render->sprite.setPosition(position->position.x, position->position.y);
                    else
                        render->sprite.setPosition(0, 0);
                    window.draw(render->sprite);
                }
            }
    
            // ----- Deuxième passe : dessiner les boutons et autres rectangles interactifs avec hover -----
            for (auto& entity : scene.entities1) {
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(entity.first);
                HoverComponent* hover = scene.getComponent<HoverComponent>(entity.first);
                PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
    
                if (rect) {
                    sf::RectangleShape shape(sf::Vector2f(rect->width, rect->height));
                    // Positionnement : privilégier le PositionComponent si présent
                    if (position)
                        shape.setPosition(position->position.x, position->position.y);
                    else
                        shape.setPosition(rect->x, rect->y);
    
                    // Si l'entité possède un HoverComponent, mettre à jour l'état de hover
                    if (hover) {
                        sf::FloatRect bounds = shape.getGlobalBounds();
                        hover->isHovered = bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                        // La couleur est choisie en fonction de l'état de hover
                        shape.setFillColor(hover->isHovered ? hover->hoverColor : rect->color);
                    } else {
                        shape.setFillColor(rect->color);
                    }
                    window.draw(shape);
                }
            }
    
            // ----- Troisième passe : dessiner les textes -----
            for (auto& entity : scene.entities1) {
                TextComponent* textComp = scene.getComponent<TextComponent>(entity.first);
                PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(entity.first);

                if (textComp && position && rect) {
                    float offsetX = rect->width / 2 - textComp->text.getLocalBounds().width / 2;
                    float offsetY = rect->height / 2 - textComp->text.getLocalBounds().height / 2;
                    textComp->text.setPosition(position->position.x + offsetX,
                                               position->position.y + offsetY);
                    window.draw(textComp->text);
                }
                
            }
    
            // ----- Quatrième passe : dessiner les autres sprites interactifs (non background) -----
            for (auto& entity : scene.entities1) {
                RenderComponent* render = scene.getComponent<RenderComponent>(entity.first);
                PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
                // On dessine ici les sprites qui ne sont pas des fonds (isBackground == false)
                if (render && !render->isBackground) {
                    if (position)
                        render->sprite.setPosition(position->position.x, position->position.y);
                    else
                        render->sprite.setPosition(0, 0);
                    window.draw(render->sprite);
                }
            }
    
            window.display();
        }
    
    private:
        sf::RenderWindow window;
    };