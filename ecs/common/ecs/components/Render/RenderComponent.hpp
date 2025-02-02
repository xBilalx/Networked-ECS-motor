#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

struct RenderComponent : public Component {
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String pathTexture;

    RenderComponent(sf::String pathTexture, bool load) : pathTexture(pathTexture) {
        if (load) {
            if (!texture.loadFromFile(pathTexture)) {
                // std::cerr << "Erreur: Impossible de charger la texture \"" << pathTexture.toAnsiString() << "\"\n";
                return;
            }
            sprite.setTexture(texture);
            sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
            // std::cout << "✅ Texture chargée: " << texture.getSize().x << "x" << texture.getSize().y << " pixels.\n";
        }
    }

    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);
        // std::cout << "🔍 Échelle appliquée: X=" << scaleX << ", Y=" << scaleY << std::endl;
    }
};
