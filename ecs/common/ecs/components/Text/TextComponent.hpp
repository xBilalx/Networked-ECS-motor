#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <string>

struct TextComponent : public Component {
    sf::Text text;
    sf::Font& font;
    std::string content;
    unsigned int size;
    sf::Color color;

    TextComponent(const std::string& textContent, sf::Font& textFont, unsigned int textSize, sf::Color textColor)
        : font(textFont), content(textContent), size(textSize), color(textColor) {

        std::cerr << "🔍 Initialisation du TextComponent avec : " << content << std::endl;

        if (!font.getInfo().family.empty()) {
            text.setFont(font);
            text.setString(content);
            text.setCharacterSize(size);
            text.setFillColor(color);
            std::cerr << "✅ TextComponent initialisé avec succès !\n";
        } else {
            std::cerr << "❌ ERREUR: Police invalide dans TextComponent !" << std::endl;
        }
    }
    void updateText() {
        text.setString(content);
    }
};
