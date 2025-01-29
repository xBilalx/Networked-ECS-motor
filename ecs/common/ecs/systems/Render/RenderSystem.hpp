#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"
#include "../../components/Box/HoverComponent.hpp"
#include "../../components/Text/TextComponent.hpp"

#include <SFML/Graphics.hpp>

// Pour une futur classe
class ISystem
{
    virtual void update(Scene& scene, float dt) = 0;
};

class RenderSystem
{
    public:
        void createWindow(unsigned int modeWidth,unsigned int modeHeight, std::string windowName) {
            window.create(sf::VideoMode(modeWidth, modeHeight), windowName);
            window.setFramerateLimit(120);
        }
        // bool createWindowWithEntity(Scene& em) {
        //     for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
        //         std::cout << "_________-->"<< it->first << " a " << it->second.size()  << " composants" << std::endl;
        //         auto win = it->second.find(std::type_index(typeid(WindowComponent)));
        //         if (win == it->second.end()) {
        //             continue;
        //         }
        //         WindowComponent *winCmpnt = dynamic_cast<WindowComponent*>(win->second.get());
        //         if (winCmpnt) {
        //             window.create(sf::VideoMode(winCmpnt->modeWidth, winCmpnt->modeHeight), winCmpnt->WindowName);
        //             return true;
        //         }
        //     }
        //     return false;
        // }
        sf::RenderWindow& getWindow() {
            return window;
        };
    void update(Scene& scene) {
        window.clear();

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            PositionComponent* position = scene.getComponent<PositionComponent>(it->first);
            RenderComponent* render = scene.getComponent<RenderComponent>(it->first);
            RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
            HoverComponent* hover = scene.getComponent<HoverComponent>(it->first);
            TextComponent* textComp = scene.getComponent<TextComponent>(it->first);

            if (render) { // Affichage des entités avec une texture
                sf::Vector2u windowSize = window.getSize();
                sf::Vector2u textureSize = render->texture.getSize();

                if (textureSize.x > 0 && textureSize.y > 0) { // Éviter division par zéro
                    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
                    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;
                    render->sprite.setScale(scaleX, scaleY);
                }

                if (position) {
                    render->sprite.setPosition(position->position.x, position->position.y);
                } else {
                    render->sprite.setPosition(0, 0);
                }

                window.draw(render->sprite);
            }

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
                if (position) {
                    textComp->text.setPosition(position->position.x + rect->width / 4, position->position.y + rect->height / 4);
                } else {
                    textComp->text.setPosition(0, 0);
                }
                window.draw(textComp->text);
            }
        }
        
        window.display();
    }

    protected:

    private:
        sf::RenderWindow window; // Best way if the window variable would be in WindowComponent ?
};