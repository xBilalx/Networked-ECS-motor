#pragma once

#include "../Component.hpp"

struct PaddleComponent : public Component {
    int playerId;
    float speed;
    float width;
    float height;
    float minX; // ✅ Limite gauche/droite
    float maxX;
    float minY; // ✅ Limite haut/bas
    float maxY;

    PaddleComponent(int playerId, float speed = 5.0f, float width = 20.0f, float height = 100.0f, float minX = 0, float maxX = 640, float minY = 0, float maxY = 720)
        : playerId(playerId), speed(speed), width(width), height(height), minX(minX), maxX(maxX), minY(minY), maxY(maxY) {}
};
