#include "../common/ecs/components/Render/WindowComponent.hpp"
#include "../common/ecs/components/Input/InputComponent.hpp"
#include "../common/ecs/entitiesManager.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/systems/Input/InputSystem.hpp"
#include "../common/ecs/systems/Network/ClientNetworkSystem.hpp"
#include "../common/ecs/systems/Network/Serializer.hpp"
#include "../common/ecs/systems/Transform/MovementSystem.hpp"

#include <bits/stdc++.h>


int main(void)
{
    EntityManager entityManager;
    ClientNetworkSystem clientNetworkSystem("127.0.0.1", 8089, 0.0083);

    bool chechk = false;

    RenderSystem renderSystem;
    InputSystem inputSystem;
    MovementSystem movementSystem;

    renderSystem.createWindow(1920, 1080, "Client");
    sf::RenderWindow& win = renderSystem.getWindow();
    sf::Clock clock;

    while(win.isOpen()) {
        float dt = clock.restart().asSeconds();
        if (!chechk) {
            clientNetworkSystem.test();
            chechk = true;
        }
        clientNetworkSystem.dataToServer(entityManager, inputSystem, dt);
        clientNetworkSystem.dataFromServer(entityManager);
        win.clear();
        renderSystem.update(entityManager);
        win.display();
        inputSystem.update(entityManager, win);
        movementSystem.update(entityManager);
    }
}