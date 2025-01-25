#pragma once

#include "../Component.hpp"

struct InputComponent : public Component {
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;
    bool spaceBar = false;
};