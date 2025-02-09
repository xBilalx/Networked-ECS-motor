#pragma once
#include "../Component.hpp"

struct CollisionComponent : public Component {
    float width;
    float height;

    CollisionComponent(float w, float h) : width(w), height(h) {}
};
