#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>

struct PositionComponent : public Component {
    sf::Vector2f position;
    PositionComponent(float x = 0, float y = 0) : position(x, y) {}
};