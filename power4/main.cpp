#include "scenes/MenuScene.hpp"
#include "scenes/Power4Scene.hpp"

int main() {
    MenuScene menuScene;
    menuScene.initialize();

    // Boucle principale pour afficher et interagir avec le menu
    while (true) {
        menuScene.update(); // On ne gère pas de `sceneManager` ici pour se concentrer sur le menu
    }

    return 0;
}



// #include <iostream>
// #include <limits> // Inclusion pour std::numeric_limits
// #include "scenes/Power4Scene.hpp"

// int main() {
//     // Crée et initialise la scène Power4
//     Power4Scene power4Scene;
//     power4Scene.initialize();

//     // Boucle principale du jeu
//     while (true) {
//         power4Scene.update();
//         if (std::cin.fail()) {
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Correction
//         }
//     }

//     return 0;
// }


// #include "scenes/MenuScene.hpp"
// #include "scenes/Power4Scene.hpp"
// #include "../ecs/common/ecs/scene/sceneManager.hpp"


// int main() {
//     sceneManager SceneManager(true, true);

//     // Ajouter la scène du menu principal
//     SceneManager.addScene("Menu", [](Scene& scene) {
//         MenuScene menuScene;
//         menuScene.initialize();

//         while (true) {
//             menuScene.update(scene.SceneManager);
//             if (scene.SceneManager->isNewScene) break;
//         }
//     });

//     // Ajouter la scène du jeu Puissance 4
//     SceneManager.addScene("Power4", [](Scene& scene) {
//         Power4Scene power4Scene;
//         power4Scene.initialize();

//         while (true) {
//             power4Scene.update();
//             if (scene.SceneManager->isNewScene) break;
//         }
//     });

//     // Configurer la scène initiale comme le menu
//     SceneManager.setCurrentScene("Menu");
//     SceneManager.run();

//     return 0;
// }
