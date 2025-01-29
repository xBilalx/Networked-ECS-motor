#pragma once

#include "../Component.hpp"
#include <functional>
//

struct MenuComponent {
    std::function<void()> onClick; // Fonction appelée au clic
    bool isSelected;               // Indique si le bouton est en focus (clavier)

    MenuComponent(std::function<void()> callback = nullptr)
        : onClick(callback), isSelected(false) {}
};