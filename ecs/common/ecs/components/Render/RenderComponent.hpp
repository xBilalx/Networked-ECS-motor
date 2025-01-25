#pragma once

#include "../Component.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

struct RenderComponent : public Component{
    sf::Texture texture;
    sf::Sprite sprite;
    sf::String pathTexture;

    RenderComponent(sf::String pathTexture, bool load) : pathTexture(pathTexture) {
        if (load) {
            if (!texture.loadFromFile(pathTexture))
            {
                std::cout << "PAS chargé\n";
                return;
            }
            sprite.setTexture(texture);
        }
    };
};