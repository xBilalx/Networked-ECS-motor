#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/ClientNetworkSystem.hpp"
#include "../common/ecs/systems/Network/Serializer.hpp"
#include "../common/ecs/systems/Transform/MovementSystem.hpp"
#include "../common/ecs/scene/sceneManager.hpp"

#include <bits/stdc++.h>


// Faire une scene de 10secondes local
// Puis faire une scene qui communique avec le serveur
int main() {
    sceneManager SceneManager(false);
    SceneManager.addScene("test", [](Scene& scene) {
        // Mettre la futur logique pour savoir si c'est une scene local ou distance à synchro
    });
    SceneManager.addScene("bulle", [](Scene& scene) {
        // Mettre la futur logique pour savoir si c'est une scene local ou distance à synchro
    });
    SceneManager.runScene("niveau 1");
}

// int main(void)
// {
//     scene scene;
//     ClientNetworkSystem clientNetworkSystem("127.0.0.1", 8089, 0.0083);


//     sceneManager
//     bool chechk = false;

//     RenderSystem renderSystem;
//     InputSystem inputSystem;
//     MovementSystem movementSystem;

//     renderSystem.createWindow(1920, 1080, "Client");
//     sf::RenderWindow& win = renderSystem.getWindow();
//     sf::Clock clock;

//     while(win.isOpen()) {
//         float dt = clock.restart().asSeconds();
//         if (!chechk) {
//             clientNetworkSystem.test();
//             chechk = true;
//         }
//         clientNetworkSystem.dataToServer(scene, inputSystem, dt);
//         clientNetworkSystem.dataFromServer(scene);
//         win.clear();
//         renderSystem.update(scene);
//         win.display();
//         inputSystem.update(scene, win);
//         movementSystem.update(scene);
//     }
// }