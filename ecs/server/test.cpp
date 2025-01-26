#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/components/Transform/PositionComponent.hpp"
#include "../common/ecs/components/Network/BindRemoteComponent.hpp"
#include "../common/ecs/components/Time/CoolDownAction.hpp"

#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/NetworkManager.hpp"
#include "../common/ecs/systems/Network/ServerNetwork.hpp"
#include "../common/ecs/systems/Transform/MovementSystem.hpp"
#include "../common/ecs/scene/sceneManager.hpp"

#include <bits/stdc++.h>


// void init_entity(scene &scene) {


//     std::size_t player = scene.createEntity(); // Client Local
//     scene.addComponent<InputComponent>(player);
//     scene.addComponent<PositionComponent>(player, 10, 19);
//     scene.addComponent<RenderComponent>(player, "../../game/common/bubble.png", true);

//     std::size_t player1 = scene.createEntity(); // Client 1
//     scene.addComponent<InputComponent>(player1);
//     scene.addComponent<PositionComponent>(player1, 100, 500);
//     scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
//     scene.addComponent<BindClientComponent>(player1);
// }

// int main() {
//     sceneManager SceneManager(true, true);
//     SceneManager.addScene("niveau 1", [](scene& scene) {
        
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
//     SceneManager.runScene("niveau 1");
// }

int main() {
    sceneManager SceneManager(true, true);
    SceneManager.addScene("test", [](Scene& scene) {
        
        std::size_t player = scene.createEntity(); // Client Local
        scene.addComponent<InputComponent>(player);
        scene.addComponent<PositionComponent>(player, 10, 19);
        scene.addComponent<RenderComponent>(player, "../../game/common/bubble.png", true);

        std::size_t player1 = scene.createEntity(); // Changement de scene Action
        scene.addComponent<CoolDownActionComponent>(player, false, 3, [](Scene& em) {
            em.SceneManager->isNewScene = true;
            em.SceneManager->setCurrentScene("niveau 1");
            std::cout << "ccaca\n";
        });

    });
    SceneManager.addScene("niveau 1", [](Scene& scene) {
        
        std::size_t player = scene.createEntity(); // Client Local
        scene.addComponent<InputComponent>(player);
        scene.addComponent<PositionComponent>(player, 10, 19);
        scene.addComponent<RenderComponent>(player, "../../game/common/bubble.png", true);

        std::size_t player1 = scene.createEntity(); // Client 1
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 100, 500);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
        scene.addComponent<BindClientComponent>(player1);
    });
    SceneManager.setCurrentScene("test");
    // SceneManager.runScene("test");
    SceneManager.run();
}


// int main()
// {
    // scene scene;
    // RenderSystem renderSystem;
    // InputSystem inputSystem;
    // MovementSystem movementSystem;
    // ServerNetworkSystem serverNetworkSystem("127.0.0.1", 8089, 0.0083);


    // init_entity(scene);

    // renderSystem.createWindow(1920, 1080, "Server Render");

    // sf::RenderWindow& win = renderSystem.getWindow();
    // // sf::Clock clock;

    // while (1) {
    //     // float dt = clock.restart().asSeconds();
    // //     serverNetworkSystem.dataToClients(scene, dt);
    // //     serverNetworkSystem.dataFromClients(scene);
    // //     win.clear();
    // //     renderSystem.update(scene);
    // //     win.display();
    // //     inputSystem.updateForServer(scene, win);
    // //     movementSystem.update(scene);
    // }
// }