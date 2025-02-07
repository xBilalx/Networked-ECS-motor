#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Bounce/BounceComponent.hpp"
#include "../../components/GameState/GameStateComponent.hpp"
#include <functional>
#include "../ISystem.hpp"

std::function<void (Scene&)> onClickPlayagainButton = [](Scene& em) {
    em.SceneManager->isNewScene = true;
    em.SceneManager->setCurrentScene("MENU");
};

class GameStateSystem : public ISystem  {
public:
    void update(Scene& scene, float dt) {
        std::cout << "VIVE LA FRANCE\n";
        for (auto& entity : scene.entities1) {
            GameStateComponent* gamestate = scene.getComponent<GameStateComponent>(entity.first);
            if (gamestate) {
                if (gamestate->gameOver == true) {
                    scene.SceneManager->addScene ("EndScene", [](Scene& scene) {
                        
                        sf::Font font;
                        font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

                        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems = {
                            {"Play Again ?", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, onClickPlayagainButton},
                        };
                        MenuModel endscene(scene, "", menuItems, font);
                
                    });
                }
            }
        }
    }
};