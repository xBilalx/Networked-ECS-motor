#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"

#include <SFML/Graphics.hpp>

class KeyboardInputSystem  {
    public:

        void handleEvent(sf::Event& event, Scene& scene, bool isServer) {
            if (event.type == sf::Event::KeyReleased) {
                onKeyRelease(scene, event, isServer);
            }
            if (event.type == sf::Event::KeyPressed) {
                onKeyPressed(scene, event, isServer);
            }
        }
        void resetKeyRelease(Scene& scene) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                if (input) {
                    input->updateKeyReleases();
                                    // std::cout << "[DEBUG] Key Right is released" << input->isKeyReleased(sf::Keyboard::Right) << std::endl;

                }
            }
        }
    private:

        void onKeyPressed(Scene& scene, sf::Event& event, bool isServer) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                BindClientComponentTest* bind = scene.getComponent<BindClientComponentTest>(it->first);

                if (isServer && bind) {
                    continue; // Ignore les ienclis côté serveur
                }

                if (input) {
                    sf::Keyboard::Key pressedKey = event.key.code;
                    std::cout << "[DEBUG] Touche pressée : " << pressedKey << std::endl;
                    
                    input->keysPressed[pressedKey] = true;
                }
            }
        }

        void onKeyRelease(Scene& scene, sf::Event& event, bool isServer) {            
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                BindClientComponentTest* bind = scene.getComponent<BindClientComponentTest>(it->first);

                if (isServer && bind) {
                    continue;
                }

                if (input) {
                    sf::Keyboard::Key releasedKey = event.key.code;
                    std::cout << "[DEBUG] Touche relaché : " << releasedKey << std::endl;
                    input->keysReleased[releasedKey] = true;
                    input->keysPressed[releasedKey] = false;
                    inputRelease = std::any_of(input->keysReleased.begin(), input->keysReleased.end(), [](const auto& pair) {
                        return pair.second;
                    });

                }
            }
        }

        bool inputRelease = false;
};