#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Render/WindowComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Time/CoolDownAction.hpp"
#include <SFML/Graphics.hpp>

class TimeSystem {
    public:

    void update(Scene& scene,  float dt) {
        coolDown(scene, dt);
    }
    
    private:
    void coolDown(Scene& scene,  float dt) {
        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            CoolDownActionComponent* cDAComponent = scene.getComponent<CoolDownActionComponent>(it->first);

            if (cDAComponent) {
                cDAComponent->currentTime += dt;
                if (!cDAComponent->isEnd && cDAComponent->currentTime >= cDAComponent->coolDown) {
                    cDAComponent->action(scene);
                    cDAComponent->currentTime = 0;
                    cDAComponent->isEnd = true;
                }
            }
        }
    }
};