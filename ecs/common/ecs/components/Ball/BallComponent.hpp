#pragma once
#include "../Component.hpp"
#include <SFML/System/Vector2.hpp>

struct BallComponent : public Component {
    sf::Vector2f velocity;
    float speed;
    float respawnTime;
    bool isRespawning;
    float screenWidth;
    float screenHeight;

    BallComponent(float speed, float screenWidth, float screenHeight)
        : speed(speed), velocity(sf::Vector2f(-speed, 0)), respawnTime(0.0f), 
          isRespawning(false), screenWidth(screenWidth), screenHeight(screenHeight) {}
};
