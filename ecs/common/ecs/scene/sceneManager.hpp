#pragma once
#include <vector>
#include <memory>
#include <algorithm>
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

// sceneManager(bool isServer, bool debug=false, bool isLocalClient=true)
class sceneManager {
    public:
        sceneManager(bool isServer, bool debug=false, bool isLocalClient=true) : isServer(isServer), debug(debug), isLocalClient(isLocalClient), isNewScene(false) {}

        // Load and Store Scene
        void addScene(std::string sceneName, std::function<void (Scene&)> initSceneLamda) {
            // save lambda init scene
            initScenes[sceneName] = initSceneLamda;

            // save entity Manager init scene
            auto scene = std::make_shared<Scene>(this);
            scenes[sceneName] = scene;
            initSceneLamda(*scene);
        }

        void setServerNetwork(std::string ip_, unsigned short int port_, float tickRate_) {
            ip = ip_;
            port = port_;
            tickRate = tickRate_;
        }

        void setCurrentScene(std::string scene) {
            currentScene = scene;
        }

        void run() {
            
            if (isLocalClient || !isServer) {
                renderSystem.createWindow(1920, 1080, "Server Render");
            }
            if (isServer) {
                serverNetworkSystem = std::make_unique<ServerNetworkSystem>(ip, port, tickRate);
            }

            int check = 0;
            while(1) {
                isNewScene = false;
                check = runScene(currentScene);
                if (!check) {
                    std::cout << "Erreur de Scene\n";
                    return;
                }
            }
        }

        // Run scene
        // Return true -> Change scene
        // Return false -> Error Scene
        bool runScene(std::string sceneName) {
            auto it = scenes.find(sceneName);
            if (it == scenes.end()) {
                std::cout << "IL existe pas\n";
                return false;
            }
        
            if (isServer) {
                return runSceneServer(*it->second);
            }
            if (!isServer) {
                return runSceneClient(*it->second);
            }
            return false;
        }


        void reloadScene(std::string sceneName) {
            auto it = scenes.find(sceneName);
            if (it == scenes.end()) {
                std::cout << "IL existe pas\n";
                return;
            }
            // lambda
            auto it1 = initScenes.find(sceneName);
            if (it1 == initScenes.end()) {
                std::cout << "IL existe pas\n";
                return;
            }
            it->second->clear();
            it1->second(*it->second);
        }

        bool isNewScene; // A mettre en privée


    private:

        bool runSceneServer(Scene &em) {
            
            InputSystem inputSystem;
            TimeSystem timeSystem;
            bool isServerScene = em.isServerScene;
            MovementSystem movementSystem;

            sf::Clock clock;
            sf::RenderWindow& win = renderSystem.getWindow();

            // peut être gérer les système dans le systeme manager pour que le dev puisse mieux config ??
            while(1) {
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
                if (isServerScene) {
                    serverNetworkSystem->dataToClients(em, dt);
                }
                renderSystem.update(em);
                
                if (isNewScene) {
                    serverNetworkSystem->sendClearScene(em, dt);
                    return true;
                }
                sf::sleep(sf::milliseconds(16));
            }
        }

        // Clients Scenes
        bool runSceneClient(Scene &em) {
            
            InputSystem inputSystem;
            TimeSystem timeSystem;
            bool chechk = false;
            bool isNetworked = em.isNetworked;

            MovementSystem movementSystem;
            ClientNetworkSystem clientNetworkSystem(em.serverAdress, em.port, em.tickRate);

            sf::Clock clock;
            sf::RenderWindow& win = renderSystem.getWindow();

            while(win.isOpen()) {
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
                win.clear();
                renderSystem.update(em);
                win.display();
                inputSystem.update(em, win);
                movementSystem.update(em);
                if (isNewScene) {
                    return true;
                }
                sf::sleep(sf::milliseconds(16));
            }
            return false;
        }

        std::unordered_map<std::string, std::function<void (Scene&)>> initScenes;
        std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
        std::string currentScene;
        RenderSystem renderSystem;
        std::unique_ptr<ServerNetworkSystem> serverNetworkSystem;
        // std::unordered_map<std::type_index, std::shared_ptr<>>;


        std::string ip = "127.0.0.1";
        std::uint16_t port = 8089;
        float tickRate = 0.0083;
        bool isServer; // applique les sytemes
        bool debug;  // debug ⚠️ Pas de logique encore implémenté
        bool isLocalClient; // Local Client Only for servers  si il y a un client local ⚠️ Pas de logique encore implémenté
};

// plus tard
        // std::shared_ptr<Scene> getScene(std::string sceneName) {
        //     auto it = scenes.find(sceneName);
        //     if (it == scenes.end()) {
        //         std::cout << "IL existe pas\n";
        //         return nullptr;
        //     }
        //     std::cout << "Get scene\n";
        //     return it->second;
        // }