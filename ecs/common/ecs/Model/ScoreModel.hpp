#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Text/TextComponent.hpp"


#include "../entitiesManager.hpp"

class ScoreModel {
public:
    ScoreModel(Scene& scene) {
        static sf::Font font; // Static pour éviter la destruction en fin de scope
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            std::cerr << "❌ ERREUR: Impossible de charger la police !" << std::endl;
            return;
        }

        std::size_t scoreEntity = scene.createEntity();
        scene.addComponent<PositionComponent>(scoreEntity, 600, 20);
        scene.addComponent<TextComponent>(scoreEntity, "0 - 0", font, 40, sf::Color::White);
    }
};