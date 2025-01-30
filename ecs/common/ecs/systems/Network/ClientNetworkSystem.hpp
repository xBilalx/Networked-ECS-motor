#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"

class ClientNetworkSystem {
    public:
        ClientNetworkSystem(std::string serverIp, uint16_t serverPort, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort), coolDown(coolDown), currentTime(0) {}
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }

        void dataToServer(Scene& em, InputSystem& inputSystem, float dt) {

            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    currentTime = 0;
                } else {
                    return;
                }
            }

            if (inputSystem.inputPress) {
                inputSystem.inputPress = false;
                for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                    std::string buffer;
                    InputComponent* input = em.getComponent<InputComponent>(it->first);
                    PositionComponent* position = em.getComponent<PositionComponent>(it->first);
                    if (input && position) {
                        // std::cout << "BEFORE SEND\n";
                        Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                        Serializer::serialize(buffer, (uint64_t)it->first);
                        Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                        Serializer::serialize(buffer, (float) position->position.x);
                        Serializer::serialize(buffer, (float) position->position.y);
                        Serializer::serialize(buffer, Serializer::MessageType::END);
                        networkManager.sendTo(buffer, serverIp, serverPort);
                    }

                }
            }
        }

        // Process of deserialization and Creation/Modification of entity/component
        void dataFromServer(Scene& em) {

            std::vector<Packet> packets = networkManager.receiveMessages(false);
            for (Packet packet : packets) {
                Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
                while (1) {
                    messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                    if (messageType == Serializer::MessageType::END)
                        break;
                    if (messageType == Serializer::MessageType::CONNECTED) {
                        std::cout << "Connected to server successful" << std::endl;
                    }
                    if (messageType == Serializer::MessageType::CLEAR) {
                        std::cout << "FLAG CLEAR RECU\n";
                        em.clear();
                    }
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                        if (em.checkIfEntityExist(entityNbr)) {
                        } else {
                            em.createEntityWithId(entityNbr);
                            std::cout << "entité pas trouvé, entité crée du coup\n";
                        }
                        while (1) {
                            messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<Serializer::MessageType>(packet.data));
                            if (messageType == Serializer::MessageType::END || messageType == Serializer::MessageType::NEXT)
                                break;
                            if (messageType == Serializer::MessageType::RENDER) {
                                std::string pathImg = Serializer::deserializeString(packet.data);
                                RenderComponent* render = em.getComponent<RenderComponent>(entityNbr);
                                if (!render) {
                                    em.addComponent<RenderComponent>(entityNbr, pathImg, true);
                                } else {
                                    // std::cout << "Il faut modifier le render\n";
                                }
                            }
                            if (messageType == Serializer::MessageType::INPUT) {
                                InputComponent* render = em.getComponent<InputComponent>(entityNbr);
                                if (!render) {
                                    em.addComponent<InputComponent>(entityNbr);
                                } else {
                                    // std::cout << "Il faut modifier le render\n";
                                }
                            }
                            if (messageType == Serializer::MessageType::POSITION) {
                                sf::Vector2f pos;
                                float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                PositionComponent* input = em.getComponent<PositionComponent>(entityNbr);
                                if (!input) {
                                    em.addComponent<PositionComponent>(entityNbr, x, y);
                                } else {
                                    input->position.x = x;
                                    input->position.y = y;
                                }
                            }
                        }
                    }
                    if (messageType == Serializer::MessageType::END)
                        break;
                }
                free((void *)packet.ptr);
            }
        }
        void test() {
            std::string buffer;
            Serializer::serialize(buffer, Serializer::MessageType::CONNECT);
            Serializer::serialize(buffer, Serializer::MessageType::END);
            networkManager.sendTo(buffer, serverIp, serverPort);
        }
    private:
        std::string serverIp;
        uint16_t serverPort;
        NetworkManager networkManager;

        float coolDown; // Time passed
        float currentTime; // Current time
        float currentTimeSend;
};