#pragma once

#include "../components/Box/HoverComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Menu/MenuComponent.hpp"
#include "../entitiesManager.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class ButtonModel {
public:
    ButtonModel(Scene& scene, float x, float y, float width, float height, sf::Color color, 
                const std::string& text, sf::Font& font, unsigned int textSize, 
                sf::Color textColor, sf::Color hoverColor) {
        buttonEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(buttonEntity, x, y);
        scene.addComponent<RectangleComponent>(buttonEntity, x, y, width, height, color); // Ajout de la couleur du bouton
        scene.addComponent<RenderComponent>(buttonEntity, "", true); // Assure que le bouton est affiché
        scene.addComponent<TextComponent>(buttonEntity, text, font, textSize, textColor); // Ajout du texte
        scene.addComponent<HoverComponent>(buttonEntity, color, hoverColor); // Gestion du survol
    }

    std::size_t getEntity() const {
        return buttonEntity;
    }

private:
    std::size_t buttonEntity;
};