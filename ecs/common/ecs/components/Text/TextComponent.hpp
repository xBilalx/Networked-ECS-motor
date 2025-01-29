#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct TextComponent : public Component {
    sf::Text text;
    sf::Font font;
    std::string content;
    unsigned int size;
    sf::Color color;

    TextComponent(const std::string& textContent, sf::Font& textFont, unsigned int textSize, sf::Color textColor) 
        : content(textContent), size(textSize), color(textColor) {
        font = textFont;
        text.setFont(font);
        text.setString(content);
        text.setCharacterSize(size);
        text.setFillColor(color);
    }
};