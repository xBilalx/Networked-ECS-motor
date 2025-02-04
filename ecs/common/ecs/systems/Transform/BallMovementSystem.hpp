#pragma once

#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Paddle/PaddleComponent.hpp"
#include "../../scene/sceneManager.hpp"
#include "../../entitiesManager.hpp"
#include <SFML/Window/Keyboard.hpp>
#include "../../systems/Render/RenderSystem.hpp" 
#include "../../systems/Render/RenderSystem.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/PlayerInput/PlayerInputComponent.hpp"
#include "../../components/Ball/BallComponent.hpp"
#include "../../components/Collision/CollisionComponent.hpp"
#include <complex>



class BallMovementSystem {
public:
    // dt représente le delta time en secondes
    void update(Scene& scene, float dt) {
        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            BallComponent* ball = scene.getComponent<BallComponent>(entity.first);
            CollisionComponent* collision = scene.getComponent<CollisionComponent>(entity.first);
            if (!position || !ball || !collision)
                continue;
            
            // Gestion du respawn de la balle (après qu'un point soit marqué)
            if (ball->isRespawning) {
                ball->respawnTime -= dt;
                if (ball->respawnTime <= 0) {
                    respawnBall(position, ball, collision);
                }
                continue;
            }
            
            // Mise à jour de la position de la balle
            position->position += ball->velocity * dt;
            
            // --- Rebonds sur le haut et le bas de l'écran ---
            if (position->position.y <= 0) {
                position->position.y = 0;
                ball->velocity.y = std::abs(ball->velocity.y);
            }
            if (position->position.y + collision->height >= ball->screenHeight) {
                position->position.y = ball->screenHeight - collision->height;
                ball->velocity.y = -std::abs(ball->velocity.y);
            }
            
            // --- Collision avec les paddles ---
            // On parcourt toutes les entités possédant un PaddleComponent
            for (auto& other : scene.entities1) {
                PaddleComponent* paddle = scene.getComponent<PaddleComponent>(other.first);
                PositionComponent* paddlePos = scene.getComponent<PositionComponent>(other.first);
                CollisionComponent* paddleCol = scene.getComponent<CollisionComponent>(other.first);
                if (paddle && paddlePos && paddleCol) {
                    // Vérification directionnelle :
                    // • Pour le paddle de gauche (playerId == 1), la balle doit se déplacer vers la gauche (velocity.x < 0)
                    // • Pour le paddle de droite (playerId == 2), la balle doit se déplacer vers la droite (velocity.x > 0)
                    if (paddle->playerId == 1 && ball->velocity.x >= 0)
                        continue;
                    if (paddle->playerId == 2 && ball->velocity.x <= 0)
                        continue;
                    
                    if (checkCollision(position, collision, paddlePos, paddleCol)) {
                        handlePaddleCollision(position, collision, ball, paddle, paddlePos, paddleCol);
                        break;  // Une collision par update suffit
                    }
                }
            }
            
            // --- Limites horizontales ---
            // On utilise le centre de la balle pour déterminer si elle quitte l'aire de jeu.
            float ballCenterX = position->position.x + collision->width * 0.5f;
            if (ballCenterX < 0 || ballCenterX > ball->screenWidth) {
                ball->isRespawning = true;
                ball->respawnTime = 3.0f;
            }
        }
    }
    
private:
    // Repositionne la balle au centre et réinitialise sa direction et sa vitesse
    void respawnBall(PositionComponent* position, BallComponent* ball, CollisionComponent* collision) {
        // Reset de la vitesse à sa valeur initiale
        ball->speed = ball->initialSpeed;
        
        // Recentrer la balle
        position->position.x = ball->screenWidth / 2.0f - collision->width / 2.0f;
        position->position.y = ball->screenHeight / 2.0f - collision->height / 2.0f;
        
        // Angle aléatoire entre -45° et 45°
        float angleDegrees = (std::rand() % 91) - 45;
        float angle = angleDegrees * (3.14159f / 180.0f);
        // Choix aléatoire de la direction horizontale
        float direction = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
        ball->velocity.x = direction * ball->speed * std::cos(angle);
        ball->velocity.y = ball->speed * std::sin(angle);
        ball->isRespawning = false;
    }
    
    // Vérification de collision AABB entre la balle et un paddle
    bool checkCollision(PositionComponent* ballPos, CollisionComponent* ballCol,
                        PositionComponent* paddlePos, CollisionComponent* paddleCol) {
        bool collisionX = (ballPos->position.x + ballCol->width >= paddlePos->position.x) &&
                          (paddlePos->position.x + paddleCol->width >= ballPos->position.x);
        bool collisionY = (ballPos->position.y + ballCol->height >= paddlePos->position.y) &&
                          (paddlePos->position.y + paddleCol->height >= ballPos->position.y);
        if (collisionX && collisionY) {
            std::cerr << "💥 Collision détectée entre la balle et un paddle.\n";
        }
        return collisionX && collisionY;
    }
    
    // Gère le rebond de la balle sur un paddle en fonction du point d'impact
    void handlePaddleCollision(PositionComponent* ballPos, CollisionComponent* ballCol,
                               BallComponent* ball, PaddleComponent* paddle,
                               PositionComponent* paddlePos, CollisionComponent* paddleCol) {
        // Repositionnement de la balle pour la sortir du volume du paddle
        if (paddle->playerId == 1) {
            // Pour le paddle de gauche, on positionne la balle juste à sa droite
            ballPos->position.x = paddlePos->position.x + paddleCol->width;
        } else {
            // Pour le paddle de droite, on positionne la balle juste à sa gauche
            ballPos->position.x = paddlePos->position.x - ballCol->width;
        }
        
        // Calcul de l'impact vertical sur le paddle
        float paddleCenterY = paddlePos->position.y + paddleCol->height / 2.0f;
        float ballCenterY   = ballPos->position.y + ballCol->height / 2.0f;
        float relativeIntersectY = ballCenterY - paddleCenterY;
        float normalizedRelativeIntersectionY = relativeIntersectY / (paddleCol->height / 2.0f);
        normalizedRelativeIntersectionY = std::clamp(normalizedRelativeIntersectionY, -1.0f, 1.0f);
        
        // Angle de rebond maximal (ici 75°)
        float maxBounceAngle = 75 * (3.14159f / 180.0f);
        float bounceAngle = normalizedRelativeIntersectionY * maxBounceAngle;
        
        // Pour le paddle de gauche, la balle repart toujours vers la droite
        if (paddle->playerId == 1) {
            ball->velocity.x = std::abs(ball->speed * std::cos(bounceAngle));
        } else {
            // Pour le paddle de droite, on applique un multiplicateur pour renforcer la composante X
            const float rightPaddleMultiplier = 1.2f;
            ball->velocity.x = -std::abs(ball->speed * std::cos(bounceAngle)) * rightPaddleMultiplier;
        }
        ball->velocity.y = ball->speed * std::sin(bounceAngle);
        
        // Augmentation progressive de la vitesse pour relever le défi
        ball->speed *= 1.05f;
    }
};