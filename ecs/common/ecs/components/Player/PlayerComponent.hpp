#pragma once

#include "../Component.hpp"

struct PlayerComponent : public Component {
    int playerId;
    
    PlayerComponent(int playerId) : playerId(playerId) {}
};
