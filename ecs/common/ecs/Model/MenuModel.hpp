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
    MenuModel(Scene& scene, const std::string& backgroundTexture, 
              std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems, sf::Font& font) {
        menuEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(menuEntity, 0, 0);
        scene.addComponent<SpriteComponent>(menuEntity, backgroundTexture, true);
        scene.addComponent<RenderComponent>(menuEntity, 0);

        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
    
        float defaultButtonWidth = 200;
        float defaultButtonHeight = 50;
        float totalButtonHeight = menuItems.size() * (defaultButtonHeight + 20) - 20;
        float defaultStartY = (windowSize.y - totalButtonHeight) / 2.0f;
        float yOffset = defaultStartY;

        for (const auto& [text, x, y, width, height, baseColor, hoverColor, textColor, action] : menuItems) {
            float btnWidth = (width == -1) ? defaultButtonWidth : width;
            float btnHeight = (height == -1) ? defaultButtonHeight : height;
            float btnX = (x == -1) ? (windowSize.x / 2.0f) - (btnWidth / 2.0f) : x;
            float btnY = (y == -1) ? yOffset : y;
            ButtonModel button(scene, btnX, btnY, btnWidth, btnHeight, baseColor, text, font, 24, textColor, hoverColor, action);
            buttonEntities.push_back(button.getEntity());
            yOffset += btnHeight + 20; 
        }
    }

private:
    std::size_t menuEntity;
    std::vector<std::size_t> buttonEntities;
};