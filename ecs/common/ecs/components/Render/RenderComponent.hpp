#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

struct RenderComponent : public Component {
    sf::Texture texture;
    sf::Sprite sprite;
    std::string pathTexture;
    bool newTexture = false;

    RenderComponent(std::string pathTexture, bool load) : pathTexture(pathTexture) {
        if (load) {
            if (!texture.loadFromFile(pathTexture)) {
                std::cerr << "Erreur: Impossible de charger la texture \"" << pathTexture << "\"\n";
                return;
            }
            sprite.setTexture(texture);
            sprite.setOrigin(texture.getSize().x / 2.0f, texture.getSize().y / 2.0f);
        }
    }

    void loadTecture() {
        if (!texture.loadFromFile(pathTexture)) {
            std::cerr << "Impossible de charger la texture de la flèche: " << pathTexture << std::endl;
        }
        newTexture = true;
    }

    void setScale(float scaleX, float scaleY) {
        sprite.setScale(scaleX, scaleY);
    }
};
