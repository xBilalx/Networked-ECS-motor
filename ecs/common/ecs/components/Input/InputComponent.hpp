#pragma once

#include "../Component.hpp"
#include "../../systems/Network/Serializer.hpp"

struct InputComponent : public Component {
    std::unordered_map<sf::Keyboard::Key, bool> keysPressed;
    std::unordered_map<sf::Keyboard::Key, bool> keysReleased;

    InputComponent() {
        for (sf::Keyboard::Key key : trackedKeys) {
            keysPressed[key] = false;
            keysReleased[key] = false;
        }
    }

    // Liste touches à serializer (car sinon trop de touches à envoyé)
    const std::vector<sf::Keyboard::Key> trackedKeys = {
        sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right,
        sf::Keyboard::Space, sf::Keyboard::Z, sf::Keyboard::Q, sf::Keyboard::S, sf::Keyboard::D
    };

    bool isKeyPressed(sf::Keyboard::Key key) const {
        auto it = keysPressed.find(key);
        return it != keysPressed.end() && it->second;
    }

    bool isKeyReleased(sf::Keyboard::Key key) const {
        auto it = keysReleased.find(key);
        return it != keysReleased.end() && it->second;
    }

    void updateKeyReleases() {
        for (sf::Keyboard::Key key : trackedKeys) {
            bool currentlyPressed = sf::Keyboard::isKeyPressed(key);

            if (keysPressed[key] && !currentlyPressed) {
                keysReleased[key] = true;
            } else {
                keysReleased[key] = false;
            }

            keysPressed[key] = currentlyPressed;
        }
    }

    void serialize(std::string& buffer) const {
        // Sérialisation des touches pressées (seulement les touches suivies)
        uint32_t pressedCount = 0;
        for (sf::Keyboard::Key key : trackedKeys) {
            if (keysPressed.find(key) != keysPressed.end() && keysPressed.at(key)) {
                pressedCount++;
            }
        }
        Serializer::serialize(buffer, pressedCount);
        for (sf::Keyboard::Key key : trackedKeys) {
            if (keysPressed.find(key) != keysPressed.end() && keysPressed.at(key)) {
                Serializer::serialize(buffer, static_cast<uint32_t>(key));
                Serializer::serialize(buffer, true);
            }
        }
        // Sérialisation des touches relâchées (seulement les touches suivies)
        uint32_t releasedCount = 0;
        for (sf::Keyboard::Key key : trackedKeys) {
            if (keysReleased.find(key) != keysReleased.end() && keysReleased.at(key)) {
                releasedCount++;
            }
        }
        Serializer::serialize(buffer, releasedCount);
        for (sf::Keyboard::Key key : trackedKeys) {
            if (keysReleased.find(key) != keysReleased.end() && keysReleased.at(key)) {
                Serializer::serialize(buffer, static_cast<uint32_t>(key));
                Serializer::serialize(buffer, true);
            }
        }
    }

    void deserialize(const char*& data) {
        keysPressed.clear();
        keysReleased.clear();

        // Désérialisation des touches pressées
        uint32_t pressedCount = Serializer::deserialize<uint32_t>(data);
        for (uint32_t i = 0; i < pressedCount; ++i) {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(Serializer::deserialize<uint32_t>(data));
            bool value = Serializer::deserialize<bool>(data);
            keysPressed[key] = value;
        }

        // Désérialisation des touches relâchées
        uint32_t releasedCount = Serializer::deserialize<uint32_t>(data);
        for (uint32_t i = 0; i < releasedCount; ++i) {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(Serializer::deserialize<uint32_t>(data));
            bool value = Serializer::deserialize<bool>(data);
            keysReleased[key] = value;
        }
    }
};

struct ActionKeyBind : public Component {
    sf::Keyboard::Key forward = sf::Keyboard::Unknown;
    sf::Keyboard::Key left = sf::Keyboard::Unknown;
    sf::Keyboard::Key right = sf::Keyboard::Unknown;
    sf::Keyboard::Key back = sf::Keyboard::Unknown;
    sf::Keyboard::Key jump = sf::Keyboard::Unknown;
    sf::Keyboard::Key pause = sf::Keyboard::Unknown;
    sf::Keyboard::Key setting = sf::Keyboard::Unknown;
    sf::Keyboard::Key use = sf::Keyboard::Unknown;
};