#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Action/OnCliclComponent.hpp"
#include "../../components/Box/RectangleComponent.hpp"

#include <SFML/Graphics.hpp>

class OnClickSytem {
    public:
    OnClickSytem(sf::RenderWindow* window) : window(window) {};

    void handleEvent(sf::Event& event, Scene& scene) {
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            onClick(scene, event.mouseButton.x, event.mouseButton.y);
    }

    private:
        void onClick(Scene& scene, int x, int y) {            
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                OnClickComponent* onClick = scene.getComponent<OnClickComponent>(it->first);
                RectangleComponent* rect = scene.getComponent<RectangleComponent>(it->first);
                if (onClick) {
                    sf::FloatRect buttonBounds(rect->x, rect->y, rect->width, rect->height);
                    if (buttonBounds.contains(static_cast<float>(x), static_cast<float>(y))) {
                        onClick->action(scene);
                    }
                }
            }
        }
    sf::RenderWindow* window;
};