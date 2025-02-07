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
    
            std::vector<std::pair<RenderComponent*, PositionComponent*>> arrows;
            std::vector<std::pair<RenderComponent*, PositionComponent*>> tokens;
            std::vector<std::pair<CircleComponent*, PositionComponent*>> balls;
            std::vector<std::pair<PaddleComponent*, PositionComponent*>> paddles;
    
            // 🔹 Première passe : affichage des éléments de base (ligne centrale, autres éléments fixes)
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
    
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
                RenderComponent* render = scene.getComponent<RenderComponent>(it->first);
                PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
                TokenComponent* token = scene.getComponent<TokenComponent>(it->first);
                CircleComponent* circle = scene.getComponent<CircleComponent>(it->first);
                PaddleComponent* paddle = scene.getComponent<PaddleComponent>(it->first);

                
                // 🔹 Dessiner les éléments statiques (ligne centrale, obstacles, autres rectangles)
                if (rect && !paddle) {
                    sf::RectangleShape shape(sf::Vector2f(rect->width, rect->height));
                    if (!position) { // C'est pas bon ca
                        shape.setPosition(rect->x, rect->y);
                    } else {
                        shape.setPosition(position->position.x, position->position.y);
                    }
                    shape.setFillColor(rect->color);
                    window.draw(shape);
                }
    
                // 🔹 Gérer les sprites (flèches, jetons, balles)
                if (render) {
    
                    if (position) {
                        render->sprite.setPosition(position->position.x, position->position.y);
                    } else {
                        render->sprite.setPosition(0, 0);
                    }
    
                    if (render->pathTexture.find("arrow") != std::string::npos) {
                        arrows.push_back({render, position});
                    } else if (token) {
                        tokens.push_back({render, position});
                    } else {
                        window.draw(render->sprite);
                    }
                }
    
                // 🔹 Stocker les balles pour affichage
                if (circle) {
    
                    balls.push_back({circle, position});
                }
    
                // 🔹 Stocker les paddles pour les dessiner plus tard
                if (paddle && position) {
    
                    paddles.push_back({paddle, position});
                }
            }
    
            // 🔹 Deuxième passe : afficher les textes
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                TextComponent* textComp = scene.getComponent<TextComponent>(it->first);
                PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
                TextFieldComponent *textF = scene.getComponent<TextFieldComponent>(it->first);
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
                HoverComponent* hover = scene.getComponent<HoverComponent>(it->first);

                if (rect && hover) {
                    sf::RectangleShape buttonShape(sf::Vector2f(rect->width, rect->height));
                    if (position) {
                        buttonShape.setPosition(position->position.x, position->position.y);
                    } else {
                        buttonShape.setPosition(rect->x, rect->y);
                    }
                    if (hover) {
                        sf::FloatRect buttonBounds(rect->x, rect->y, rect->width, rect->height);
                        hover->isHovered = buttonBounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
                        buttonShape.setFillColor(hover->isHovered ? hover->hoverColor : rect->color);
                    } else {
                        buttonShape.setFillColor(rect->color);
                    }
                    window.draw(buttonShape);
                }

                if (textComp && position && rect) {
                    float offsetX = rect->width / 2 - textComp->text.getLocalBounds().width / 2;
                    float offsetY = rect->height / 2 - textComp->text.getLocalBounds().height / 2;
                    textComp->text.setPosition(position->position.x + offsetX,
                                               position->position.y + offsetY);
                    window.draw(textComp->text);
                }
                
                if (textF) {
                    try {
                        if (textF->text.getFont() != nullptr) {
                            window.draw(textF->text);
                        } else {
                            std::cerr << "⚠️ ERREUR : `TextComponent` référencé sans police valide !\n";
                        }
                    } catch (const std::exception& e) {
                        std::cerr << "❌ ERREUR: Exception attrapée dans RenderSystem lors de l'affichage du texte : " << e.what() << "\n";
                    }
                }
            }
    
            // 🔹 Troisième passe : afficher les paddles
            for (auto& paddle : paddles) {
                sf::RectangleShape paddleShape(sf::Vector2f(20.0f, 100.0f)); // ⚠️ Vérifier la bonne taille
                paddleShape.setPosition(paddle.second->position.x, paddle.second->position.y);
                paddleShape.setFillColor(sf::Color::White);
                window.draw(paddleShape);
            }
    
            // 🔹 Quatrième passe : afficher les balles
            for (auto& ball : balls) {
    
                if (ball.second) {
                    ball.first->circle.setPosition(ball.second->position.x, ball.second->position.y);
                }
                window.draw(ball.first->circle);
            }
    
            // 🔹 Cinquième passe : afficher les jetons avant la flèche
            for (auto& token : tokens) {
                if (token.second) {
                    token.first->sprite.setPosition(token.second->position.x, token.second->position.y);
                }
                window.draw(token.first->sprite);
            }
    
            // 🔹 Sixième passe : afficher les flèches en dernier
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