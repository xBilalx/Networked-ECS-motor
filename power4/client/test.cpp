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


int main() {
    sceneManager SceneManager(false);
    SceneManager.addScene("sync_test", [](Scene& scene) {
        scene.isNetworked = true;
        scene.serverAdress = "127.0.0.1";
        scene.port = 8090;
    });
    SceneManager.setCurrentScene("sync_test");
    SceneManager.run();
}
