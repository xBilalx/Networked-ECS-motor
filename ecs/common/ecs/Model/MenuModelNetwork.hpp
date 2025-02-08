#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Text/TextComponent.hpp"
#include "../components/Text/TextFieldComponent.hpp"
#include "../components/Box/RectangleComponent.hpp"
#include "../components/Score/ScoreComponent.hpp"
#include "../components/State/OnClickToggleComponent.hpp"
#include "../systems/Render/RenderSystem.hpp"

#include "../entitiesManager.hpp"
#include "ButtonModel.hpp"
#include "TextFieldModel.hpp"

class MenuModelNetwork {
public:
    MenuModelNetwork(Scene& scene, const std::string& backgroundTexture, 
              std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems, 
              sf::Font& font) {
        
        // Création du menu principal
        menuEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(menuEntity, 0, 0);
        scene.addComponent<SpriteComponent>(menuEntity, backgroundTexture, true);
        scene.addComponent<RenderComponent>(menuEntity, 0);

        // Récupération des dimensions de la fenêtre
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
    
        float defaultButtonWidth = 200;
        float defaultButtonHeight = 50;
        float totalButtonHeight = menuItems.size() * (defaultButtonHeight + 20) - 20;
        float defaultStartY = (windowSize.y - totalButtonHeight) / 2.0f;
        float yOffset = defaultStartY;

        float textFieldWidth = 300;
        float textFieldHeight = 50;
        float textFieldX = (windowSize.x / 2.0f) - (textFieldWidth / 2.0f);

        // 🔹 Ajout des champs de texte (ex: Identifiant & Mot de passe)
        TextFieldModel textField1(scene, textFieldX, yOffset - 100, textFieldWidth, textFieldHeight, "Ip Server");
        TextFieldModel textField2(scene, textFieldX, yOffset - 40, textFieldWidth, textFieldHeight, "Port");

        // Ajout des entités des champs de texte à la liste
        textFieldEntities.push_back(textField1.getEntity());
        textFieldEntities.push_back(textField2.getEntity());

        // Mise à jour du yOffset pour les boutons
        yOffset += 40; 

        // 🔹 Ajout des boutons
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
    std::vector<std::size_t> textFieldEntities; // Ajout des entités des champs de texte
};
