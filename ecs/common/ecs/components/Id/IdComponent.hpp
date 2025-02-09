#pragma once

#include "../Component.hpp"
#include <string>

struct IdComponent : public Component {
    std::string id;

    IdComponent(std::string id_) : id(id_) {}
};