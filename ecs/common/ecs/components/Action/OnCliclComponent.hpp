#pragma once

#include "../Component.hpp"
#include <string>
#include <SFML/Graphics.hpp>
#include <functional>

class Scene;

// Marche seulement avec le composant rectangle
struct OnClickComponent : public Component
{
    std::function<void (Scene&)> action;

    OnClickComponent( std::function<void (Scene&)> action) : action(action) {}
};
