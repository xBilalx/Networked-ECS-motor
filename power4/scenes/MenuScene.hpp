#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/components/Render/WindowComponent.hpp"
#include "../../ecs/common/ecs/components/Input/InputComponent.hpp"
#include "../../ecs/common/ecs/components/Transform/PositionComponent.hpp"
#include "../../ecs/common/ecs/components/Network/BindRemoteComponent.hpp"
#include "../../ecs/common/ecs/components/Time/CoolDownAction.hpp"
#include "../../ecs/common/ecs/entitiesManager.hpp"
#include "../../ecs/common/ecs/systems/Render/RenderSystem.hpp"
#include "../../ecs/common/ecs/systems/Input/InputSystem.hpp"
#include "../../ecs/common/ecs/systems/Network/NetworkManager.hpp"
#include "../../ecs/common/ecs/systems/Network/ServerNetwork.hpp"
#include "../../ecs/common/ecs/systems/Transform/MovementSystem.hpp"
#include "../../ecs/common/ecs/scene/sceneManager.hpp"

#include <bits/stdc++.h>


class MenuScene {
public:
    MenuScene() {
        // Initialisation de la fenêtre SFML
        window.create(sf::VideoMode(800, 800), "Menu Principal");
    }

    void initialize() {
        std::cout << "MenuScene initialized." << std::endl;
    }

    void update(sceneManager* sceneManager) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
        }

        // Afficher le menu
        window.clear();
        displayMenu();
        window.display();

        // Gestion des entrées utilisateur (temporairement clavier)
        int choice;
        std::cout << "1: Start Game\n2: Quit\nEnter your choice: ";
        std::cin >> choice;

        if (choice == 1) {
            // Passer à la scène du jeu
            sceneManager->setCurrentScene("Power4");
            sceneManager->isNewScene = true;
        } else if (choice == 2) {
            window.close();
            exit(0);
        }
    }

private:
    sf::RenderWindow window;

    void displayMenu() {
        // Menu visuel simplifié (remplacer par des éléments graphiques si nécessaire)
        sf::Font font;
        font.loadFromFile("path/to/font.ttf");

        sf::Text title("Puissance 4", font, 50);
        title.setPosition(250, 100);
        title.setFillColor(sf::Color::White);

        sf::Text startGame("1: Start Game", font, 30);
        startGame.setPosition(250, 300);
        startGame.setFillColor(sf::Color::White);

        sf::Text quitGame("2: Quit", font, 30);
        quitGame.setPosition(250, 400);
        quitGame.setFillColor(sf::Color::White);

        window.draw(title);
        window.draw(startGame);
        window.draw(quitGame);
    }
};
