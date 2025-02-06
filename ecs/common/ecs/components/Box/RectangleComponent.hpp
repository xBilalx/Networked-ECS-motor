#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>

struct RectangleComponent : public Component {
    float x;
    float y;
    float width;
    float height;
    sf::Color color;

    RectangleComponent(float posX = 0, float posY = 0, float w = 0, float h = 0, sf::Color col = sf::Color::White)
        : x(posX), y(posY), width(w), height(h), color(col) {}
};
