#pragma once

#include "../Component.hpp"

struct BounceComponent : public Component {
    float bounceOffset;
    float bounceSpeed;
    float maxBounce;
    int bounceDirection;

    BounceComponent(float speed = 0.1f, float maxOffset = 10.0f) 
        : bounceOffset(0.0f), bounceSpeed(speed), maxBounce(maxOffset), bounceDirection(1) {}
};