#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct TextComponent : public Component {
    sf::Text text;
    sf::Font& font;
    sf::Font fontDuBled;
    std::string content;
    unsigned int size;
    sf::Color color;

    TextComponent(const std::string& textContent, sf::Font& textFont, unsigned int textSize, sf::Color textColor)
        : font(textFont), content(textContent), size(textSize), color(textColor) {

        if (!fontDuBled.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) { // ⚠️ Le  path de cette font passe-t-elle sur linux ??   
            std::cout << "Error font didn't load" << std::endl;
        }
            text.setFont(fontDuBled);
            text.setString(content);
            text.setCharacterSize(size);
            text.setFillColor(color);
            // std::cerr << "✅ TextComponent initialisé avec succès !\n";
    }
    void updateText() {
        text.setString(content);
    }
};
