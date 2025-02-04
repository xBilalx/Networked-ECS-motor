#pragma once

#include "../Component.hpp"

struct ScoreComponent : public Component {
    int score;
    int playerId; // 1 pour le joueur de gauche, 2 pour celui de droite

    ScoreComponent(int playerId) : score(0), playerId(playerId) {}

    void increaseScore() {
        score++;
    }
};