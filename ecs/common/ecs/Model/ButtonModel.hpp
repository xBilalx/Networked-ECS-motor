#pragma once

#include "../components/Box/HoverComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Menu/MenuComponent.hpp"
#include "../components/Action/OnCliclComponent.hpp"

#include "../entitiesManager.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>

class ButtonModel {
public:
    ButtonModel(Scene& scene, float x, float y, float width, float height, sf::Color color, 
                const std::string& text, sf::Font& font, unsigned int textSize, 
                sf::Color textColor, sf::Color hoverColor, std::function<void (Scene&)> action) {
        buttonEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(buttonEntity, x, y);
        scene.addComponent<RectangleComponent>(buttonEntity, x, y, width, height, color);
        scene.addComponent<TextComponent>(buttonEntity, text, font, textSize, textColor);
        scene.addComponent<HoverComponent>(buttonEntity, color, hoverColor);
        scene.addComponent<OnClickComponent>(buttonEntity, action);
        scene.addComponent<OnClickToggleComponent>(buttonEntity);
        scene.addComponent<RenderComponentTest>(buttonEntity, 1);
    }

    std::size_t getEntity() const {
        return buttonEntity;
    }

private:
    std::size_t buttonEntity;
};