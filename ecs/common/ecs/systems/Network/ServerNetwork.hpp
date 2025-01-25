#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../../components/Network/BindRemoteComponent.hpp"
#include "../../components/Render/RenderComponent.hpp"
#include "../../components/Render/WindowComponent.hpp"

class ServerNetworkSystem {
    public:
        ServerNetworkSystem(std::string serverIp, uint16_t serverPort, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort), coolDown(coolDown) {
            networkManager.bindSocket(serverIp, serverPort);
        }
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }

        void dataToClients(EntityManager& em, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    // std::cout << currentTime << std::endl;
                    currentTime = 0;
                } else {
                    return;
                }
            }
            
            int next = false;
            for (auto itBind = em.entities1.begin(); itBind != em.entities1.end(); itBind++) {
                BindClientComponent *currentbindClient = em.getComponentTest<BindClientComponent>(itBind->first);
                if (currentbindClient && currentbindClient->connected) {
                    // std::cout << "ta le droit frérot\n";
                    for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                        std::string buffer;
                        // auto* window = entityy->getComponent<WindowComponent>();
                        InputComponent* input = em.getComponentTest<InputComponent>(it->first);
                        RenderComponent* render = em.getComponentTest<RenderComponent>(it->first);
                        PositionComponent* position = em.getComponentTest<PositionComponent>(it->first);
                        BindClientComponent *bindClient = em.getComponentTest<BindClientComponent>(it->first);

                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)it->first);
                        // std::cout << "Entity Id --> " << it->first << " :";
                        // if (window) {
                        //     std::cout << " WINDOW " << " |";
                        //     Serializer::serialize(buffer, Serializer::MessageType::WINDOW);
                        //     Serializer::serialize(buffer, (unsigned int) window->modeWidth);
                        //     Serializer::serialize(buffer, (unsigned int) window->modeHeight);
                        // }
                        if (input && currentbindClient == bindClient) {
                            // std::cout << " INPUT "  << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::INPUT);
                        }
                        if (render) {
                            // std::cout << " RENDER "  << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::RENDER);
                            Serializer::serialize(buffer, (std::string) render->pathTexture);
                        }
                        if (position) {
                            if (currentbindClient == bindClient && !currentbindClient->ManagePosByServ) {
                                continue;
                            }
                            if (currentbindClient == bindClient && currentbindClient->ManagePosByServ) {
                                currentbindClient->ManagePosByServ = false;
                            }
                            // std::cout << " POS "  << " |";
                            Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                            Serializer::serialize(buffer, (float) position->position.x);
                            Serializer::serialize(buffer, (float) position->position.y); 
                        }
                        // std::cout << std::endl;
                        Serializer::serialize(buffer, Serializer::MessageType::END);
                        networkManager.sendTo(buffer, currentbindClient->ipClient, currentbindClient->portClient);
                    }
                }
            }
        }

        void dataFromClients(EntityManager& em) {
            std::vector<Packet> packets = networkManager.receiveMessages(true);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::CONNECT) {
                        for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                            BindClientComponent *bindClient = em.getComponentTest<BindClientComponent>(it->first);
                            if (bindClient && !bindClient->connected) {
                                bindClient->ipClient = packet.senderIp;
                                bindClient->portClient = packet.senderPort;
                                bindClient->connected = true;
                                std::cout << "Connected to BIND Client to Server OK " << std::endl;
                                std::string buffer;
                                Serializer::serialize(buffer, Serializer::MessageType::CONNECTED);
                                Serializer::serialize(buffer, Serializer::MessageType::END);
                                networkManager.sendTo(buffer, bindClient->ipClient, bindClient->portClient);
                                std::cout << "\033[1;32mConnexion Accepté pour Client " << packet.senderIp <<"\033[0m\n";
                                return;
                            }
                        }
                        std::cout << "\033[1;31mConnexion Refusé pour Client " << packet.senderIp <<"\033[0m\n";
                        return;
                    }
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                        BindClientComponent* bindClient = em.getComponentTest<BindClientComponent>(entityNbr);
                        messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                        if (messageType == Serializer::MessageType::POSITION)
                            if (bindClient->ipClient == packet.senderIp && bindClient->portClient == packet.senderPort) {
                                float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                PositionComponent* pos = em.getComponentTest<PositionComponent>(entityNbr);
                                // std::cout << "xe -> " << x << " y -> " << y << std::endl;
                                if (pos) {
                                    pos->position.x = x;
                                    pos->position.y = y;
                                }
                            }
                    }

                    if (messageType == Serializer::MessageType::END)
                        break;
                }
            }
        }
        std::string serverIp;
        uint16_t serverPort;
        NetworkManager networkManager;

        float coolDown; // Time passed
        float currentTime; // Current time
};