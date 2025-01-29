#pragma once

#include "../components/Box/HoverComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Menu/MenuComponent.hpp"
#include "../Model/ButtonModel.hpp"
#include "../entitiesManager.hpp"
#include <vector>
#include <functional>
#include <string>
class MenuModel {
public:
    MenuModel(Scene& scene, const std::string& backgroundTexture, std::vector<std::tuple<std::string, float, float>> menuItems, sf::Font& font) {
        menuEntity = scene.createEntity();
        // scene.addComponent<PositionComponent>(menuEntity, 0, 0);
        // scene.addComponent<RenderComponent>(menuEntity, backgroundTexture, true);
        
        float buttonWidth = 200;
        float buttonHeight = 50;

        for (const auto& [text, x, y] : menuItems) {
            ButtonModel button(scene, x, y, buttonWidth, buttonHeight, sf::Color::Red, text, font, 24, sf::Color::White);
            buttonEntities.push_back(button.getEntity()); // Stocker uniquement l'ID des boutons
        }
    }

    std::size_t getEntity() const {
        return menuEntity;
    }

    const std::vector<std::size_t>& getButtons() const {
        return buttonEntities;
    }

private:
    std::size_t menuEntity;
    std::vector<std::size_t> buttonEntities; // Liste des entités des boutons
};