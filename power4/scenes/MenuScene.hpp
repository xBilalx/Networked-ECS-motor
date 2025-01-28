#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

class MenuScene {
public:
    MenuScene(sf::RenderWindow& sharedWindow) : window(sharedWindow), startClicked(false), quitClicked(false) {
        // Charger le fond d'écran
        if (!backgroundTexture.loadFromFile("assets/power4_background.png")) {
            std::cerr << "Erreur: Impossible de charger le fond d'écran." << std::endl;
        }
        backgroundSprite.setTexture(backgroundTexture);
        backgroundSprite.setScale(
            static_cast<float>(window.getSize().x) / backgroundTexture.getSize().x,
            static_cast<float>(window.getSize().y) / backgroundTexture.getSize().y
        );

        // Charger la police
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            std::cerr << "Erreur: Impossible de charger la police par défaut." << std::endl;
        }

        // Configuration des boutons
        setupButton(startButton, startButtonBackground, "Play Bubble WaR", 300);
        setupButton(quitButton, quitButtonBackground, "Quit", 400);
    }

    void initialize() {
        std::cout << "MenuScene initialized." << std::endl;

        // Réinitialiser les états des boutons
        resetButtonStates();
        startClicked = false;
        quitClicked = false;
    }

    void update() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                exit(0);
            }

            // Récupérer la position de la souris
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);

            // Gestion des événements de la souris
            if (event.type == sf::Event::MouseMoved) {
                handleButtonHover(startButton, startButtonBackground, mousePosition);
                handleButtonHover(quitButton, quitButtonBackground, mousePosition);
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                if (startButtonBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    std::cout << "Start Game clicked!" << std::endl;
                    startClicked = true;
                } else if (quitButtonBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
                    std::cout << "Quit clicked!" << std::endl;
                    quitClicked = true;
                }
            }
        }

        // Affichage des éléments
        window.clear();
        window.draw(backgroundSprite);
        window.draw(startButtonBackground);
        window.draw(startButton);
        window.draw(quitButtonBackground);
        window.draw(quitButton);
        window.display();
    }

    // Retourne si "Start Game" a été cliqué
    bool isStartClicked() const {
        return startClicked;
    }

    // Retourne si "Quit" a été cliqué
    bool isQuitClicked() const {
        return quitClicked;
    }

private:
    sf::RenderWindow& window;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Font font;
    sf::Text startButton;
    sf::Text quitButton;
    sf::RectangleShape startButtonBackground;
    sf::RectangleShape quitButtonBackground;

    bool startClicked;
    bool quitClicked;

    void setupButton(sf::Text& button, sf::RectangleShape& buttonBackground, const std::string& text, float yPosition) {
        button.setFont(font);
        button.setString(text);
        button.setCharacterSize(40);
        button.setFillColor(sf::Color::White);
        button.setStyle(sf::Text::Bold);

        sf::FloatRect textBounds = button.getLocalBounds();
        button.setPosition(
            (window.getSize().x - textBounds.width) / 2.0f,
            yPosition + 10
        );

        buttonBackground.setSize(sf::Vector2f(textBounds.width + 40, textBounds.height + 30));
        buttonBackground.setFillColor(sf::Color(50, 50, 150));
        buttonBackground.setOutlineColor(sf::Color::White);
        buttonBackground.setOutlineThickness(2);
        buttonBackground.setPosition(
            button.getPosition().x - 20,
            button.getPosition().y - 15
        );
    }

    void handleButtonHover(sf::Text& button, sf::RectangleShape& buttonBackground, const sf::Vector2i& mousePosition) {
        if (buttonBackground.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition))) {
            buttonBackground.setFillColor(sf::Color(70, 70, 200)); // Couleur hover
            button.setFillColor(sf::Color::Yellow); // Texte sur hover
        } else {
            buttonBackground.setFillColor(sf::Color(50, 50, 150)); // Couleur par défaut
            button.setFillColor(sf::Color::White); // Texte par défaut
        }
    }

    void resetButtonStates() {
        startButtonBackground.setFillColor(sf::Color(50, 50, 150)); // Couleur par défaut
        startButton.setFillColor(sf::Color::White);

        quitButtonBackground.setFillColor(sf::Color(50, 50, 150)); // Couleur par défaut
        quitButton.setFillColor(sf::Color::White);
    }
};
