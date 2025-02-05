#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"
#include "../../components/Input/InputComponent.hpp"

class InputSystem { // C'est plus KeyInputSystem
public:
    void update(Scene& scene, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                if (input) {
                    input->keys.clear();
                    for (int i = sf::Keyboard::A; i <= sf::Keyboard::KeyCount; ++i) {
                        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
                        input->keys[key] = sf::Keyboard::isKeyPressed(key);
                    }
                    inputPress = std::any_of(input->keys.begin(), input->keys.end(), [](const auto& pair) {
                        return pair.second;
                    });
                }
            }
        }
    }
    void updateForServer(Scene& scene, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                BindClientComponentTest* bind = scene.getComponent<BindClientComponentTest>(it->first);
                if (input && !bind) {
                    input->keys.clear();
                    for (int i = sf::Keyboard::A; i <= sf::Keyboard::KeyCount; ++i) {
                        sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
                        input->keys[key] = sf::Keyboard::isKeyPressed(key);
                    }

                    inputPress = std::any_of(input->keys.begin(), input->keys.end(), [](const auto& pair) {
                        return pair.second;
                    });

                }
            }
        }
    }

    bool inputPress = false;
};
