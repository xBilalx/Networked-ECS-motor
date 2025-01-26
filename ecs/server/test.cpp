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



int main() {
    sceneManager SceneManager(true, true);
    SceneManager.addScene("test", [](Scene& scene) {
        scene.isServerScene = true;

        std::size_t player = scene.createEntity(); // Client Local
        scene.addComponent<InputComponent>(player);
        scene.addComponent<PositionComponent>(player, 10, 19);
        scene.addComponent<RenderComponent>(player, "../../game/common/bubble.png", true);

        std::size_t player1 = scene.createEntity(); // Client 1
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 100, 500);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
        scene.addComponent<BindClientComponent>(player1);

        std::size_t changeScene = scene.createEntity();
        scene.addComponent<CoolDownActionComponent>(changeScene, false, 10, [](Scene& em) {
            em.SceneManager->isNewScene = true;
            em.SceneManager->setCurrentScene("scene2");
            std::cout << "Passage a la scene scene2\n";
        });
    });
    SceneManager.addScene("scene2", [](Scene& scene) {
        scene.isServerScene = true;

        std::size_t player1 = scene.createEntity(); // Client 1
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 500, 800);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubbleGum.png", true);

        player1 = scene.createEntity(); // Client Local
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 10, 19);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
        scene.addComponent<BindClientComponent>(player1);

        player1 = scene.createEntity(); // Client 1
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 100, 500);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
        scene.addComponent<BindClientComponent>(player1);

        player1 = scene.createEntity(); // Client 1
        scene.addComponent<InputComponent>(player1);
        scene.addComponent<PositionComponent>(player1, 100, 800);
        scene.addComponent<RenderComponent>(player1, "../../game/common/bubbleGum.png", true);
        scene.addComponent<BindClientComponent>(player1);
    });
    SceneManager.setCurrentScene("test");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
    SceneManager.run();
}
