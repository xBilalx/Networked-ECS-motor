#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Paddle/PaddleComponent.hpp"
#include "../components/PlayerInput/PlayerInputComponent.hpp"



#include "../entitiesManager.hpp"


class PaddleModel {
public:
    PaddleModel(Scene& scene, float x, float y, int playerId, float width = 20.0f, float height = 100.0f) {
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();

        // ✅ Limitation des positions initiales pour éviter les sorties d'écran
        if (y < 0) y = 0;
        if (y + height > windowSize.y) y = windowSize.y - height;
        if (x < 0) x = 0;
        if (x + width > windowSize.x) x = windowSize.x - width;

        paddleEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(paddleEntity, x, y);
        scene.addComponent<RectangleComponent>(paddleEntity, x, y, width, height, sf::Color::White);
        scene.addComponent<PlayerInputComponent>(paddleEntity, playerId); // ✅ Utilisation du `PlayerInputComponent`
        scene.addComponent<PaddleComponent>(paddleEntity, playerId, 5.0f, width, height);
    }

    std::size_t getEntity() const {
        return paddleEntity;
    }

private:
    std::size_t paddleEntity;
};
