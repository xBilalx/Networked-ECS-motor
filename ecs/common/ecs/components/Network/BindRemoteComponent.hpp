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

struct BindClientComponentTest : public Component {
    public:
        int bindId;
        bool ManagePosByClient = true;
        bool ManagePosByServ;
        BindClientComponentTest(int bindId, bool ManagePosByServ = true) : bindId(bindId), ManagePosByServ(ManagePosByServ) {}
};
