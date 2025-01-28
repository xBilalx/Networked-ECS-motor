#pragma once

#include "../../ecs/common/ecs/components/Grid/GridComponent.hpp"
#include "../../ecs/common/ecs/components/GameState/GameStateComponent.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include "../logic/Power4LogicSystem.hpp"
#include "../../ecs/common/ecs/systems/Grid/GridSystem.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>

class Power4Scene {
public:
    Power4Scene()
        : gridComponent(6, 7), // Initialisation de la grille avec 6 lignes et 7 colonnes
          currentPlayer(1), currentArrowColumn(3), arrowBounceOffset(0.0f), arrowBounceDirection(1) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police par défaut." << std::endl;
        }
        winText.setFont(font);
        winText.setCharacterSize(40);
        winText.setFillColor(sf::Color::White);
        winText.setStyle(sf::Text::Bold);
    }

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
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

        // Charger les sprites des joueurs
        if (!blueBubbleTexture.loadFromFile("assets/blue_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger blue_bubble.png." << std::endl;
        }
        if (!yellowBubbleTexture.loadFromFile("assets/yellow_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger yellow_bubble.png." << std::endl;
        }
        if (!popBubbleTexture.loadFromFile("assets/bubble_pop.png")) {
            std::cerr << "Erreur: Impossible de charger bubble_pop.png." << std::endl;
        }

        // Charger les flèches des joueurs
        if (!blueArrowTexture.loadFromFile("assets/blue_arrow.png")) {
            std::cerr << "Erreur: Impossible de charger blue_arrow.png." << std::endl;
        }
        if (!yellowArrowTexture.loadFromFile("assets/yellow_arrow.png")) {
            std::cerr << "Erreur: Impossible de charger yellow_arrow.png." << std::endl;
        }

        // Configurer la flèche initiale
        arrowSprite.setTexture(currentPlayer == 1 ? blueArrowTexture : yellowArrowTexture);
        arrowSprite.setScale(0.9f, 0.9f); // Ajuster la taille proportionnelle à la cellule

        std::cout << "Power4Scene initialized." << std::endl;
    }

    // Mise à jour de la scène
    void update() {
        if (gameState.getGameOver()) {
            // Définir le texte de victoire
            winText.setString((currentPlayer == 1 ? "Blue Bubble" : "Yellow Bubble") + std::string("\nwon the game!"));
            winText.setPosition(
                (window.getSize().x - winText.getLocalBounds().width) / 2.0f, // Centrage horizontal
                30.0f // Position en haut de l'écran
            );

            // Afficher l'écran final
            window.clear();
            window.draw(backgroundSprite);
            displayGrid();
            window.draw(winText);
            window.display();
            return;
        }

        // Gestion des événements de la fenêtre
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            // Déplacer la flèche avec les touches gauche et droite
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left && currentArrowColumn > 0) {
                    currentArrowColumn--;
                } else if (event.key.code == sf::Keyboard::Right && currentArrowColumn < gridComponent.getCols() - 1) {
                    currentArrowColumn++;
                } else if (event.key.code == sf::Keyboard::Space) {
                    // Confirmer la position et placer le jeton
                    if (logicSystem.placeToken(gridSystem, gridComponent, currentArrowColumn, currentPlayer)) {
                        if (logicSystem.checkWin(gridSystem, gridComponent, currentPlayer)) {
                            logicSystem.replaceWinningBubbles(gridComponent); // Remplacer les cellules gagnantes
                            gameState.setGameOver(true);
                        } else {
                            currentPlayer = (currentPlayer == 1) ? 2 : 1;
                            gameState.setCurrentPlayerId(currentPlayer);
                            arrowSprite.setTexture(currentPlayer == 1 ? blueArrowTexture : yellowArrowTexture);
                        }
                    }
                }
            }
        }

        // Animation de la flèche (rebond)
        arrowBounceOffset += 0.1f * arrowBounceDirection;
        if (std::abs(arrowBounceOffset) > 10.0f) {
            arrowBounceDirection *= -1;
        }

        // Dessine la scène
        window.clear();
        window.draw(backgroundSprite); // Dessine le fond d'écran
        displayGrid(); // Dessine la grille par-dessus
        displayArrow(); // Dessine la flèche au-dessus
        window.display();
    }

private:
    GridComponent gridComponent;
    GameStateComponent gameState;
    GridSystem gridSystem;
    Power4LogicSystem logicSystem;
    int currentPlayer;
    int currentArrowColumn;
    float arrowBounceOffset;
    int arrowBounceDirection;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text winText;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture blueBubbleTexture;
    sf::Texture yellowBubbleTexture;
    sf::Texture popBubbleTexture;
    sf::Texture blueArrowTexture;
    sf::Texture yellowArrowTexture;
    sf::Sprite arrowSprite;

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
                } else if (cellState == -1) {
                    cellSprite.setTexture(popBubbleTexture);
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

    // Dessine la flèche au-dessus de la grille
    void displayArrow() {
        float cellSize = 80.0f;

        // Positionnement centré de la grille
        float offsetX = (window.getSize().x - (gridComponent.getCols() * cellSize)) / 2.0f;
        float offsetY = (window.getSize().y - (gridComponent.getRows() * cellSize)) / 2.0f;
        float arrowYPosition = offsetY - cellSize - 10.0f + arrowBounceOffset; // Animation rebond

        arrowSprite.setScale(
            (cellSize - 5.0f) / blueArrowTexture.getSize().x,
            (cellSize - 5.0f) / blueArrowTexture.getSize().y
        );
        arrowSprite.setPosition(offsetX + currentArrowColumn * cellSize, arrowYPosition);
        window.draw(arrowSprite);
    }
};
