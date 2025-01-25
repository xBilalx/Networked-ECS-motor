#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"

class InputSystem {
public:

    void update(EntityManager& entityManager, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = entityManager.entities1.begin(); it != entityManager.entities1.end(); it++) {
                InputComponent* input = entityManager.getComponentTest<InputComponent>(it->first);
                BindClientComponent* bind = entityManager.getComponentTest<BindClientComponent>(it->first);
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
    void updateForServer(EntityManager& entityManager, sf::RenderWindow& win) {
        if (win.isOpen() && win.hasFocus()) {
            for (auto it = entityManager.entities1.begin(); it != entityManager.entities1.end(); it++) {
                InputComponent* input = entityManager.getComponentTest<InputComponent>(it->first);
                BindClientComponent* bind = entityManager.getComponentTest<BindClientComponent>(it->first);
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