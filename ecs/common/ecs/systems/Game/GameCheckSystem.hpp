#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/GameState/GameStateComponent.hpp"
#include "../ISystem.hpp"
#include "../../scene/sceneManager.hpp"

class GameCheckSystem : public ISystem  {
    void update(Scene& scene, float dt) {
        for (auto& entity : scene.entities1) {
            GameStateComponent* gameState = scene.getComponent<GameStateComponent>(entity.first);
            if (gameState) {
                if (gameState->gameOver) {
                    bool winner = gameState->winner;
                    std::cout << winner << std::endl;
                    scene.SceneManager->addScene("END", [winner](Scene& scene) {
                        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
                        sf::Vector2u windowSize = renderSystem.getWindow().getSize();

                        if (winner) {
                            sf::Font font;
                            size_t text = scene.createEntity();
                            scene.addComponent<PositionComponent>(text, windowSize.x/3.5, windowSize.y/2);
                            scene.addComponent<TextComponent>(text, "The player YELLOW has WIN", font, 50, sf::Color(70, 70, 200));
                            scene.addComponent<RenderComponent>(text, 1);
                        } else {
                            sf::Font font;
                            size_t text = scene.createEntity();
                            scene.addComponent<PositionComponent>(text, windowSize.x/3.5, windowSize.y/2);
                            scene.addComponent<TextComponent>(text, "The player BLUE has WIN", font, 50, sf::Color(70, 70, 200));
                            scene.addComponent<RenderComponent>(text, 1);
                        }
                    });
                    scene.SceneManager->isNewScene = true;
                    scene.SceneManager->setCurrentScene("END");
                }
            }
        }
    }
};