#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/components/Transform/PositionComponent.hpp"
#include "../common/ecs/components/Network/BindRemoteComponent.hpp"

#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/NetworkManager.hpp"
#include "../common/ecs/systems/Network/ServerNetwork.hpp"
#include "../common/ecs/systems/Transform/MovementSystem.hpp"

#include <bits/stdc++.h>


void init_entity(EntityManager &entityManager) {
    std::size_t player = entityManager.createEntityTest(); // Client Local
    entityManager.addComponentTest<InputComponent>(player);
    entityManager.addComponentTest<PositionComponent>(player, 10, 19);
    entityManager.addComponentTest<RenderComponent>(player, "../../game/common/bubble.png", true);

    std::size_t player1 = entityManager.createEntityTest(); // Client 1
    entityManager.addComponentTest<InputComponent>(player1);
    entityManager.addComponentTest<PositionComponent>(player1, 100, 500);
    entityManager.addComponentTest<RenderComponent>(player1, "../../game/common/bubble.png", true);
    entityManager.addComponentTest<BindClientComponent>(player1);
}

int main()
{
    EntityManager entityManager;
    RenderSystem renderSystem;
    InputSystem inputSystem;
    MovementSystem movementSystem;
    ServerNetworkSystem serverNetworkSystem("127.0.0.1", 8089, 0.0083);

    init_entity(entityManager);

    renderSystem.createWindow(1920, 1080, "Server Render");

    sf::RenderWindow& win = renderSystem.getWindow();
    sf::Clock clock;

    while (1) {
        float dt = clock.restart().asSeconds();
        serverNetworkSystem.dataToClients(entityManager, dt);
        serverNetworkSystem.dataFromClients(entityManager);
        win.clear();
        renderSystem.update(entityManager);
        win.display();
        inputSystem.updateForServer(entityManager, win);
        movementSystem.update(entityManager);
    }
}