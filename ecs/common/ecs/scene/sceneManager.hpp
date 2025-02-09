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
#include "../systems/Action/OnClickSytem.hpp"
#include "../systems/Input/KeyboardInputSystem.hpp"

#include <functional>

class sceneManager
{
public:
    sceneManager(bool isServer, bool debug = false, bool isLocalClient = true)
        : isNewScene(false), isServer(isServer), debug(debug), isLocalClient(isLocalClient)
    {

        // Initialiser la fenêtre dès le début
        initializeWindow();
    }

    // void addScene(std::string sceneName, std::function<void(Scene &)> initSceneLambda)
    // {
    //     initScenes[sceneName] = initSceneLambda;
    //     auto scene = std::make_shared<Scene>(this);
    //     scenes[sceneName] = scene;
    //     initSceneLambda(*scene);
    // }

    void addScene(std::string sceneName, std::function<void(Scene &)> initSceneLambda)
    {
        initScenes[sceneName] = initSceneLambda;
        scenes[sceneName] = nullptr;
    }

    void setServerNetwork(std::string ip_, unsigned short int port_, int nbrClients_, float tickRate_)
    {
        ip = ip_;
        port = port_;
        nbrClient = nbrClients_;
        tickRate = tickRate_;
    }

    void initServerNetwork(std::string ipServer, uint16_t portServer, bool managePos_, float tickRate_) {
        clientNetworkSystemTest = std::make_unique<ClientNetworkSystem>(ipServer, portServer, managePos_, tickRate_);
    }

    std::unique_ptr<ClientNetworkSystem>& getClientNetworkSystem() {
        return clientNetworkSystemTest;
    }

    std::unique_ptr<ServerNetworkSystem>& getServerNetworkSystem() {
        return serverNetworkSystem;
    }

    void setCurrentScene(std::string scene)
    {
        currentScene = scene;
    }

    void run()
    {
        if (isServer)
        {
            serverNetworkSystem = std::make_unique<ServerNetworkSystem>(ip, port, nbrClient, tickRate);
        }

        int check = 0;
        while (1)
        {
            isNewScene = false;
            check = runScene(currentScene);
            if (!check)
            {
                std::cout << "Erreur de Scene\n";
                return;
            }
        }
    }

    RenderSystem &getRenderSystem()
    {
        return renderSystem;
    }

    // A mettre en privée

    bool isNewScene; 
    bool managePos = false; // [ONLY CLIENT] Quand client recoit un Input Component, si True : le Client va directement gérer ces pos du composant et envoyer au serveur, si False : le client va envoyer de facon continue c'est Input

private:
    void initializeWindow()
    {
        if (isLocalClient || !isServer)
        {
            renderSystem.createWindow(1920, 1080, "ECS Game Window");
            std::cout << "Fenêtre créée avec succès: 1920x1080" << std::endl;
        }
    }

    // bool runScene(std::string sceneName)
    // {
    //     auto it = scenes.find(sceneName);
    //     if (it == scenes.end())
    //     {
    //         std::cout << "La scène " << sceneName << " n'existe pas\n";
    //         return false;
    //     }

    //     return isServer ? runSceneServer(*it->second) : runSceneClient(*it->second);
    // }

    bool runScene(std::string sceneName)
    {
        auto it = initScenes.find(sceneName);
        if (it == initScenes.end())
        {
            std::cout << "La scène " << sceneName << " n'existe pas\n";
            return false;
        }
        if (!scenes[sceneName]) {
            scenes[sceneName] = std::make_shared<Scene>(this);
            it->second(*scenes[sceneName]);
        }
        return isServer ? runSceneServer(*scenes[sceneName]) : runSceneClient(*scenes[sceneName]);
    }


    bool runSceneServer(Scene &em)
    {
        // InputSystem inputSystem;
        TimeSystem timeSystem;
        MovementSystem movementSystem;
        sf::Clock clock;
        sf::RenderWindow &win = renderSystem.getWindow();

        // il faudra les ajouter les systemes de facon générique, parce qu'on à pas besoin de ces systemes souvent par exemple !
        KeyboardInputSystem keyBoardInputSystem;

        while (1)
        {
            float dt = clock.restart().asSeconds();
            if (debug)
            {
                std::cout << "Time for loop -> " << dt << "s\n";
            }
            serverNetworkSystem->dataFromClients(em);

            timeSystem.update(em, dt);
            if (isLocalClient)
            {
                sf::Event event;
                while (win.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        win.close();
                    }
                }
            }            
            em.updateSystems(dt);
            serverNetworkSystem->dataToClients(em, dt);

            renderSystem.update(em);
            if (isNewScene)
            {
                serverNetworkSystem->sendClearScene();
                return true;
            }
        }
    }

    bool runSceneClient(Scene &em)
    { // Networked
        // InputSystem inputSystem;
        TimeSystem timeSystem;
        MovementSystem movementSystem;
        sf::Clock clock;
        sf::RenderWindow &win = renderSystem.getWindow();
        bool isNetworked = em.isNetworked;
        KeyboardInputSystem keyBoardInputSystem;

        if (isNetworked && !clientNetworkSystemTest) {
            std::cout << "Scene Networked but Client Network isn't used. Please use initClientNetwork function !" << std::endl;
            return false;
        }

        OnClickSytem onclickSystem(&win);

        while (win.isOpen())
        {

            float dt = clock.restart().asSeconds();
            if (debug)
            {
                std::cout << "Time for loop -> " << dt << "s\n";
            }
            timeSystem.update(em, dt);
            keyBoardInputSystem.resetKeyRelease(win, em);
            sf::Event event;

            while (win.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    win.close();
                }
                onclickSystem.handleEvent(event, em);
                keyBoardInputSystem.handleEvent(win, event, em, true);
            }

            // movementSystem.update(em, dt);
            em.updateSystems(dt);

            if (isNetworked)
            {
                clientNetworkSystemTest.get()->dataToServer(em, dt);
                clientNetworkSystemTest.get()->dataFromServer(em);
            }

            renderSystem.update(em);

            if (isNewScene)
            {
                return true;
            }
        }
        return false;
    }
    std::unordered_map<std::string, std::function<void(Scene &)>> initScenes;
    std::unordered_map<std::string, std::shared_ptr<Scene>> scenes;
    std::string currentScene;
    RenderSystem renderSystem;
    std::unique_ptr<ServerNetworkSystem> serverNetworkSystem;
    std::unique_ptr<ClientNetworkSystem> clientNetworkSystemTest;

    std::string ip = "127.0.0.1";
    std::uint16_t port = 8089;
    int nbrClient = 0;
    float tickRate = 0.01667;
    bool isServer;      // applique les sytemes
    bool debug;         // debug ⚠️ Pas de logique encore implémenté
    bool isLocalClient; // Local Client Only for servers  si il y a un client local ⚠️ Pas de logique encore implémenté
};
