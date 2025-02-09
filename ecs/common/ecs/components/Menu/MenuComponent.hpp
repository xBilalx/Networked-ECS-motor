#pragma once

#include "../Component.hpp"
#include <functional>


struct MenuComponent {
    std::function<void()> onClick;
    bool isSelected;

    MenuComponent(std::function<void()> callback = nullptr)
        : onClick(callback), isSelected(false) {}
};