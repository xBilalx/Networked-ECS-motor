// #include "../common/ecs/components/Render/WindowComponent.hpp"
// #include "../common/ecs/components/Input/InputComponent.hpp"
// #include "../common/ecs/components/Transform/PositionComponent.hpp"
// #include "../common/ecs/components/Network/BindRemoteComponent.hpp"
// #include "../common/ecs/components/Time/CoolDownAction.hpp"

// #include "../common/ecs/entitiesManager.hpp"
// #include "../common/ecs/systems/Render/RenderSystem.hpp"
// #include "../common/ecs/systems/Input/InputSystem.hpp"
// #include "../common/ecs/systems/Network/NetworkManager.hpp"
// #include "../common/ecs/systems/Network/ServerNetwork.hpp"
// #include "../common/ecs/systems/Transform/MovementSystem.hpp"
// #include "../common/ecs/scene/sceneManager.hpp"

// #include <bits/stdc++.h>



// int main() {
//     sceneManager SceneManager(true, true);
//     SceneManager.addScene("test", [](Scene& scene) {
//         scene.isServerScene = true;

//         std::size_t player = scene.createEntity(); // Client Local
//         scene.addComponent<InputComponent>(player);
//         scene.addComponent<PositionComponent>(player, 10, 19);
//         scene.addComponent<RenderComponent>(player, "../../game/common/bubble.png", true);

//         std::size_t player1 = scene.createEntity(); // Client 1
//         scene.addComponent<InputComponent>(player1);
//         scene.addComponent<PositionComponent>(player1, 100, 500);
//         scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
//         scene.addComponent<BindClientComponent>(player1);
//     });
//     SceneManager.setCurrentScene("test");
//     SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
//     SceneManager.run();
// }
#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include <SFML/Graphics.hpp>

#include <bits/stdc++.h>


// int main() {
//     sceneManager SceneManager(true, true);
    
//     sf::Font font;
//     font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf"); // Assure-toi que le fichier existe

//     SceneManager.addScene("ButtonTest", [&font](Scene& scene) {
//         ButtonModel button(scene, 200, 300, 150, 50, sf::Color::Red, "Play", font, 24, sf::Color::White);
//     });

//     SceneManager.setCurrentScene("ButtonTest");
//     SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
//     SceneManager.run();
// }
int main() {
    sceneManager SceneManager(true, true);
    
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    SceneManager.addScene("MainMenu", [&font](Scene& scene) {
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color>> menuItems = {
            {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
            {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
            {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White}
        };

        MenuModel menu(scene, "../assets/menu_background.png", menuItems, font);
    });

    SceneManager.setCurrentScene("MainMenu");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
    SceneManager.run();
}