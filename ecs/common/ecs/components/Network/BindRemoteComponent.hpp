#pragma once

#include "../Component.hpp"
#include <string>

struct BindClientComponent : public Component {
    public:
        std::string ipClient;
        unsigned short portClient;
        bool connected = false;
        bool ManagePosByServ = true;
        BindClientComponent() {}
};