#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>

struct HoverComponent : public Component {
    bool isHovered;
    sf::Color normalColor;
    sf::Color hoverColor;

    HoverComponent(sf::Color normal, sf::Color hover)
        : isHovered(false), normalColor(normal), hoverColor(hover) {}
};