#pragma once

#include "../components/Render/RenderComponent.hpp"
#include "../components/Transform/PositionComponent.hpp"
#include "../components/Text/TextComponent.hpp"
#include "../components/Score/ScoreComponent.hpp"


#include "../entitiesManager.hpp"
class ScoreModel {
public:
    ScoreModel(Scene& scene, sf::Font& font, float posX1, float posY1, float posX2, float posY2) {
        std::cerr << "🎯 Création du ScoreModel\n";

        // Vérification que la police est valide
        if (!font.getInfo().family.empty()) {
            std::cerr << "✅ Police chargée avec succès pour le ScoreModel !\n";
        } else {
            std::cerr << "❌ ERREUR: Police invalide dans ScoreModel !\n";
            return;
        }

        // Création des entités pour les scores des joueurs
        scorePlayer1 = scene.createEntity();
        scorePlayer2 = scene.createEntity();
        std::cerr << "✅ Entités Score créées : " << scorePlayer1 << ", " << scorePlayer2 << "\n";

        // ✅ Ajout du ScoreComponent
        scene.addComponent<ScoreComponent>(scorePlayer1);
        scene.addComponent<ScoreComponent>(scorePlayer2);

        // ✅ Ajout du texte des scores avec une police et une taille correcte
        scene.addComponent<TextComponent>(scorePlayer1, "0", font, 60, sf::Color::White);
        scene.addComponent<TextComponent>(scorePlayer2, "0", font, 60, sf::Color::White);

        // ✅ Position des scores selon les paramètres passés
        scene.addComponent<PositionComponent>(scorePlayer1, posX1, posY1);
        scene.addComponent<PositionComponent>(scorePlayer2, posX2, posY2);

        std::cerr << "✅ ScoreModel initialisé avec succès !\n";
    }

private:
    std::size_t scorePlayer1;
    std::size_t scorePlayer2;
};
