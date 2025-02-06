#pragma once

#include "../components/Transform/PositionComponent.hpp"
#include "../components/Render/RenderComponent.hpp"
#include "../components/Token/TokenComponent.hpp"
#include "../scene/sceneManager.hpp"

class TokenModel {
public:
    TokenModel(Scene& scene, const std::string& texturePath, float x, float y, int playerId) {
        tokenEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(tokenEntity, x, y);
        RenderComponent& renderComponent = scene.addComponent<RenderComponent>(tokenEntity, texturePath, true);
        scene.addComponent<TokenComponent>(tokenEntity, playerId);

        sf::Vector2u textureSize = renderComponent.texture.getSize();
        if (textureSize.x == 0 || textureSize.y == 0) {
            std::cerr << "Erreur: La texture du jeton n'a pas été chargée correctement !" << std::endl;
            return;
        }

        float scaleFactor = 80.0f / static_cast<float>(textureSize.x);
        renderComponent.sprite.setScale(scaleFactor, scaleFactor);
        renderComponent.sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    }

    std::size_t getEntity() const {
        return tokenEntity;
    }

private:
    std::size_t tokenEntity;
};