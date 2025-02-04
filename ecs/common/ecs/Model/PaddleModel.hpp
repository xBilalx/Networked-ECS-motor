#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Input/InputComponent.hpp"
#include "../components/Paddle/PaddleComponent.hpp"


#include "../entitiesManager.hpp"

class PaddleModel {
public:
    PaddleModel(Scene& scene, float x, float y, int playerId, float width = 20.0f, float height = 100.0f) {
        paddleEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(paddleEntity, x, y);
        scene.addComponent<RectangleComponent>(paddleEntity, x, y, width, height, sf::Color::White);
        scene.addComponent<InputComponent>(paddleEntity);
        scene.addComponent<PaddleComponent>(paddleEntity, playerId);
    }

    std::size_t getEntity() const {
        return paddleEntity;
    }

private:
    std::size_t paddleEntity;
};