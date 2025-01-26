#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"

class ClientNetworkSystem {
    public:
        ClientNetworkSystem(std::string serverIp, uint16_t serverPort, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort), coolDown(coolDown) {}
        bool createSocket() {
            networkManager = NetworkManager();
            return true;
        }

        void dataToServer(Scene& em, InputSystem& inputSystem, float dt) {
            if (coolDown != 0) {
                currentTime += dt;
                if (currentTime >= coolDown) {
                    // std::cout << currentTime << std::endl;
                    currentTime = 0;
                } else {
                    return;
                }
            }

            if (inputSystem.inputPress) {
                inputSystem.inputPress = false;
                for (auto it = em.entities1.begin(); it != em.entities1.end(); it++) {
                    std::cout << "c'est l'heure !\n";
                    std::string buffer;
                    InputComponent* input = em.getComponent<InputComponent>(it->first);
                    PositionComponent* position = em.getComponent<PositionComponent>(it->first);
                    if (!position) {
                        std::cout << "-------------------------------------------------Il ya pas pos !\n";
                    }
                    if (!input) {
                        std::cout << "-èèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèèè Il ya pas input !\n";
                    }
                    if (input && position) {
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
                        // ECS connection good
                        std::cout << "Connected to server successful" << std::endl;
                    }
                    // Creation of modification of entity
                    if (messageType == Serializer::MessageType::ENTITY) {
                        uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                        std::cout << "entity -> " << entityNbr << std::endl;
                        if (em.checkIfEntityExist(entityNbr)) {
                            // Créer l'entité avec son ID
                            // std::cout << "entité trouvé\n";
                        } else {
                            em.createEntityWithId(entityNbr);
                            std::cout << "entité pas trouvé, entité crée du coup\n";
                            // continue;
                        }
                        // std::unique_ptr<Entity>& entity = em.findEntity(entityNbr);
                        while (1) {
                            messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<Serializer::MessageType>(packet.data));
                            if (messageType == Serializer::MessageType::END || messageType == Serializer::MessageType::NEXT)
                                break;
                            // Component WINDOW
                            if (messageType == Serializer::MessageType::WINDOW) {
                                std::cout << "WINDOW\n";
                                unsigned int modeWidth = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                                unsigned int modeHeight = static_cast<unsigned int>(Serializer::deserialize<unsigned int>(packet.data));
                                std::cout << "modeWidth -> " << modeWidth << " modeHeight -> " << modeHeight << std::endl;
                                WindowComponent* win = em.getComponent<WindowComponent>(entityNbr);
                                if (!win) {
                                    std::cout << "ezfbuizibffieuzbfuizebiufbuifbiuezbfiuezbiufbzuief\n";
                                    em.addComponent<WindowComponent>(entityNbr, modeWidth, modeHeight);
                                }  
                            }

                            // Component RENDER
                            if (messageType == Serializer::MessageType::RENDER) {
                                std::cout << "RENDER\n";
                                std::string pathImg = Serializer::deserializeString(packet.data);
                                std::cout << "Pathimg -> " << pathImg << std::endl;
                                RenderComponent* render = em.getComponent<RenderComponent>(entityNbr);
                                if (!render) {
                                    em.addComponent<RenderComponent>(entityNbr, pathImg, true);
                                } else {
                                    std::cout << "Il faut modifier le render\n";
                                }
                            }

                            // Component INPUT
                            if (messageType == Serializer::MessageType::INPUT) {
                                std::cout << "INPUT\n";
                                InputComponent* render = em.getComponent<InputComponent>(entityNbr);
                                if (!render) {
                                    em.addComponent<InputComponent>(entityNbr);
                                } else {
                                    std::cout << "Il faut modifier le render\n";
                                }
                            }
                            //Component POS
                            if (messageType == Serializer::MessageType::POSITION) {
                                std::cout << "POSITION\n";
                                sf::Vector2f pos;
                                float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                                std::cout << "x -> " << x << " y -> " << y << std::endl;
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
};