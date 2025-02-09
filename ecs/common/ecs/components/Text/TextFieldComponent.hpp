#pragma once

#include "../Component.hpp"
#include "../../systems/Network/Serializer.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

struct TextFieldComponent : public Component {
    sf::Font font;
    sf::Text text;
    std::string dataText;
    std::string defaultString; // il sert aussi de référence
    bool isFocus = false;
    sf::Color textColor = sf::Color::Black;

    TextFieldComponent(float x, float y, float widht, float height, const std::string& defaultString = "Texter") : dataText(""), defaultString(defaultString) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) { // ⚠️ Le  path de cette font passe-t-elle sur linux ??   
            std::cout << "Error font didn't load" << std::endl;
        }
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(textColor);
        text.setString(defaultString);
        sf::FloatRect textBounds = text.getLocalBounds();

        float textX = x + (widht / 2.0f) - (textBounds.width / 2.0f);
        float textY = y + (height / 2.0f) - (textBounds.height / 2.0f) - textBounds.top; // Ajustement pour le décalage SFML

        text.setPosition(textX, textY);
    }
    void updateText() {
        text.setString(dataText);
    }
};