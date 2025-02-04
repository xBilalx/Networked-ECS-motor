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
    PaddleModel(Scene& scene, float x, float y, int playerId, float width = 20.0f, float height = 100.0f) {
        // Récupération de la taille de la fenêtre
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
        float screenMiddle = windowSize.x / 2.0f;

        // Pour l'axe X, le paddle reste dans sa moitié :
        // - Joueur 1 : de 0 à (screenMiddle - width)
        // - Joueur 2 : de screenMiddle à (windowSize.x - width)
        float minX = (playerId == 1) ? 0.0f : screenMiddle;
        float maxX = (playerId == 1) ? screenMiddle - width : windowSize.x - width;

        // Pour l'axe Y, le paddle est totalement visible :
        // La position Y représente le haut du paddle, il peut donc descendre jusqu'à (windowSize.y - height)
        float minY = 0.0f;
        float maxY = windowSize.y - height;

        // Clamp des positions initiales pour éviter toute incohérence
        x = std::clamp(x, minX, maxX);
        y = std::clamp(y, minY, maxY);

        // Création de l'entité paddle et ajout des composants
        paddleEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(paddleEntity, x, y);
        scene.addComponent<RectangleComponent>(paddleEntity, x, y, width, height, sf::Color::White);
        scene.addComponent<PlayerInputComponent>(paddleEntity, playerId);
        scene.addComponent<CollisionComponent>(paddleEntity, width, height);
        scene.addComponent<PaddleComponent>(paddleEntity, playerId, 6.0f, width, height, minX, maxX, minY, maxY);
    }

    std::size_t getEntity() const {
        return paddleEntity;
    }

private:
    std::size_t paddleEntity;
};