#pragma once

#include "../Component.hpp"
#include "../../systems/Network/Serializer.hpp"

struct InputComponent : public Component {
    std::unordered_map<sf::Keyboard::Key, bool> keysPressed;
    std::unordered_map<sf::Keyboard::Key, bool> keysReleased;

    bool isKeyPressed(sf::Keyboard::Key key) const {
        auto it = keysPressed.find(key);
        return it != keysPressed.end() && it->second;
    }

    bool isKeyReleased(sf::Keyboard::Key key) const {
        auto it = keysReleased.find(key);
        return it != keysReleased.end() && it->second;
    }

    void updateKeyReleases() {
        for (int i = sf::Keyboard::A; i < sf::Keyboard::KeyCount; ++i) {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
            bool currentlyPressed = sf::Keyboard::isKeyPressed(key);

            if (keysPressed[key] && !currentlyPressed) {
                keysReleased[key] = true;
            } else {
                keysReleased[key] = false; 
            }

        }
    }
    void serialize(std::string& buffer) const {
        uint32_t pressedCount = static_cast<uint32_t>(keysPressed.size());
        Serializer::serialize(buffer, pressedCount);
        for (const auto& key : keysPressed) {
            Serializer::serialize(buffer, static_cast<uint32_t>(key.first));
            Serializer::serialize(buffer, key.second);
        }

        uint32_t releasedCount = static_cast<uint32_t>(keysReleased.size());
        Serializer::serialize(buffer, releasedCount);
        for (const auto& key : keysReleased) {
            Serializer::serialize(buffer, static_cast<uint32_t>(key.first));
            Serializer::serialize(buffer, key.second);
        }
    }

    void deserialize(const char*& data) {
        keysPressed.clear();
        keysReleased.clear();

        uint32_t pressedCount = Serializer::deserialize<uint32_t>(data);
        for (uint32_t i = 0; i < pressedCount; ++i) {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(Serializer::deserialize<uint32_t>(data));
            bool value = Serializer::deserialize<bool>(data);
            keysPressed[key] = value;
        }
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