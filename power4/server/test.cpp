#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"

#include <SFML/Graphics.hpp>

#include <bits/stdc++.h>


int main() {
    sceneManager SceneManager(true, true);

    SceneManager.addScene("Power4Scene", [](Scene& scene) {
        float cellSize = 80.0f;
        GridModel grid(scene, "../assets/menu_background.png", 6, 7, cellSize);
        ArrowModel arrow(scene, "../assets/blue_arrow.png", cellSize);
    });

    SceneManager.setCurrentScene("Power4Scene");
    SceneManager.setServerNetwork("127.0.0.1", 8090, 0.0083);
    SceneManager.run();
}