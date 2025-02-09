#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModelNetwork.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/systems/Network/LobbySystem.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/components/Id/IdComponent.hpp"

#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include <bits/stdc++.h>

std::function<void (Scene&)> onClickPlayButton = [](Scene& em) {
    std::string ipAdress = "";
    std::string portString = "";
    uint16_t port = 0;
    for (auto& entity : em.entities1) {
        TextFieldComponent* textF = em.getComponent<TextFieldComponent>(entity.first);

        if (textF) {
            if (textF->defaultString == "Ip Server") {
                ipAdress = textF->dataText;
            } else if (textF->defaultString == "Port"){
                portString = textF->dataText;
            }
        }
    }
    try {
        port = static_cast<unsigned short>(std::stoi(portString));
    } catch (std::invalid_argument e) {
        std::cout << "Problème de conversion" << std::endl;
        return; // Faire la logique de la mauvaise saisie
    };
    std::cout << ipAdress << " et " << port << std::endl;
    if (!em.SceneManager->getClientNetworkSystem()) {
        std::cout << "Je crée unn Client Network Systeme!\n";
        em.SceneManager->initServerNetwork(ipAdress, port, false, 0.01667);
    }
    em.SceneManager->getClientNetworkSystem().get()->setIpPort(ipAdress, port);
    em.SceneManager->isNewScene = true;
    em.SceneManager->setCurrentScene("LOBBY");
};

std::function<void (Scene&)> onClickReadyButton = [](Scene& em) {
    em.SceneManager->getClientNetworkSystem().get()->sendReadyToServer();
    // Faire un systeme Lobby
    // em.SceneManager->isNewScene = true;
    // em.SceneManager->setCurrentScene("GAME");
};

int main() {
    sceneManager SceneManager(false, false);

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    SceneManager.addScene("MENU", [&font](Scene& scene) {
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems = {
            {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, onClickPlayButton},
            {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}},
            {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}}
        };

        MenuModelNetwork menu(scene, "power4/assets/menu_background.png", menuItems, font);
    });

    SceneManager.addScene("LOBBY", [&font](Scene& scene) {
        scene.SceneManager->getClientNetworkSystem().get()->sendConnectToServer();

        scene.isNetworked = true;
        RenderSystem& renderSystem = scene.SceneManager->getRenderSystem();
        sf::Vector2u windowSize = renderSystem.getWindow().getSize();
    
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems = {
            {"LAUNCH GAME", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, onClickReadyButton}
        };
        size_t text = scene.createEntity();
        scene.addComponent<PositionComponent>(text, 100, 100);
        scene.addComponent<TextComponent>(text, "0/2 joueurs en attente", font, 24, sf::Color(70, 70, 200));
        scene.addComponent<IdComponent>(text, "nbrOfClients");
        scene.addComponent<RenderComponent>(text, 1);
        MenuModel menu(scene, "power4/assets/menu_background.png", menuItems, font);
        scene.addSystem<LobbySystem>("GAME");

    });

    SceneManager.addScene("GAME", [](Scene& scene) {
        scene.isNetworked = true; // autorise la scene à une communication avec le serveur
        scene.SceneManager->getClientNetworkSystem().get()->setSyncEntities(true); // autorise à sync les entités avec le serveur, false de base
    });

    SceneManager.setCurrentScene("MENU");
    SceneManager.run();
}
