#pragma once

#include "../Component.hpp"

struct ScoreComponent : public Component {
    int score;

    ScoreComponent() : score(0) {}

    void increaseScore() {
        score++;
    }
};
