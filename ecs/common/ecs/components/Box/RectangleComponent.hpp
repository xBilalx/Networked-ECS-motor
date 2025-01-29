#pragma once

#include "../Component.hpp"

struct RectangleComponent : public Component {
    float x;
    float y;
    float width;
    float height;

    RectangleComponent(float posX = 0, float posY = 0, float w = 0, float h = 0)
        : x(posX), y(posY), width(w), height(h) {}
};