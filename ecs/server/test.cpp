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
    // Regler le problème si on Bind une idClient trop haute
    sceneManager SceneManager(true, false, false);

    SceneManager.addScene("test", [](Scene& scene) {
        scene.isServerScene = true;
        std::size_t player1 = 0;
        for (int i = 0; i < 3; i++) {
            player1 = scene.createEntity(); // Client Local
            scene.addComponent<InputComponent>(player1);
            scene.addComponent<PositionComponent>(player1, 10, 19);
            scene.addComponent<RenderComponent>(player1, "../../game/common/bubble.png", true);
            scene.addComponent<BindClientComponentTest>(player1, 2);
        }
    });

    SceneManager.setCurrentScene("test");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 1, 0.0083);
    SceneManager.run();
}
