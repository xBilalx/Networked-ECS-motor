#pragma once

#include "../Component.hpp"
#include <SFML/Window/Keyboard.hpp>

struct PlayerInputComponent : public Component {
    int playerId; // 1 = Joueur 1, 2 = Joueur 2

    PlayerInputComponent(int playerId) : playerId(playerId) {}
};
