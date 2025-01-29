#pragma once

#include "../Component.hpp"

struct BoxComponent : public Component {
    float width;
    float height;

    BoxComponent(float w = 0, float h = 0) : width(w), height(h) {}
};