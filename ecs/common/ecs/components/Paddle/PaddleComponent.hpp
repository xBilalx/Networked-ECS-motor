#pragma once

#include "../Component.hpp"

struct PaddleComponent : public Component {
    int playerId;
    float speed;
    PaddleComponent(int playerId, float speed = 5.0f) 
        : playerId(playerId), speed(speed) {}
};
