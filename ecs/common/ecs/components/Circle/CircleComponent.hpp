#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>

struct CircleComponent : public Component {
    sf::CircleShape circle;

    CircleComponent(float x, float y, float radius, sf::Color color) {
        circle.setRadius(radius);
        circle.setFillColor(color);
        circle.setOrigin(radius, radius);
        circle.setPosition(x, y);
    }
};
