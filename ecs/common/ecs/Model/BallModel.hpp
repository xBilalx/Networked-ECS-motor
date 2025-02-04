#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Circle/CircleComponent.hpp"


#include "../entitiesManager.hpp"

class BallModel {
public:
    BallModel(Scene& scene, float x, float y, float radius = 10.0f) {
        ballEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(ballEntity, x, y);
        scene.addComponent<CircleComponent>(ballEntity, x, y, radius, sf::Color::White);
    }

    std::size_t getEntity() const {
        return ballEntity;
    }

private:
    std::size_t ballEntity;
};