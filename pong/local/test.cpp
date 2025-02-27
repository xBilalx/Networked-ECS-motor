#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/Model/ScoreModel.hpp"
#include "../../ecs/common/ecs/Model/BallModel.hpp"
#include "../../ecs/common/ecs/Model/PaddleModel.hpp"
#include "../../ecs/common/ecs/components/Input/InputComponent.hpp"
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
    sceneManager SceneManager(false, false, true);

    SceneManager.addScene("PongScene", [](Scene& scene) {
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();

        float paddleWidth = 20.0f;
        float paddleHeight = 100.0f;
        float ballRadius = 10.0f;

        float centerX = windowSize.x / 2.0f;
        float dashHeight = 10.0f;
        float spaceBetween = 10.0f;
        float startY = 0;

        while (startY < windowSize.y) {
            std::size_t dash = scene.createEntity();
            scene.addComponent<RectangleComponent>(dash, centerX, startY, 5, dashHeight, sf::Color::White);
            scene.addComponent<RenderComponent>(dash, 1);
            startY += dashHeight + spaceBetween;
        }

        std::size_t paddleLeft = scene.createEntity();
        PaddleModel paddle1(scene, paddleLeft, 50, windowSize.y / 2.0f, 1, paddleWidth, paddleHeight);
        ActionKeyBind& actionKeyBind1 = scene.addComponent<ActionKeyBind>(paddleLeft); // Configure les touches 
        actionKeyBind1.left = sf::Keyboard::Q;
        actionKeyBind1.right= sf::Keyboard::D;
        actionKeyBind1.forward= sf::Keyboard::Z;
        actionKeyBind1.back= sf::Keyboard::S;

        std::size_t paddlRight = scene.createEntity();
        PaddleModel paddle2(scene, paddlRight, windowSize.x - 50, windowSize.y / 2.0f, 2, paddleWidth, paddleHeight);
        ActionKeyBind& actionKeyBind2 = scene.addComponent<ActionKeyBind>(paddlRight); // Configure les touches 
        actionKeyBind2.left = sf::Keyboard::Left;
        actionKeyBind2.right= sf::Keyboard::Right;
        actionKeyBind2.forward= sf::Keyboard::Up;
        actionKeyBind2.back= sf::Keyboard::Down;

        BallModel ball(scene, windowSize.x / 2.0f, windowSize.y / 2.0f, ballRadius);
        
        static sf::Font font;
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            std::cerr << "❌ ERREUR: Impossible de charger la police !" << std::endl;
            return;
        }

        float score1X = windowSize.x / 4.0f;  // Score du joueur 1 à gauche
        float score2X = 3 * windowSize.x / 4.0f; // Score du joueur 2 à droite
        float scoreY = 20.0f; // Position verticale commune

        ScoreModel score(scene, font, score1X, scoreY, score2X, scoreY);
        // scene.addSystem<PaddleMovementSystem>();
        scene.addSystem<MovementSystem>();
        scene.addSystem<BallMovementSystem>();
    });

    SceneManager.setCurrentScene("PongScene");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 2, 0.0083);
    SceneManager.run();
}

// int main() {
//     sceneManager SceneManager(true, true);

//     SceneManager.addScene("PongScene", [](Scene& scene) {
//         // ✅ Chargement correct de la police (garde-la en mémoire)
//         static sf::Font font;  // 🔹 Rend la police accessible tout le long du programme
//         if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
//             std::cerr << "❌ ERREUR: Impossible de charger la police !" << std::endl;
//             return;
//         }

//         std::cerr << "✅ Police chargée avec succès !" << std::endl;

//         // Création du ScoreModel avec la police
//         ScoreModel score(scene, font);
//     });

//     SceneManager.setCurrentScene("PongScene");
//     SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
//     SceneManager.run();
// }