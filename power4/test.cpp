#include <iostream>
#include <limits> // Inclusion pour std::numeric_limits

#include "./scenes/Power4Scene.hpp"

int main() {
    // Crée et initialise la scène Power4
    Power4Scene power4Scene;
    power4Scene.initialize();

    // Boucle principale du jeu
    while (true) {
        power4Scene.update();
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}
