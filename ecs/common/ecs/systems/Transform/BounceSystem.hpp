#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Bounce/BounceComponent.hpp"

class BounceSystem {
public:
    void update(Scene& scene, float dt) {
        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            BounceComponent* bounce = scene.getComponent<BounceComponent>(entity.first);

            if (position && bounce) {
                bounce->bounceOffset += bounce->bounceSpeed * bounce->bounceDirection;

                if (std::abs(bounce->bounceOffset) > bounce->maxBounce) {
                    bounce->bounceDirection *= -1;
                }

                position->position.y += bounce->bounceSpeed * bounce->bounceDirection;
            }
        }
    }
};