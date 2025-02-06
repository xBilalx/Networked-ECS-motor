#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Circle/CircleComponent.hpp"
#include "../components/Collision/CollisionComponent.hpp"


#include "../entitiesManager.hpp"

class BallModel {
public:
    BallModel(Scene& scene, float x, float y, float radius = 10.0f, float speed = 300.0f) {
        // Récupération de la taille de la fenêtre pour éviter d'y accéder depuis le système
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();

        ballEntity = scene.createEntity();

        // Ajout des composants
        scene.addComponent<PositionComponent>(ballEntity, x, y);
        scene.addComponent<CircleComponent>(ballEntity, x, y, radius, sf::Color::White);
        scene.addComponent<CollisionComponent>(ballEntity, radius * 2, radius * 2);
        scene.addComponent<BallComponent>(ballEntity, speed, windowSize.x, windowSize.y); // Ajout des limites de l'écran
    }

    std::size_t getEntity() const {
        return ballEntity;
    }

private:
    std::size_t ballEntity;
};