#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"
#include "../../components/Box/HoverComponent.hpp"
#include "../../components/Text/TextComponent.hpp"
#include "../../components/Circle/CircleComponent.hpp"

#include "../Transform/BounceSystem.hpp"
#include "../Transform/ArrowMovementSystem.hpp"
#include "../Transform/TokenPlacementSystem.hpp"
#include "../Transform/PaddleMovementSystem.hpp"
#include "../Transform/MovementSystem.hpp"




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
        window.setFramerateLimit(120);
    }

    sf::RenderWindow& getWindow() {
        return window;
    }
void update(Scene& scene) {
    window.clear();
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

    BounceSystem bounceSystem;
    ArrowMovementSystem arrowMovementSystem;
    TokenPlacementSystem tokenPlacementSystem;
    
    bounceSystem.update(scene, 1.0f / 60.0f);
    arrowMovementSystem.update(scene);
    tokenPlacementSystem.update(scene);
    // std::cerr << "🛠 Début de RenderSystem::update()\n";

    std::vector<std::pair<RenderComponent*, PositionComponent*>> arrows;
    std::vector<std::pair<RenderComponent*, PositionComponent*>> tokens;
    std::vector<std::pair<CircleComponent*, PositionComponent*>> balls;

    // std::cerr << "🔍 Vérification des entités (Première passe : Fond et Grille)\n";

    for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
        // std::cerr << "   ▶ Traitement de l'entité : " << it->first << "\n";

        RenderComponent* render = scene.getComponent<RenderComponent>(it->first);
        PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
        TokenComponent* token = scene.getComponent<TokenComponent>(it->first);
        CircleComponent* circle = scene.getComponent<CircleComponent>(it->first);

        if (render) {
            // std::cerr << "     ✅ RenderComponent détecté\n";
            if (position) {
                render->sprite.setPosition(position->position.x, position->position.y);
            } else {
                render->sprite.setPosition(0, 0);
            }

            if (render->pathTexture.toAnsiString().find("arrow") != std::string::npos) {
                arrows.push_back({render, position});
            } else if (token) {
                tokens.push_back({render, position});
            } else {
                window.draw(render->sprite);
            }
        }

        if (circle) {
            // std::cerr << "     ✅ CircleComponent détecté\n";
            balls.push_back({circle, position});
        }
    }

    // std::cerr << "🔍 Vérification des entités (Deuxième passe : Textes et Boutons)\n";

    for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
        // std::cerr << "   ▶ Traitement de l'entité (Textes) : " << it->first << "\n";

        RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
        HoverComponent* hover = scene.getComponent<HoverComponent>(it->first);
        TextComponent* textComp = scene.getComponent<TextComponent>(it->first);
        PositionComponent* position = scene.getComponent<PositionComponent>(it->first);

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

        if (textComp && position) {
            try {
                if (textComp->text.getFont() != nullptr) {
                    textComp->text.setPosition(position->position.x, position->position.y);
                    window.draw(textComp->text);
                } else {
                    std::cerr << "⚠️ ERREUR : `TextComponent` référencé sans police valide !\n";
                }
            } catch (const std::exception& e) {
                std::cerr << "❌ ERREUR: Exception attrapée dans RenderSystem lors de l'affichage du texte : " << e.what() << "\n";
            }
        }
    }

    // std::cerr << "🔍 Affichage des objets de jeu\n";

    for (auto& ball : balls) {
        if (ball.second) {
            ball.first->circle.setPosition(ball.second->position.x, ball.second->position.y);
        }
        window.draw(ball.first->circle);
    }

    for (auto& token : tokens) {
        if (token.second) {
            token.first->sprite.setPosition(token.second->position.x, token.second->position.y);
        }
        window.draw(token.first->sprite);
    }

    for (auto& arrow : arrows) {
        if (arrow.second) {
            arrow.first->sprite.setPosition(arrow.second->position.x, arrow.second->position.y);
        }
        window.draw(arrow.first->sprite);
    }

    window.display();
    // std::cerr << "✅ Fin de RenderSystem::update()\n";
}


private:
    sf::RenderWindow window;
};