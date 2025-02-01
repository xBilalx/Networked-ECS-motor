#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include <SFML/Graphics.hpp>

#include <bits/stdc++.h>

int main() {
    sceneManager SceneManager(true, true);
    
    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    SceneManager.addScene("MainMenu", [&font](Scene& scene) {
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color>> menuItems = {
            {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
            {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White},
            {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White}
        };

        MenuModel menu(scene, "../assets/menu_background.png", menuItems, font);
    });

    SceneManager.setCurrentScene("MainMenu");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
    SceneManager.run();
}