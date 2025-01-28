#pragma once

#include "../Component.hpp"

struct HoverComponent : public Component {
    bool isHovered;

    HoverComponent(float w = 0, float h = 0) : width(w), height(h) {}
};