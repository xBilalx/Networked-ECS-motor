#pragma once

#include "../../ecs/common/ecs/components/Grid/GridComponent.hpp"
#include "../../ecs/common/ecs/components/GameState/GameStateComponent.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include "../logic/Power4LogicSystem.hpp"
#include "../../ecs/common/ecs/systems/Grid/GridSystem.hpp"
#include <SFML/Graphics.hpp>

class Power4Scene {
public:
    Power4Scene()
        : gridComponent(6, 7), // Initialisation de la grille avec 6 lignes et 7 colonnes
          currentPlayer(1) {}

    // Initialisation de la scène
    void initialize() {
        // Réinitialise la grille
        logicSystem.resetGame(gridSystem, gridComponent);
        // Configure l'état du jeu
        gameState.setGameOver(false);
        gameState.setCurrentPlayerId(currentPlayer);

        // Initialisation de la fenêtre SFML
        window.create(sf::VideoMode(800, 800), "Puissance 4");

        // Charger le fond d'écran
        if (!backgroundTexture.loadFromFile("assets/power4_background.png")) {
            std::cerr << "Erreur: Impossible de charger le fond d'ecran." << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);

        // Charger les sprites des joueurs
        if (!blueBubbleTexture.loadFromFile("assets/blue_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger blue_bubble.png." << std::endl;
        }
        if (!yellowBubbleTexture.loadFromFile("assets/yellow_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger yellow_bubble.png." << std::endl;
        }

        // Ajuster la taille du sprite pour correspondre à la fenêtre
        sf::Vector2u textureSize = backgroundTexture.getSize();
        sf::Vector2u windowSize = window.getSize();
        backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
        );

        std::cout << "Power4Scene initialized." << std::endl;
    }

    // Mise à jour de la scène
    void update() {
        if (gameState.getGameOver()) {
            std::cout << "Game Over! Player " << currentPlayer << " wins!" << std::endl;
            return;
        }

        // Gestion des événements de la fenêtre
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
        }

        // Dessine la scène
        window.clear();
        window.draw(backgroundSprite); // Dessine le fond d'écran
        displayGrid(); // Dessine la grille par-dessus
        window.display();

        // Gestion des entrées : choix de la colonne (temporaire pour le texte)
        size_t column;
        std::cout << "Player " << currentPlayer << ", choose a column (0-6): ";
        std::cin >> column;

        if (!logicSystem.placeToken(gridSystem, gridComponent, column, currentPlayer)) {
            std::cout << "Invalid move. Try again." << std::endl;
            return;
        }

        // Vérifie si le joueur courant a gagné
        if (logicSystem.checkWin(gridSystem, gridComponent, currentPlayer)) {
            gameState.setGameOver(true);
            return;
        }

        // Passe au joueur suivant
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        gameState.setCurrentPlayerId(currentPlayer);
    }

private:
    GridComponent gridComponent;
    GameStateComponent gameState;
    GridSystem gridSystem;
    Power4LogicSystem logicSystem;
    int currentPlayer;
    sf::RenderWindow window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture blueBubbleTexture;
    sf::Texture yellowBubbleTexture;

    // Dessine la grille graphiquement
    void displayGrid() {
        float cellSize = 80.0f;
        sf::RectangleShape cell(sf::Vector2f(cellSize - 5.0f, cellSize - 5.0f));
        sf::Sprite cellSprite;

        // Positionnement centré de la grille
        float offsetX = (window.getSize().x - (gridComponent.getCols() * cellSize)) / 2.0f;
        float offsetY = (window.getSize().y - (gridComponent.getRows() * cellSize)) / 2.0f;

        for (size_t row = 0; row < gridComponent.getRows(); ++row) {
            for (size_t col = 0; col < gridComponent.getCols(); ++col) {
                int cellState = gridComponent.getCell(row, col);

                // Dessiner les bordures de la cellule (grille visible)
                cell.setPosition(offsetX + col * cellSize, offsetY + row * cellSize);
                cell.setFillColor(sf::Color(200, 200, 200, 200)); // Gris clair transparent
                window.draw(cell);

                // Gestion des sprites selon l'état de la cellule
                if (cellState == 0) {
                    continue; // Pas de sprite pour une cellule vide
                } else if (cellState == 1) {
                    cellSprite.setTexture(blueBubbleTexture);
                } else if (cellState == 2) {
                    cellSprite.setTexture(yellowBubbleTexture);
                }

                // Redimensionner les sprites pour s'adapter à la taille des cellules
                cellSprite.setScale(
                    (cellSize - 5.0f) / blueBubbleTexture.getSize().x,
                    (cellSize - 5.0f) / blueBubbleTexture.getSize().y
                );

                // Positionne le sprite avec offset pour centrer
                cellSprite.setPosition(offsetX + col * cellSize, offsetY + row * cellSize);

                // Dessine le sprite de la cellule
                window.draw(cellSprite);
            }
        }
    }
};