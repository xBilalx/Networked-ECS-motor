#include "../common/ecs/scene/sceneManager.hpp"
#include "../common/ecs/systems/Network/ServerNetwork.hpp"
#include "../common/ecs/systems/Render/RenderSystem.hpp"
#include "../common/ecs/components/Grid/GridComponent.hpp"
#include "../common/ecs/components/GameState/GameStateComponent.hpp"
#include "../common/ecs/components/Player/PlayerComponent.hpp"
#include "../power4/logic/Power4LogicSystem.hpp"

int main() {
    sceneManager SceneManager(true, true); // Mode serveur activé

    SceneManager.addScene("Power4", [](Scene& scene) {
        scene.isServerScene = true;

        // Ajouter la grille de jeu
        std::size_t gridEntity = scene.createEntity();
        scene.addComponent<GridComponent>(gridEntity, 6, 7); // Grille 6x7

        // Ajouter l'état du jeu
        std::size_t gameStateEntity = scene.createEntity();
        scene.addComponent<GameStateComponent>(gameStateEntity);

        // Ajouter les joueurs (liaison réseau avec `BindRemoteComponent`)
        std::size_t player1 = scene.createEntity();
        scene.addComponent<PlayerComponent>(player1, 1, "Player 1");
        scene.addComponent<BindClientComponent>(player1);

        std::size_t player2 = scene.createEntity();
        scene.addComponent<PlayerComponent>(player2, 2, "Player 2");
        scene.addComponent<BindClientComponent>(player2);

        // Ajouter un système de mise à jour logicielle pour Puissance 4
        scene.addComponent<Power4LogicSystem>(gridEntity);
    });

    SceneManager.setCurrentScene("Power4");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
    SceneManager.run();

    return 0;
}
