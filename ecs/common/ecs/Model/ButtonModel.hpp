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
    struct Button {
        sf::RectangleShape shape;
        sf::Color defaultColor;
        sf::Color hoverColor;
        std::function<void()> onClick;

        Button(float x, float y, float width, float height, sf::Color color, sf::Color hover, std::function<void()> action)
            : defaultColor(color), hoverColor(hover), onClick(action) {
            shape.setPosition(x, y);
            shape.setSize(sf::Vector2f(width, height));
            shape.setFillColor(defaultColor);
        }

        void update(const sf::RenderWindow& window) {
            if (shape.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) {
                shape.setFillColor(hoverColor);
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                    onClick();
                }
            } else {
                shape.setFillColor(defaultColor);
            }
        }

        void draw(sf::RenderWindow& window) {
            window.draw(shape);
        }
    };
};