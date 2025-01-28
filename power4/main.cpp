#include "scenes/MenuScene.hpp"
#include "scenes/Power4Scene.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Bubble 4");
    MenuScene menuScene(window);
    Power4Scene power4Scene(window);
    enum SceneType { MENU, GAME };
    SceneType currentScene = MENU;
    menuScene.initialize();

    while (window.isOpen()) {
        if (currentScene == MENU) {
            menuScene.update();

            if (menuScene.isStartClicked()) {
                currentScene = GAME;
                power4Scene.initialize();
            } else if (menuScene.isQuitClicked()) {
                window.close();
                break;
            }
        } else if (currentScene == GAME) {
            power4Scene.update();

            if (power4Scene.isGameOver()) {
                currentScene = MENU;
                menuScene.initialize();
            }
        }
    }

    return 0;
}
