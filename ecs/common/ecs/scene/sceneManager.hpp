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


        void setCurrentScene(std::string scene) {
            currentScene = scene;
        }

        void run() {

            if (isLocalClient || !isServer) {
                renderSystem.createWindow(1920, 1080, "Server Render");
            }


            int check = 0;
            while(1) {
                isNewScene = false;
                std::cout << currentScene <<"ee\n";
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
            if (it->second) {
                std::cout << it->second->createEntity() << "ss" << std::endl;
            }
            
            if (isServer) {
                return runSceneServer(*it->second);
            }
            if (!isServer) {
                return runSceneClient(*it->second);
            }
            return false;
        }

        void changeScene(std::string sceneName) {
            std::cout << "CHANGE SCENE" << sceneName << std::endl;
        }
        void reloadScene(std::string sceneName) {
            std::cout << "NEW SCENE" << sceneName << std::endl;
        }
        bool isNewScene; // A mettre en privée


    private:
        bool runSceneServer(Scene &em) {
            
            InputSystem inputSystem;
            TimeSystem timeSystem;
            
            MovementSystem movementSystem;
            ServerNetworkSystem serverNetworkSystem("127.0.0.1", 8089, 0.0083);

            sf::Clock clock;
            sf::RenderWindow& win = renderSystem.getWindow();

            // peut être gérer les système dans le systeme manager pour que le dev puisse mieux config ??
            while(1) {
                float dt = clock.restart().asSeconds();
                // serverNetworkSystem.dataFromClients(em);
                timeSystem.update(em, dt);
                if (isLocalClient) {
                    inputSystem.updateForServer(em, win);
                }
                movementSystem.update(em);
                // serverNetworkSystem.dataToClients(em, dt);
                win.clear();
                renderSystem.update(em);
                win.display();
                if (isNewScene) {
                    return true;
                }
            }
        }

        // Clients Scenes
        bool runSceneClient(Scene &em) {
            
            InputSystem inputSystem;
            TimeSystem timeSystem;
            bool chechk = false;


            MovementSystem movementSystem;
            ClientNetworkSystem clientNetworkSystem("127.0.0.1", 8089, 0.0083);

            sf::Clock clock;
            sf::RenderWindow& win = renderSystem.getWindow();

            while(win.isOpen()) {
                float dt = clock.restart().asSeconds();
                if (!chechk) {
                    clientNetworkSystem.test(); // Envoie un paquet CONNECT mais a upgrade
                    chechk = true;
                }
                timeSystem.update(em, dt);
                clientNetworkSystem.dataToServer(em, inputSystem, dt);
                clientNetworkSystem.dataFromServer(em);
                win.clear();
                renderSystem.update(em);
                win.display();
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

        bool isServer; // applique les sytemes
        bool debug;  // debug ⚠️ Pas de logique encore implémenté
        bool isLocalClient; // Local Client Only for servers  si il y a un client local ⚠️ Pas de logique encore implémenté
};
