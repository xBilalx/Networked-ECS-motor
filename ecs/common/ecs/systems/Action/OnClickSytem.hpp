#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Action/OnCliclComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"
#include "../../components/State/OnClickToggleComponent.hpp"

#include <SFML/Graphics.hpp>

// Si le Composant OnClickToogle est True alors faire la lambda

class OnClickSytem {
    public:
    OnClickSytem(sf::RenderWindow* window) : window(window) {};

    void handleEvent(sf::Event& event, Scene& scene) {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
            onClickToggle(scene, event.mouseButton.x, event.mouseButton.y);
            onClickAction(scene);
        }
    }

    private:
        void onClickToggle(Scene& scene, int x, int y) {            
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                OnClickToggleComponent* toggle = scene.getComponent<OnClickToggleComponent>(it->first);
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);

                if (toggle && ! rect) {
                    std::cout << "Il y a Toggle sans RECT COmponent" << std::endl;
                }
                if (toggle) {
                    sf::FloatRect buttonBounds(rect->x, rect->y, rect->width, rect->height);
                    if (buttonBounds.contains(static_cast<float>(x), static_cast<float>(y))) {
                        toggle->isFocus = true;
                    } else {
                        toggle->isFocus = false;
                    }
                }
            }
        }
        void onClickAction(Scene& scene) {  
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                OnClickComponent* onClick = scene.getComponent<OnClickComponent>(it->first);
                OnClickToggleComponent* toggle = scene.getComponent<OnClickToggleComponent>(it->first);
                if (onClick && !toggle) {
                    std::cout << "Il y a onClick sans toggle COmponent" << std::endl;
                }
                if (onClick && toggle && toggle->isFocus) {
                    std::cout << "🔴\n";
                    onClick->action(scene);
                }
            }
        }
    sf::RenderWindow* window;
};