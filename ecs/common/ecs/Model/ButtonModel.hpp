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
    ButtonModel(Scene& scene, float x, float y, float width, float height, sf::Color color, const std::string& text, sf::Font& font, unsigned int textSize, sf::Color textColor) {
        buttonEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(buttonEntity, x, y);
        scene.addComponent<RectangleComponent>(buttonEntity, x, y, width, height, color); // Ajout de la couleur
        scene.addComponent<RenderComponent>(buttonEntity, "", true); // Assure que le bouton s'affiche
        scene.addComponent<TextComponent>(buttonEntity, text, font, textSize, textColor); // Ajout du texte
        scene.addComponent<HoverComponent>(buttonEntity, color, sf::Color::White); // Gère le survol
    }

    std::size_t getEntity() const {
        return buttonEntity;
    }

private:
    std::size_t buttonEntity;
};