#pragma once

#include "../Component.hpp"
#include <string>
#include <SFML/Graphics.hpp>

struct WindowComponent : public Component
{
    unsigned int modeWidth;
    unsigned int modeHeight;
    std::string WindowName; // 32 char max
    
    WindowComponent(unsigned int modeWidth, unsigned int modeHeight, std::string WindowName = "") : modeWidth(modeWidth), modeHeight(modeHeight), WindowName(WindowName) {}
};
