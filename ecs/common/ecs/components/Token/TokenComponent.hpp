#pragma once

#include "../Component.hpp"

struct TokenComponent : public Component {
    int playerId;
    
    TokenComponent(int playerId) : playerId(playerId) {}
};
