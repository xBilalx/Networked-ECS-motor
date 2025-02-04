#pragma once

#include "../Component.hpp"

struct PaddleComponent : public Component {
    int playerId;
    float speed;
    float width;  // Largeur du paddle
    float height; // Hauteur du paddle

    PaddleComponent(int playerId, float speed = 5.0f, float width = 20.0f, float height = 100.0f) 
        : playerId(playerId), speed(speed), width(width), height(height) {}
};