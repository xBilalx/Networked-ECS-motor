#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Text/TextFieldComponent.hpp"
#include "../../components/State/OnClickToggleComponent.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"

#include <SFML/Graphics.hpp>

class KeyboardInputSystem  {
public:
    void handleEvent(sf::RenderWindow& window, sf::Event& event, Scene& scene, bool isServer) {
        if (window.hasFocus()) {
            if (event.type == sf::Event::KeyReleased) {
                onKeyRelease(scene, event, isServer);
            }
            if (event.type == sf::Event::KeyPressed) {
                onKeyPressed(scene, event, isServer);
            }
            if (event.type == sf::Event::TextEntered) {
                handleTextInput(scene, event);
            }
        }
    }

    void resetKeyRelease(sf::RenderWindow& window, Scene& scene) {
        if (window.hasFocus()) {
            for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
                InputComponent* input = scene.getComponent<InputComponent>(it->first);
                if (input) {
                    input->updateKeyReleases();
                }
            }
            inputRelease = false;
        }
    }

    bool inputPressed = false;
    bool inputRelease = false;

private:
    void onKeyPressed(Scene& scene, sf::Event& event, bool isServer) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            InputComponent* input = scene.getComponent<InputComponent>(it->first);
            BindClientComponentTest* bind = scene.getComponent<BindClientComponentTest>(it->first);

            if (isServer && bind) {
                continue; // Ignore les clients côté serveur
            }

            if (input) {
                sf::Keyboard::Key pressedKey = event.key.code;
                // std::cout << "[DEBUG] Touche pressée : " << pressedKey << std::endl;
                input->keysPressed[pressedKey] = true;
                inputRelease = true;
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
                // std::cout << "[DEBUG] Touche relâchée : " << releasedKey << std::endl;
                input->keysReleased[releasedKey] = true;
                input->keysPressed[releasedKey] = false;

                inputRelease = true;
                inputPressed = std::any_of(input->keysPressed.begin(), input->keysPressed.end(), [](const auto& pair) {
                    return pair.second;
                });
            }
        }
    }
    void handleTextInput(Scene& scene, sf::Event& event) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            TextFieldComponent* textField  = scene.getComponent<TextFieldComponent>(it->first);
            OnClickToggleComponent* isActive  = scene.getComponent<OnClickToggleComponent>(it->first);

            if (textField && isActive && isActive->isFocus) {
                if (event.text.unicode== 8) {
                    if (!textField->dataText.empty()) {
                        textField->dataText.pop_back();
                    }
                } else if (event.text.unicode >= 32 && event.text.unicode < 128) {
                    textField->dataText += static_cast<char>(event.text.unicode);
                }
                textField->updateText();
            }
        }
    }
};
