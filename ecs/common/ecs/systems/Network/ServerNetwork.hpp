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

struct BindClientsTest {
    public:
        std::string ipClient;
        unsigned short portClient;
        bool connected = false;
        // bool ManagePosByServ = true;
        BindClientsTest() {}
};

class ServerNetworkSystem {
    public:
        ServerNetworkSystem(std::string serverIp, uint16_t serverPort, unsigned int maxClient, float coolDown) : serverIp(serverIp), serverPort(serverPort), maxClient(maxClient), coolDown(coolDown) {
            networkManager.bindSocket(serverIp, serverPort);
            if (maxClient == 0) {
                std::cout << "0 clients set\n";
            }
            setXBindClient(maxClient);
        }
        void setXBindClient(int x) {
            maxClient = x;
            for (int i = 0; i < x;  i++) {
                std::cout << x << std::endl;
                bindClients.push_back(std::make_unique<BindClientsTest>());
            }
        }
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }
        void sendClearScene(Scene& em, float dt) {
            for (int currentBindId = 0; currentBindId < bindClients.size(); currentBindId++) {
                BindClientsTest* cli = bindClients[currentBindId].get();
                if (cli->connected) {
                    std::string buffer;
                    Serializer::serialize(buffer, Serializer::MessageType::CLEAR);
                    Serializer::serialize(buffer, Serializer::MessageType::END);
                    networkManager.sendTo(buffer, cli->ipClient, cli->portClient);
                }
            }
        }

        void dataToClients(Scene& em, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    currentTime = 0;
                } else {
                    return;
                }
            }
            
            int next = false;
            for (int currentBindId = 0; currentBindId < bindClients.size(); currentBindId++) {
                BindClientsTest* cli = bindClients[currentBindId].get();
                if (cli->connected) {
                    for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                        std::string buffer;
                        InputComponent* input = em.getComponent<InputComponent>(it->first);
                        RenderComponent* render = em.getComponent<RenderComponent>(it->first);
                        PositionComponent* position = em.getComponent<PositionComponent>(it->first);
                        BindClientComponent *bindClient = em.getComponent<BindClientComponent>(it->first);
                        BindClientComponentTest *bindClientTest = em.getComponent<BindClientComponentTest>(it->first);
                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)it->first);

                        if (input && bindClientTest && currentBindId == bindClientTest->bindId) {
                            Serializer::serialize(buffer, Serializer::MessageType::INPUT);
                        }
                        if (render) {
                            Serializer::serialize(buffer, Serializer::MessageType::RENDER);
                            Serializer::serialize(buffer, (std::string) render->pathTexture);
                        }
                        if (position) {
                            if (bindClientTest && currentBindId == bindClientTest->bindId && !bindClientTest->ManagePosByServ) {
                                continue;
                            }
                            if (bindClientTest && currentBindId == bindClientTest->bindId && bindClientTest->ManagePosByServ) {
                                bindClientTest->ManagePosByServ = false;
                            }
                            Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                            Serializer::serialize(buffer, (float) position->position.x);
                            Serializer::serialize(buffer, (float) position->position.y); 
                        }
                        Serializer::serialize(buffer, Serializer::MessageType::END);
                        networkManager.sendTo(buffer, cli->ipClient, cli->portClient);
                    }
                }
            }
        }

        void dataFromClients(Scene& em) {
            std::vector<Packet> packets = networkManager.receiveMessages(true);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::CONNECT) {
                        std::cout << bindClients.size() << std::endl;
                        for (int currentBindId = 0; currentBindId < bindClients.size(); currentBindId++) {
                            BindClientsTest* cli = bindClients[currentBindId].get();
                            if (!cli->connected) {
                                cli->ipClient = packet.senderIp;
                                cli->portClient = packet.senderPort;
                                cli->connected = true;
                                std::cout << "Connected to BIND Client to Server OK " << std::endl;
                                std::string buffer;
                                Serializer::serialize(buffer, Serializer::MessageType::CONNECTED);
                                Serializer::serialize(buffer, Serializer::MessageType::END);
                                networkManager.sendTo(buffer, packet.senderIp, packet.senderPort);
                                std::cout << "\033[1;32mConnexion Accepté pour Client " << packet.senderIp <<"\033[0m\n";
                                return;
                            }
                        }
                        std::cout << "\033[1;31mConnexion Refusé pour Client " << packet.senderIp <<"\033[0m\n";
                        return;
                    }
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));

                        BindClientComponentTest* bindClient = em.getComponent<BindClientComponentTest>(entityNbr);
                        if (bindClient) {
                            messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                            if (messageType == Serializer::MessageType::POSITION) {
                                BindClientsTest* cli = bindClients[bindClient->bindId].get(); // Faudrait vérifier ca, ca peut crash

                                if (bindClient && cli->ipClient == packet.senderIp && cli->portClient == packet.senderPort) {
                                    float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                    float y = static_cast<float>(Serializer::deserialize<float>(packet.data));

                                    PositionComponent* pos = em.getComponent<PositionComponent>(entityNbr);
                                    if (pos) {
                                        pos->position.x = x;
                                        pos->position.y = y;
                                    }
                                }
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
        std::vector<std::unique_ptr<BindClientsTest>> bindClients;

        int maxClient;
        float coolDown;
        float currentTime;
        float currentTimeReceiv;
};