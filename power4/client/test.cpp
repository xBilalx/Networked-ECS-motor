#include "../../ecs/common/ecs/scene/sceneManager.hpp"
#include "../../ecs/common/ecs/Model/MenuModel.hpp"
#include "../../ecs/common/ecs/Model/GridModel.hpp"
#include "../../ecs/common/ecs/Model/ArrowModel.hpp"
#include "../../ecs/common/ecs/components/Player/PlayerComponent.hpp"
#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>

#include <bits/stdc++.h>


int main() {
    sceneManager SceneManager(false, true);
    SceneManager.addScene("sync_test", [](Scene& scene) {
        scene.isNetworked = true;
        scene.serverAdress = "127.0.0.1";
        scene.port = 8090;
    });
    SceneManager.setCurrentScene("sync_test");
    SceneManager.run();
}
