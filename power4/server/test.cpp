#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

// int main() {
//     sceneManager SceneManager(true, true);
    
//     sf::Font font;
//     font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

//     SceneManager.addScene("MainMenu", [&font](Scene& scene) {
//         std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color>> menuItems = {
//             {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
//             {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
//             {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White}
//         };

//         MenuModel menu(scene, "../assets/menu_background.png", menuItems, font);
//     });

//     SceneManager.setCurrentScene("MainMenu");
//     SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
//     SceneManager.run();
// }

int main() {
    sceneManager SceneManager(true, false);

    SceneManager.addScene("Power4Scene", [](Scene& scene) {
        scene.isServerScene = true;
        float cellSize = 80.0f;
        GridModel grid(scene, "../assets/menu_background.png", 6, 7, cellSize);
        ArrowModel arrow(scene, "../assets/blue_arrow.png", cellSize);
        std::size_t player1 = scene.createEntity();
        std::size_t player2 = scene.createEntity();
        scene.addComponent<PlayerComponent>(player1, 1);
        scene.addComponent<PlayerComponent>(player2, 2);
        scene.addComponent<GameStateComponent>(0, player1, player2);
    });

    SceneManager.setCurrentScene("Power4Scene");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 1, 0.0083);
    SceneManager.run();
}