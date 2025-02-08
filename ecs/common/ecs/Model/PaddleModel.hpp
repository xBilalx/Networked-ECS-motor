#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Paddle/PaddleComponent.hpp"
#include "../components/PlayerInput/PlayerInputComponent.hpp"
// #include "../../scene/sceneManager.hpp"
#include "../entitiesManager.hpp"

class PaddleModel {
public:
    PaddleModel(Scene& scene, std::size_t paddleEntity, float x, float y, int playerId, float width = 20.0f, float height = 100.0f) {
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
        float screenMiddle = windowSize.x / 2.0f;
        float minX = (playerId == 1) ? 0.0f : screenMiddle;
        float maxX = (playerId == 1) ? screenMiddle - width : windowSize.x - width;
        float minY = 0.0f;
        float maxY = windowSize.y - height;
        x = std::clamp(x, minX, maxX);
        y = std::clamp(y, minY, maxY);
        scene.addComponent<PositionComponent>(paddleEntity, x, y);
        scene.addComponent<RectangleComponent>(paddleEntity, x, y, width, height, sf::Color::White);
        scene.addComponent<InputComponent>(paddleEntity);
        scene.addComponent<PlayerInputComponent>(paddleEntity, playerId);
        scene.addComponent<CollisionComponent>(paddleEntity, width, height);
        scene.addComponent<PaddleComponent>(paddleEntity, playerId, 6.0f, width, height, minX, maxX, minY, maxY);
        scene.addComponent<LimitMovementComponent>(paddleEntity,  minX, minY, maxX, maxY);
        scene.addComponent<RenderComponent>(paddleEntity, 1);
    }

    std::size_t getEntity() const {
        return paddleEntity;
    }

private:
    std::size_t paddleEntity;
};