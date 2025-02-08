#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"

#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

std::function<void (Scene&)> onClickPlayButton = [](Scene& em) {
    em.SceneManager->isNewScene = true;
    em.SceneManager->setCurrentScene("GAME");
};

int main() {
    sceneManager SceneManager(false, false, true);


    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    SceneManager.addScene("MENU", [&font](Scene& scene) {
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems = {
            {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, onClickPlayButton},
            {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}},
            {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}}
        };

        MenuModel menu(scene, "../assets/menu_background.png", menuItems, font);
    });

    SceneManager.addScene("GAME", [](Scene& scene) {
        float cellSize = 80.0f;
        GridModel grid(scene, "../assets/menu_background.png", 6, 7, cellSize);

        std::size_t player1 = scene.createEntity();

        ArrowModel arrow(scene, "../assets/blue_arrow.png", cellSize, player1);
        ActionKeyBind& actionKeyBind = scene.addComponent<ActionKeyBind>(arrow.getEntity()); // Configure les touches 
        actionKeyBind.left = sf::Keyboard::Left;
        actionKeyBind.right= sf::Keyboard::Right;

        std::size_t player2 = scene.createEntity();
        ArrowModel arrow1(scene, "../assets/yellow_arrow.png", cellSize, player2);
        ActionKeyBind& actionKeyBind1 = scene.addComponent<ActionKeyBind>(arrow1.getEntity()); // Configure les touches 
        actionKeyBind1.left = sf::Keyboard::Q;
        actionKeyBind1.right= sf::Keyboard::D;

        scene.addComponent<GameStateComponent>(0, player1, "../assets/blue_bubble.png", player2, "../assets/yellow_bubble.png");

        scene.addSystem<ArrowMovementSystem>();
        scene.addSystem<BounceSystem>();
        scene.addSystem<TokenPlacementSystem>();
    });


    SceneManager.setCurrentScene("MENU");
    SceneManager.run();
}