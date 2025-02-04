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
#include "../../components/Score/ScoreComponent.hpp"
#include "../../components/Score/ScoreComponent.hpp"
#include "../../components/Text/TextComponent.hpp" 
#include <complex>


class BallMovementSystem {
public:
    void update(Scene& scene, float dt) {
        for (auto& entity : scene.entities1) {
            PositionComponent* position = scene.getComponent<PositionComponent>(entity.first);
            BallComponent* ball = scene.getComponent<BallComponent>(entity.first);
            CollisionComponent* collision = scene.getComponent<CollisionComponent>(entity.first);
            if (!position || !ball || !collision)
                continue;
            
            if (ball->isRespawning) {
                ball->respawnTime -= dt;
                if (ball->respawnTime <= 0) {
                    respawnBall(position, ball, collision);
                }
                continue;
            }
            
            position->position += ball->velocity * dt;
            
            if (position->position.y <= 0) {
                position->position.y = 0;
                ball->velocity.y = std::abs(ball->velocity.y);
            }
            if (position->position.y + collision->height >= ball->screenHeight) {
                position->position.y = ball->screenHeight - collision->height;
                ball->velocity.y = -std::abs(ball->velocity.y);
            }
            
            for (auto& other : scene.entities1) {
                PaddleComponent* paddle = scene.getComponent<PaddleComponent>(other.first);
                PositionComponent* paddlePos = scene.getComponent<PositionComponent>(other.first);
                CollisionComponent* paddleCol = scene.getComponent<CollisionComponent>(other.first);
                if (paddle && paddlePos && paddleCol) {
                    if (paddle->playerId == 1 && ball->velocity.x >= 0)
                        continue;
                    if (paddle->playerId == 2 && ball->velocity.x <= 0)
                        continue;
                    
                    if (checkCollision(position, collision, paddlePos, paddleCol)) {
                        handlePaddleCollision(position, collision, ball, paddle, paddlePos, paddleCol);
                        break;
                    }
                }
            }
            
            float ballCenterX = position->position.x + collision->width * 0.5f;
            if (ballCenterX < 0) {
                updateScore(scene, 2);
                ball->isRespawning = true;
                ball->respawnTime = 3.0f;
            }
            else if (ballCenterX > ball->screenWidth) {
                updateScore(scene, 1);
                ball->isRespawning = true;
                ball->respawnTime = 3.0f;
            }
        }
    }
    
private:
    void respawnBall(PositionComponent* position, BallComponent* ball, CollisionComponent* collision) {
        ball->speed = ball->initialSpeed;
        position->position.x = ball->screenWidth / 2.0f - collision->width / 2.0f;
        position->position.y = ball->screenHeight / 2.0f - collision->height / 2.0f;
        
        float angleDegrees = (std::rand() % 91) - 45;
        float angle = angleDegrees * (3.14159f / 180.0f);
        float direction = (std::rand() % 2 == 0) ? 1.0f : -1.0f;
        ball->velocity.x = direction * ball->speed * std::cos(angle);
        ball->velocity.y = ball->speed * std::sin(angle);
        ball->isRespawning = false;
    }
    
    bool checkCollision(PositionComponent* ballPos, CollisionComponent* ballCol,
                        PositionComponent* paddlePos, CollisionComponent* paddleCol) {
        bool collisionX = (ballPos->position.x + ballCol->width >= paddlePos->position.x) &&
                          (paddlePos->position.x + paddleCol->width >= ballPos->position.x);
        bool collisionY = (ballPos->position.y + ballCol->height >= paddlePos->position.y) &&
                          (paddlePos->position.y + paddleCol->height >= ballPos->position.y);
        return collisionX && collisionY;
    }
    
    void handlePaddleCollision(PositionComponent* ballPos, CollisionComponent* ballCol,
                               BallComponent* ball, PaddleComponent* paddle,
                               PositionComponent* paddlePos, CollisionComponent* paddleCol) {
        if (paddle->playerId == 1) {
            ballPos->position.x = paddlePos->position.x + paddleCol->width;
        } else {
            ballPos->position.x = paddlePos->position.x - ballCol->width;
        }
        
        float paddleCenterY = paddlePos->position.y + paddleCol->height / 2.0f;
        float ballCenterY   = ballPos->position.y + ballCol->height / 2.0f;
        float relativeIntersectY = ballCenterY - paddleCenterY;
        float normalizedRelativeIntersectionY = relativeIntersectY / (paddleCol->height / 2.0f);
        normalizedRelativeIntersectionY = std::clamp(normalizedRelativeIntersectionY, -1.0f, 1.0f);
        
        float maxBounceAngle = 75 * (3.14159f / 180.0f);
        float bounceAngle = normalizedRelativeIntersectionY * maxBounceAngle;
        
        if (paddle->playerId == 1) {
            ball->velocity.x = std::abs(ball->speed * std::cos(bounceAngle));
        } else {
            const float rightPaddleMultiplier = 1.2f;
            ball->velocity.x = -std::abs(ball->speed * std::cos(bounceAngle)) * rightPaddleMultiplier;
        }
        ball->velocity.y = ball->speed * std::sin(bounceAngle);
        ball->speed *= 1.05f;
    }
    
    void updateScore(Scene& scene, int scoringPlayerId) {
        for (auto& entity : scene.entities1) {
            ScoreComponent* scoreComp = scene.getComponent<ScoreComponent>(entity.first);
            if (scoreComp && scoreComp->playerId == scoringPlayerId) {
                scoreComp->increaseScore();
                TextComponent* textComp = scene.getComponent<TextComponent>(entity.first);
                if (textComp) {
                    textComp->text.setString(std::to_string(scoreComp->score));
                }
                if (scoreComp->score >= 5) {
                    endGame(scene, scoringPlayerId);
                }
            }
        }
    }
    
    void endGame(Scene& scene, int winningPlayerId) {
        std::string winMessage;
        if (winningPlayerId == 1)
            winMessage = "Le joueur gauche a gagne!";
        else
            winMessage = "Le joueur droit a gagne!";
        std::size_t winEntity = scene.createEntity();

        // Récupération des dimensions de l'écran depuis un BallComponent existant dans le modèle
        float winX = 0.0f, winY = 0.0f;
        for (auto& entity : scene.entities1) {
            BallComponent* ball = scene.getComponent<BallComponent>(entity.first);
            if (ball) {
                winX = ball->screenWidth / 2.0f;
                winY = ball->screenHeight / 2.0f;
                break;
            }
        }
        static sf::Font victoryFont;
        if (victoryFont.getInfo().family.empty()) {
            if (!victoryFont.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
                std::cerr << "❌ ERREUR: Impossible de charger la police de victoire!" << std::endl;
            }
        }
        scene.addComponent<TextComponent>(winEntity, winMessage, victoryFont, 40, sf::Color::Yellow);
        scene.addComponent<PositionComponent>(winEntity, winX, winY);
        
        // Arrêt du jeu (ici, on suppose que SceneManager possède une méthode stop())
        // scene.SceneManager->stop();
        return;
    }
};