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
   Power4Scene(sf::RenderWindow& sharedWindow)
        : window(sharedWindow),
          gridComponent(6, 7),
          currentPlayer(1), currentArrowColumn(3), arrowBounceOffset(0.0f), arrowBounceDirection(1) {
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police par défaut." << std::endl;
        }
        winText.setFont(font);
        winText.setCharacterSize(40);
        winText.setFillColor(sf::Color::White);
        winText.setStyle(sf::Text::Bold);
    }
    void initialize() {
        logicSystem.resetGame(gridSystem, gridComponent);
        gameState.setGameOver(false);
        gameState.setCurrentPlayerId(currentPlayer);
        endGameTimer = 0.0f;
        window.create(sf::VideoMode(800, 800), "Puissance 4");
        if (!backgroundTexture.loadFromFile("assets/power4_background.png")) {
            std::cerr << "Erreur: Impossible de charger le fond d'ecran." << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );
        if (!blueBubbleTexture.loadFromFile("assets/blue_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger blue_bubble.png." << std::endl;
        }
        if (!yellowBubbleTexture.loadFromFile("assets/yellow_bubble.png")) {
            std::cerr << "Erreur: Impossible de charger yellow_bubble.png." << std::endl;
        }
        if (!popBubbleTexture.loadFromFile("assets/bubble_pop.png")) {
            std::cerr << "Erreur: Impossible de charger bubble_pop.png." << std::endl;
        }
        if (!blueArrowTexture.loadFromFile("assets/blue_arrow.png")) {
            std::cerr << "Erreur: Impossible de charger blue_arrow.png." << std::endl;
        }
        if (!yellowArrowTexture.loadFromFile("assets/yellow_arrow.png")) {
            std::cerr << "Erreur: Impossible de charger yellow_arrow.png." << std::endl;
        }
        arrowSprite.setTexture(currentPlayer == 1 ? blueArrowTexture : yellowArrowTexture);
        arrowSprite.setScale(0.9f, 0.9f);
        std::cout << "Power4Scene initialized." << std::endl;
    }
    void update() {
        if (gameState.getGameOver()) {
            winText.setString((currentPlayer == 1 ? "Blue Bubble" : "Yellow Bubble") + std::string("\nwon the game!"));
            winText.setPosition(
                (window.getSize().x - winText.getLocalBounds().width) / 2.0f,
                30.0f
            );
            std::cout << "A player won." << std::endl;

            window.clear();
            window.draw(backgroundSprite);
            displayGrid();
            window.draw(winText);
            window.display();
            endGameTimer += 0.016f;
            return;
        }

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left && currentArrowColumn > 0) {
                    currentArrowColumn--;
                } else if (event.key.code == sf::Keyboard::Right && currentArrowColumn < gridComponent.getCols() - 1) {
                    currentArrowColumn++;
                } else if (event.key.code == sf::Keyboard::Space) {
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
        arrowBounceOffset += 0.1f * arrowBounceDirection;
        if (std::abs(arrowBounceOffset) > 10.0f) {
            arrowBounceDirection *= -1;
        }
        window.clear();
        window.draw(backgroundSprite);
        displayGrid();
        displayArrow();
        window.display();
    }
    bool isGameOver() const {
        return gameState.getGameOver() && endGameTimer >= 3.0f;
    }
private:
    sf::RenderWindow& window;
    GridComponent gridComponent;
    GameStateComponent gameState;
    GridSystem gridSystem;
    Power4LogicSystem logicSystem;
    int currentPlayer;
    int currentArrowColumn;
    float arrowBounceOffset;
    int arrowBounceDirection;
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
    float endGameTimer;

    void displayGrid() {
        float cellSize = 80.0f;
        sf::RectangleShape cell(sf::Vector2f(cellSize - 5.0f, cellSize - 5.0f));
        sf::Sprite cellSprite;
        float offsetX = (window.getSize().x - (gridComponent.getCols() * cellSize)) / 2.0f;
        float offsetY = (window.getSize().y - (gridComponent.getRows() * cellSize)) / 2.0f;

        for (size_t row = 0; row < gridComponent.getRows(); ++row) {
            for (size_t col = 0; col < gridComponent.getCols(); ++col) {
                int cellState = gridComponent.getCell(row, col);
                cell.setPosition(offsetX + col * cellSize, offsetY + row * cellSize);
                cell.setFillColor(sf::Color(200, 200, 200, 200)); // Gris clair transparent
                window.draw(cell);
                if (cellState == 0) {
                    continue; // Pas de sprite pour une cellule vide
                } else if (cellState == 1) {
                    cellSprite.setTexture(blueBubbleTexture);
                } else if (cellState == 2) {
                    cellSprite.setTexture(yellowBubbleTexture);
                } else if (cellState == -1) {
                    cellSprite.setTexture(popBubbleTexture);
                }
                cellSprite.setScale(
                    (cellSize - 5.0f) / blueBubbleTexture.getSize().x,
                    (cellSize - 5.0f) / blueBubbleTexture.getSize().y
                );
                cellSprite.setPosition(offsetX + col * cellSize, offsetY + row * cellSize);
                window.draw(cellSprite);
            }
        }
    }

    void displayArrow() {
        float cellSize = 80.0f;
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
