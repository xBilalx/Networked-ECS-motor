#pragma once

#include "../Component.hpp"

struct TokenComponent : public Component {
    int playerId; // 1 = Joueur 1 (bleu), 2 = Joueur 2 (jaune)
    
    TokenComponent(int playerId) : playerId(playerId) {}
};
