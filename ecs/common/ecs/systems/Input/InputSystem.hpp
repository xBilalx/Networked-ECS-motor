#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"
#include "../../components/Input/InputComponent.hpp"

class InputSystem {
public:

    void update(Scene& scene, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                BindClientComponent* bind = scene.getComponent<BindClientComponent>(it->first);
                if (input) {
                    input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                    input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                    input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                    input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                    input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        inputPress = true;
                    }
                }
            }
        }
    }
    void updateForServer(Scene& scene, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                BindClientComponent* bind = scene.getComponent<BindClientComponent>(it->first);
                if (input && !bind) {
                    input->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
                    input->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
                    input->moveUp = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
                    input->moveDown = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
                    input->spaceBar = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        inputPress = true;
                    }
                }
            }
        }
    }
    bool inputPress;
};