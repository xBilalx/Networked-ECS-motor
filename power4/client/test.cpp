#include "../../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include <bits/stdc++.h>

std::function<void (Scene&)> onClickPlayButton = [](Scene& em) {
    em.SceneManager->isNewScene = true;
    em.SceneManager->setCurrentScene("GAME");
};


int main() {
    sceneManager SceneManager(false, false);

    sf::Font font;
    font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");

    SceneManager.addScene("MENU", [&font](Scene& scene) {
        std::vector<std::tuple<std::string, float, float, float, float, sf::Color, sf::Color, sf::Color, std::function<void (Scene&)>>> menuItems = {
            {"Play", -1, -1, -1, -1,sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, onClickPlayButton},
            {"Settings", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}},
            {"Exit", -1, -1, -1, -1, sf::Color::Blue, sf::Color(70, 70, 200), sf::Color::White, [](Scene& em) {}}
        };

        MenuModel menu(scene, "../../assets/menu_background.png", menuItems, font);
    });

    SceneManager.addScene("GAME", [](Scene& scene) {
        scene.isNetworked = true;
        scene.serverAdress = "127.0.0.1";
        scene.port = 8090;
    });

    SceneManager.setCurrentScene("MENU");
    SceneManager.run();
}
