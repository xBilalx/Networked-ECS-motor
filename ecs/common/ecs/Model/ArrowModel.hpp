#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Arrow/ArrowComponent.hpp"

#include "../entitiesManager.hpp"

class ArrowModel {
public:
    ArrowModel(Scene& scene, const std::string& texturePath, float cellSize, std::size_t arrowEntity_, int initialColumn = 3) {
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
        float gridOffsetX = (windowSize.x - (7 * cellSize)) / 2.0f;
        float gridOffsetY = (windowSize.y - (6 * cellSize)) / 2.0f;

        float arrowX = gridOffsetX + initialColumn * cellSize + (cellSize / 2.0f);
        float arrowY = gridOffsetY - cellSize + 10;
        arrowEntity = arrowEntity_;
        std::cout << arrowEntity_ << std::endl;
        scene.addComponent<PositionComponent>(arrowEntity, arrowX, arrowY);
        RenderComponent& renderComponent = scene.addComponent<RenderComponent>(arrowEntity, texturePath, true);
        scene.addComponent<InputComponent>(arrowEntity);
        scene.addComponent<BounceComponent>(arrowEntity, 0.3f, 10.0f);
        scene.addComponent<ArrowComponent>(arrowEntity, initialColumn, cellSize, 7); // 7 colonnes
        ActionKeyBind& actionKeyBind = scene.addComponent<ActionKeyBind>(arrowEntity); // 7 colonnes
        

        actionKeyBind.left = sf::Keyboard::Left;
        actionKeyBind.right= sf::Keyboard::Right;


        sf::Vector2u textureSize = renderComponent.texture.getSize();
        if (textureSize.x == 0 || textureSize.y == 0) {
            std::cerr << "Erreur: La texture de la flèche n'a pas été chargée correctement !" << std::endl;
            return;
        }

        float scaleFactor = cellSize / static_cast<float>(textureSize.x);
        renderComponent.sprite.setScale(scaleFactor, scaleFactor);
        renderComponent.sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    }

    std::size_t getEntity() const {
        return arrowEntity;
    }

private:
    std::size_t arrowEntity;
};

