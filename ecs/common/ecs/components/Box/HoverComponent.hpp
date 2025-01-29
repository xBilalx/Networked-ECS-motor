#pragma once

#include "../Component.hpp"

struct HoverComponent : public Component {
    bool isHovered;

    HoverComponent(bool hovered = false) : isHovered(hovered) {}
};