#pragma once

#include "../../entitiesManager.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Arrow/ArrowComponent.hpp"
#include "../../components/Grid/GridComponent.hpp"
#include "../../components/Id/IdComponent.hpp"

// Verifie si la valeur RUN du ClientNetworkSystem est true, si elle est true passe à la Scene du constructeur
class LobbySystem : public ISystem {
public:
    LobbySystem(std::string sceneName) : sceneName(sceneName) {};
    void update(Scene& scene, float dt) {
        ClientNetworkSystem * clientNetwork = scene.SceneManager->getClientNetworkSystem().get();

        for (auto it = scene.entities1.begin(); it != scene.entities1.end(); it++) {
            IdComponent* idComponent = scene.getComponent<IdComponent>(it->first);
            TextComponent* textComponent = scene.getComponent<TextComponent>(it->first);

            if (idComponent && textComponent) {
                if (idComponent->id == "nbrOfClients") {
                    textComponent->content = std::to_string(clientNetwork->getNbrOfClient()) + "/2 clients connected";
                    textComponent->updateText();
                }
            }
        }
        if (clientNetwork) {
            if (clientNetwork->getRun()) {
                scene.SceneManager->isNewScene = true;
                scene.SceneManager->setCurrentScene(sceneName);
            }
        }
    }
    std::string sceneName;
    size_t textEntity;
};

class LobbySystemServer : public ISystem {
    public:
    LobbySystemServer(std::string sceneName) : sceneName(sceneName) {};
        void update(Scene& scene, float dt) {
            ServerNetworkSystem * clientNetwork = scene.SceneManager->getServerNetworkSystem().get();

            if (clientNetwork) {
                if (clientNetwork->run) {
                    scene.SceneManager->isNewScene = true;
                    scene.SceneManager->setCurrentScene(sceneName);
                }
            }
        }
        std::string sceneName;
        size_t textEntity;
};