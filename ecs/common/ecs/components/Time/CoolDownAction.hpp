#pragma once

#include "../Component.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>

class Scene;

struct CoolDownActionComponent : public Component
{
    bool isInfini; // ⚠️ PAS ENCORE implémenté
    float coolDown; // in milli seconde
    float currentTime;
    std::function<void (Scene&)> action;
    bool isEnd; // peut être direct supprimé le composant ? 

    CoolDownActionComponent(bool isInfini, float coolDown, std::function<void (Scene&)> action) : isInfini(isInfini), coolDown(coolDown), currentTime(0), action(action),  isEnd(false) {}
};
