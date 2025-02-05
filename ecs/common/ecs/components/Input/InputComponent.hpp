#pragma once

#include "../Component.hpp"
#include "../../systems/Network/Serializer.hpp"

struct InputComponent : public Component {
    std::unordered_map<sf::Keyboard::Key, bool> keys;
    std::unordered_map<sf::Keyboard::Key, bool> keyPressedState; 

    bool isKeyPressed(sf::Keyboard::Key key) const {
        auto it = keys.find(key);
        return it != keys.end() && it->second;
    }
    void serialize(std::string& buffer) const {
        uint32_t keyCount = static_cast<uint32_t>(keys.size());
        Serializer::serialize(buffer, keyCount);
        for (const auto& key : keys) {
            Serializer::serialize(buffer, static_cast<uint32_t>(key.first));
            Serializer::serialize(buffer, key.second);
        }
    }
    void deserialize(const char*& data) {
        keys.clear();
        uint32_t keyCount = Serializer::deserialize<uint32_t>(data);
        for (uint32_t i = 0; i < keyCount; ++i) {
            sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(Serializer::deserialize<uint32_t>(data));
            bool value = Serializer::deserialize<bool>(data);
            keys[key] = value;
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