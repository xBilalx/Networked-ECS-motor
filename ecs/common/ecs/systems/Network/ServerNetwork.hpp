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
                // std::cout << "Nbr of clients : " <<  x << std::endl;
                bindClients.push_back(std::make_unique<BindClientsTest>());
            }
        }
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }
        void sendClearScene() {
            for (int currentBindId = 0; currentBindId < static_cast<int>(bindClients.size()); currentBindId++) {
                BindClientsTest* cli = bindClients[currentBindId].get();
                if (cli->connected) {
                    std::string buffer;
                    Serializer::serialize(buffer, Serializer::MessageType::CLEAR);
                    Serializer::serialize(buffer, Serializer::MessageType::END);
                    networkManager.sendTo(buffer, cli->ipClient, cli->portClient);
                }
            }
        }

        void lobbyToClient(Scene& em, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= 1) {
                    currentTime = 0;
                } else {
                    return;
                }
            }
            if (ready) {
                for (std::unique_ptr<BindClientsTest>& bindClient : bindClients) {
                    if (bindClient->connected) {
                        for (int i = 0; i < 10; i++) { // On envoie 10x on sait jamais !
                            std::string buffer;
                            Serializer::serialize(buffer, Serializer::MessageType::RUN);
                            Serializer::serialize(buffer, Serializer::MessageType::END);
                            networkManager.sendTo(buffer, bindClient->ipClient, bindClient->portClient);
                        }
                    }
                }
                std::cout << "RUN envoyé\n";
                run = true;
                return;
            }
            for (std::unique_ptr<BindClientsTest>& bindClient : bindClients) {
                if (bindClient->connected) {
                    std::string buffer;
                    Serializer::serialize(buffer, Serializer::MessageType::WAIT);
                    Serializer::serialize(buffer, (uint8_t) nbrOfClients);
                    Serializer::serialize(buffer, Serializer::MessageType::END);
                    networkManager.sendTo(buffer, bindClient->ipClient, bindClient->portClient);
                }
            }
        }

        void dataToClients(Scene& em, float dt) {
            if (!run) {
                lobbyToClient(em, dt);
                return;
            }

            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    currentTime = 0;
                } else {
                    return;
                }
            }
            for (int currentBindId = 0; currentBindId < static_cast<int>(bindClients.size()); currentBindId++) {
                BindClientsTest* cli = bindClients[currentBindId].get();
                if (cli->connected) {
                    for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                        std::string buffer;
                        RectangleComponent* rect = em.getComponent<RectangleComponent>(it->first);
                        InputComponent* input = em.getComponent<InputComponent>(it->first);
                        SpriteComponent* sprite = em.getComponent<SpriteComponent>(it->first);  // ⚠️ Sprite
                        RenderComponent* render = em.getComponent<RenderComponent>(it->first);
                        PositionComponent* position = em.getComponent<PositionComponent>(it->first);
                        TokenComponent* token = em.getComponent<TokenComponent>(it->first);
                        CircleComponent* circle = em.getComponent<CircleComponent>(it->first);
                        BindClientComponentTest *bindClientTest = em.getComponent<BindClientComponentTest>(it->first);
                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)it->first);


                        if (render) {
                            Serializer::serialize(buffer, Serializer::MessageType::RENDERZ);
                            Serializer::serialize(buffer, (char)render->zIndex);
                        }
                        if (input && bindClientTest && currentBindId == bindClientTest->bindId) {
                            Serializer::serialize(buffer, Serializer::MessageType::INPUT);
                        }
                        if (circle) {
                            Serializer::serialize(buffer, Serializer::MessageType::CIRCLE);
                            Serializer::serialize(buffer, (float)circle->circle.getPosition().x);
                            Serializer::serialize(buffer, (float)circle->circle.getPosition().y);
                            Serializer::serialize(buffer, (float)circle->circle.getRadius());
                            Serializer::serialize(buffer, (uint8_t)circle->circle.getFillColor().r);
                            Serializer::serialize(buffer, (uint8_t)circle->circle.getFillColor().g);
                            Serializer::serialize(buffer, (uint8_t)circle->circle.getFillColor().b);
                            Serializer::serialize(buffer, (uint8_t)circle->circle.getFillColor().a);
                        }
                        if (sprite) {
                            Serializer::serialize(buffer, Serializer::MessageType::RENDER);
                            Serializer::serialize(buffer, (std::string) sprite->pathTexture);
                        }
                        if (rect) {
                            Serializer::serialize(buffer, Serializer::MessageType::RECTANGLE);
                            Serializer::serialize(buffer, (float)rect->x);
                            Serializer::serialize(buffer, (float)rect->y);
                            Serializer::serialize(buffer, (float)rect->width);
                            Serializer::serialize(buffer, (float)rect->height);
                            Serializer::serialize(buffer, (uint8_t)rect->color.r);
                            Serializer::serialize(buffer, (uint8_t)rect->color.g);
                            Serializer::serialize(buffer, (uint8_t)rect->color.b);
                            Serializer::serialize(buffer, (uint8_t)rect->color.a);
                        }
                        if (token) {
                            Serializer::serialize(buffer, Serializer::MessageType::TOKEN);
                            Serializer::serialize(buffer, (int)token->playerId);
                            // std::cout << "Send TOKEN\n";
                        }
                        if (position) {
                            // if (bindClientTest && !bindClientTest->ManagePosByServ &&currentBindId == bindClientTest->bindId && !bindClientTest->ManagePosByServ) {
                            //     continue;
                            // }
                            // if (bindClientTest && currentBindId == bindClientTest->bindId && bindClientTest->ManagePosByServ) {
                            //     bindClientTest->ManagePosByServ = false;
                            // }

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
            std::vector<Packet> packets = networkManager.receiveMessages();
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::READY) {
                        std::cout << "🚀 Flag Ready receive !\n";
                        // Ce serait cool de faire des vérifs avant de mettre la valeur en READY
                        ready = true;
                    }
                    if (messageType == Serializer::MessageType::CONNECT) {
                        for (int currentBindId = 0; currentBindId < static_cast<int>(bindClients.size()); currentBindId++) {
                            BindClientsTest* cli = bindClients[currentBindId].get();
                            if (!cli->connected) {
                                cli->ipClient = packet.senderIp;
                                cli->portClient = packet.senderPort;
                                cli->connected = true;
                                nbrOfClients++;
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
                            if (messageType == Serializer::MessageType::INPUT) {
                                InputComponent* input = em.getComponent<InputComponent>(entityNbr);
                                if (input) {
                                    input->deserialize(packet.data);
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
        float currentTime = 0;

        uint8_t nbrOfClients = 0;
        bool run = false; // Les données entités peuvent transité ?
        bool ready = false; // Flag Ready recu
};