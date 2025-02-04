#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include "../entitiesManager.hpp"
#include "../systems/Render/RenderSystem.hpp"
#include "../systems/Input/InputSystem.hpp"
#include "../systems/Transform/MovementSystem.hpp"
#include "../systems/Network/ServerNetwork.hpp"
#include "../systems/Network/ClientNetworkSystem.hpp"
#include "../systems/Time/TimeSystem.hpp"
#include <functional>

class sceneManager {
public:
    sceneManager(bool isServer, bool debug = false, bool isLocalClient = true) 
        : isServer(isServer), debug(debug), isLocalClient(isLocalClient), isNewScene(false) {
        
        // Initialiser la fenêtre dès le début
        initializeWindow();
    }

    void addScene(std::string sceneName, std::function<void(Scene&)> initSceneLambda) {
        initScenes[sceneName] = initSceneLambda;
        auto scene = std::make_shared<Scene>(this);
        scenes[sceneName] = scene;
        initSceneLambda(*scene);
    }

        void setServerNetwork(std::string ip_, unsigned short int port_, int nbrClients_ ,float tickRate_) {
            ip = ip_;
            port = port_;
            nbrClient = nbrClients_;
            tickRate = tickRate_;
        }

    void setCurrentScene(std::string scene) {
        currentScene = scene;
    }

    void run() {
        if (isServer) {
            serverNetworkSystem = std::make_unique<ServerNetworkSystem>(ip, port, nbrClient, tickRate);
        }

        int check = 0;
        while (1) {
            isNewScene = false;
            check = runScene(currentScene);
            if (!check) {
                std::cout << "Erreur de Scene\n";
                return;
            }
        }
    }

    RenderSystem& getRenderSystem() {
        return renderSystem;
    }

    bool isNewScene; // A mettre en privée


private:
    void initializeWindow() {
        if (isLocalClient || !isServer) {
            renderSystem.createWindow(1920, 1080, "ECS Game Window");
            std::cout << "Fenêtre créée avec succès: 1920x1080" << std::endl;
        }
    }

    bool runScene(std::string sceneName) {
        auto it = scenes.find(sceneName);
        if (it == scenes.end()) {
            std::cout << "La scène " << sceneName << " n'existe pas\n";
            return false;
        }

        return isServer ? runSceneServer(*it->second) : runSceneClient(*it->second);
    }

    bool runSceneServer(Scene &em) {
        InputSystem inputSystem;
        TimeSystem timeSystem;
        MovementSystem movementSystem;
        sf::Clock clock;
        bool isServerScene = em.isServerScene;
        sf::RenderWindow& win = renderSystem.getWindow();

        // il faudra les ajouter les systemes de facon générique, parce qu'on à pas besoin de ces systemes souvent par exemple !
        BounceSystem bounceSystem;
        ArrowMovementSystem arrowMovementSystem;
        TokenPlacementSystem tokenPlacementSystem;

        while (1) {

                float dt = clock.restart().asSeconds();
                if (debug) {
                    std::cout << "Time for loop -> " << dt << "s\n"; 
                }
                if (isServerScene) {
                    serverNetworkSystem->dataFromClients(em);
                }            
                timeSystem.update(em, dt);
                if (isLocalClient) {
                    inputSystem.updateForServer(em, win);
                }
                movementSystem.update(em);
                arrowMovementSystem.update(em);
                tokenPlacementSystem.update(em);

                if (isServerScene) {
                    serverNetworkSystem->dataToClients(em, dt);
                }
                renderSystem.update(em);
                
                if (isNewScene) {
                    serverNetworkSystem->sendClearScene(em, dt);
                    return true;
                }
            }
        }


    bool runSceneClient(Scene &em) {
        InputSystem inputSystem;
        TimeSystem timeSystem;
        MovementSystem movementSystem;
        ClientNetworkSystem clientNetworkSystem(em.serverAdress, em.port, em.tickRate);
        sf::Clock clock;
        sf::RenderWindow& win = renderSystem.getWindow();
        bool chechk = false;
        bool isNetworked = em.isNetworked;

        while (win.isOpen()) {
            float dt = clock.restart().asSeconds();
                if (debug) {
                    std::cout << "Time for loop -> " << dt << "s\n"; 
                }
                if (isNetworked) {
                    if (!chechk) {
                        clientNetworkSystem.test(); // Envoie un paquet CONNECT mais a upgrade
                        chechk = true;
                    }
                }
            timeSystem.update(em, dt);
                if (isNetworked) {
                    clientNetworkSystem.dataToServer(em, inputSystem, dt);
                    clientNetworkSystem.dataFromServer(em);
                }
            renderSystem.update(em);
            inputSystem.update(em, win);
            movementSystem.update(em);

            if (isNewScene) {
                return true;
            }
        }
        return false;
    }


        std::unordered_map<std::string, std::function<void (Scene&)>> initScenes;
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
        std::string currentScene;
        RenderSystem renderSystem;
        std::unique_ptr<ServerNetworkSystem> serverNetworkSystem;

        std::string ip = "127.0.0.1";
        std::uint16_t port = 8089;
        int nbrClient = 0;
        float tickRate = 0.01667;
        bool isServer; // applique les sytemes
        bool debug;  // debug ⚠️ Pas de logique encore implémenté
        bool isLocalClient; // Local Client Only for servers  si il y a un client local ⚠️ Pas de logique encore implémenté
};
