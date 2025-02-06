#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>


struct VelocityComponent : public Component {
    sf::Vector2f velocity; // Vitesse (x, y)

    VelocityComponent(float vx = 0.0f, float vy = 0.0f) : velocity(vx, vy) {}
};