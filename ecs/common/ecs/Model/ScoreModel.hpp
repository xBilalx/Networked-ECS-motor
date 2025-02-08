#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Text/TextComponent.hpp"
#include "../components/Score/ScoreComponent.hpp"


#include "../entitiesManager.hpp"
class ScoreModel {
public:
    ScoreModel(Scene& scene, sf::Font& font, float posX1, float posY1, float posX2, float posY2) {
        if (!font.getInfo().family.empty()) {
            std::cerr << "✅ Police chargée avec succès pour le ScoreModel !\n";
        } else {
            std::cerr << "❌ ERREUR: Police invalide dans ScoreModel !\n";
            return;
        }
        scorePlayer1 = scene.createEntity();
        scorePlayer2 = scene.createEntity();
        std::cerr << "✅ Entités Score créées : " << scorePlayer1 << ", " << scorePlayer2 << "\n";
        scene.addComponent<ScoreComponent>(scorePlayer1, 1);
        scene.addComponent<ScoreComponent>(scorePlayer2, 2);
        scene.addComponent<TextComponent>(scorePlayer1, "0", font, 60, sf::Color::White);
        scene.addComponent<TextComponent>(scorePlayer2, "0", font, 60, sf::Color::White);
        scene.addComponent<PositionComponent>(scorePlayer1, posX1, posY1);
        scene.addComponent<PositionComponent>(scorePlayer2, posX2, posY2);
        scene.addComponent<RenderComponent>(scorePlayer1, 1);
        scene.addComponent<RenderComponent>(scorePlayer2, 1);
    }

private:
    std::size_t scorePlayer1;
    std::size_t scorePlayer2;
};