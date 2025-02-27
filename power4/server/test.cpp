#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include "../../ecs/common/ecs/systems/Network/LobbySystem.hpp"

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
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
    SceneManager.addScene("LOBBY", [&font](Scene& scene) {
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
        scene.addSystem<LobbySystemServer>("GAME");
        size_t text = scene.createEntity();
        scene.addComponent<PositionComponent>(text, windowSize.x/2.5, windowSize.y / 2);
        scene.addComponent<TextComponent>(text, "Visualisation cote Serveur", font, 24, sf::Color(70, 70, 200));
        scene.addComponent<IdComponent>(text, "nbrOfClients");
        scene.addComponent<RenderComponent>(text, 1);
    });
    SceneManager.addScene("GAME", [](Scene& scene) {
        
        float cellSize = 80.0f;
        GridModel grid(scene, "", 6, 7, cellSize);

        std::size_t player1 = scene.createEntity();

        ArrowModel arrow(scene, "power4/assets/blue_arrow.png", cellSize, player1);
        scene.addComponent<BindClientComponentTest>(arrow.getEntity(), 0, true); // La fleche est géré par le client 0 et l'entité est géré par le serveur
        ActionKeyBind& actionKeyBind = scene.addComponent<ActionKeyBind>(arrow.getEntity()); // Configure les touches 
        actionKeyBind.left = sf::Keyboard::Left;
        actionKeyBind.right= sf::Keyboard::Right;

        std::size_t player2 = scene.createEntity();
        ArrowModel arrow1(scene, "power4/assets/yellow_arrow.png", cellSize, player2);
        scene.addComponent<BindClientComponentTest>(arrow1.getEntity(), 1, true); // La fleche est géré par le client 0 et l'entité est géré par le serveur
        ActionKeyBind& actionKeyBind1 = scene.addComponent<ActionKeyBind>(arrow1.getEntity()); // Configure les touches 
        actionKeyBind1.left = sf::Keyboard::Left;
        actionKeyBind1.right= sf::Keyboard::Right;

        scene.addComponent<PlayerComponent>(player1, 1);
        scene.addComponent<PlayerComponent>(player2, 2);
        scene.addComponent<GameStateComponent>(0, player1, "power4/assets/blue_bubble.png", player2, "power4/assets/yellow_bubble.png");

        scene.addSystem<ArrowMovementSystem>();
        scene.addSystem<BounceSystem>();
        scene.addSystem<TokenPlacementSystem>();
    });


    SceneManager.setCurrentScene("LOBBY");
    SceneManager.setServerNetwork("0.0.0.0", 8090, 2, 0.0083);
    SceneManager.run();
}