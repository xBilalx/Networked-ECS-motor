#pragma once
#include <string>
#include <cstring>
#include <cstdint>
#include "./NetworkManager.hpp"
#include "../../components/Input/InputComponent.hpp"
#include "../../components/Transform/PositionComponent.hpp"
#include "../Input/KeyboardInputSystem.hpp"
class ClientNetworkSystem
{
public:
    ClientNetworkSystem(std::string serverIp, uint16_t serverPort, bool managePos_, float coolDown = 0) : serverIp(serverIp), serverPort(serverPort),  managePos(managePos_) ,coolDown(coolDown), currentTime(0) {}
    bool createSocket()
    {
        
        networkManager = NetworkManager();
        return true;
    }


    void dataToServer(Scene &em, float dt)
    {

        if (coolDown != 0)
        {
            currentTime += dt;
            if (currentTime >= coolDown)
            {
                currentTime = 0;
            }
            else
            {
                return;
            }
        }
        if (managePos)
        {
            for (auto it = em.entities1.begin(); it != em.entities1.end(); it++)
            {
                std::string buffer;
                InputComponent *input = em.getComponent<InputComponent>(it->first);
                PositionComponent *position = em.getComponent<PositionComponent>(it->first);
                if (input && position)
                {
                    Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                    Serializer::serialize(buffer, (uint64_t)it->first);
                    Serializer::serialize(buffer, Serializer::MessageType::POSITION);
                    Serializer::serialize(buffer, (float)position->position.x);
                    Serializer::serialize(buffer, (float)position->position.y);
                    Serializer::serialize(buffer, Serializer::MessageType::END);
                    networkManager.sendTo(buffer, serverIp, serverPort);
                }
            }
        }
        else
        {
            // ICI
            for (auto it = em.entities1.begin(); it != em.entities1.end(); it++)
            {
                std::string buffer;
                InputComponent *input = em.getComponent<InputComponent>(it->first);
                if (input)
                {
                    Serializer::serialize(buffer, Serializer::MessageType::ENTITY);
                    Serializer::serialize(buffer, (uint64_t)it->first);
                    Serializer::serialize(buffer, Serializer::MessageType::INPUT);
                    input->serialize(buffer);
                    Serializer::serialize(buffer, Serializer::MessageType::END);
                    networkManager.sendTo(buffer, serverIp, serverPort);
                }
            }
        }
    }

    // Process of deserialization and Creation/Modification of entity/component
    void dataFromServer(Scene &em)
    {

        std::vector<Packet> packets = networkManager.receiveMessages();
        for (Packet packet : packets)
        {
            Serializer::MessageType messageType = Serializer::MessageType::NOTHING;
            while (1)
            {
                messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<uint8_t>(packet.data));
                if (messageType == Serializer::MessageType::END)
                    break;
                if (messageType == Serializer::MessageType::CONNECTED)
                {
                    std::cout << "Connected to server successful" << std::endl;
                }
                if (messageType == Serializer::MessageType::CLEAR)
                {
                    std::cout << "FLAG CLEAR RECU\n";
                    em.clear();
                }
                if (messageType == Serializer::MessageType::ENTITY)
                {
                    uint64_t entityNbr = static_cast<uint64_t>(Serializer::deserialize<uint64_t>(packet.data));
                    if (em.checkIfEntityExist(entityNbr))
                    {
                    }
                    else
                    {
                        em.createEntityWithId(entityNbr);
                        std::cout << "entité pas trouvé, entité crée du coup\n";
                    }
                    while (1)
                    {
                        messageType = static_cast<Serializer::MessageType>(Serializer::deserialize<Serializer::MessageType>(packet.data));
                        if (messageType == Serializer::MessageType::END || messageType == Serializer::MessageType::NEXT)
                            break;
                        if (messageType == Serializer::MessageType::RENDER)
                        {
                            std::string pathImg = Serializer::deserializeString(packet.data);
                            RenderComponent *render = em.getComponent<RenderComponent>(entityNbr);
                            if (!render)
                            {
                                RenderComponent &renderComponent = em.addComponent<RenderComponent>(entityNbr, pathImg, true);

                                sf::Vector2u textureSize = renderComponent.texture.getSize();
                                if (textureSize.x == 0 || textureSize.y == 0)
                                {
                                    std::cerr << "Erreur: La texture n'a pas été chargée correctement !" << std::endl;
                                    return;
                                }

                                float scaleFactor = 80.0f / static_cast<float>(textureSize.x);
                                renderComponent.sprite.setScale(scaleFactor, scaleFactor);
                                renderComponent.sprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
                            }
                            else
                            {
                                if (render->pathTexture != pathImg)
                                {
                                    render->pathTexture = pathImg;
                                    render->loadTecture();
                                }
                            }
                        }
                        if (messageType == Serializer::MessageType::TOKEN)
                        {
                            TokenComponent *token = em.getComponent<TokenComponent>(entityNbr);
                            float playerId = Serializer::deserialize<int>(packet.data);
                            if (!token)
                            {
                                em.addComponent<TokenComponent>(entityNbr, playerId);
                            }
                        }
                        if (messageType == Serializer::MessageType::RECTANGLE)
                        {
                            float x = Serializer::deserialize<float>(packet.data);
                            float y = Serializer::deserialize<float>(packet.data);
                            float width = Serializer::deserialize<float>(packet.data);
                            float height = Serializer::deserialize<float>(packet.data);

                            uint8_t r = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t g = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t b = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t a = Serializer::deserialize<uint8_t>(packet.data);

                            sf::Color color(r, g, b, a);

                            RectangleComponent *rect = em.getComponent<RectangleComponent>(entityNbr);
                            if (!rect)
                            {
                                em.addComponent<RectangleComponent>(entityNbr, x, y, width, height, color);
                            }
                            else
                            {
                                rect->x = x;
                                rect->y = y;
                                rect->width = width;
                                rect->height = height;
                                rect->color = color;
                            }
                        }
                        if (messageType == Serializer::MessageType::CIRCLE) {
                            float x = Serializer::deserialize<float>(packet.data);
                            float y = Serializer::deserialize<float>(packet.data);
                            float radius = Serializer::deserialize<float>(packet.data);

                            uint8_t r = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t g = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t b = Serializer::deserialize<uint8_t>(packet.data);
                            uint8_t a = Serializer::deserialize<uint8_t>(packet.data);

                            sf::Color color(r, g, b, a);

                            CircleComponent* circle = em.getComponent<CircleComponent>(entityNbr);
                            if (!circle) {
                                em.addComponent<CircleComponent>(entityNbr, x, y, radius, color);
                            } else {
                                circle->circle.setPosition(x, y);
                                circle->circle.setRadius(radius);
                                circle->circle.setFillColor(color);
                            }
                        }
                        if (messageType == Serializer::MessageType::INPUT)
                        {
                            InputComponent *render = em.getComponent<InputComponent>(entityNbr);
                            if (!render)
                            {
                                em.addComponent<InputComponent>(entityNbr);
                            }
                        }
                        if (messageType == Serializer::MessageType::POSITION)
                        {
                            sf::Vector2f pos;
                            float x = static_cast<float>(Serializer::deserialize<float>(packet.data));
                            float y = static_cast<float>(Serializer::deserialize<float>(packet.data));
                            PositionComponent *input = em.getComponent<PositionComponent>(entityNbr);
                            if (!input)
                            {
                                em.addComponent<PositionComponent>(entityNbr, x, y);
                            }
                            else
                            {
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
    void test()
    {
        std::string buffer;
        Serializer::serialize(buffer, Serializer::MessageType::CONNECT);
        Serializer::serialize(buffer, Serializer::MessageType::END);
        networkManager.sendTo(buffer, serverIp, serverPort);
    }

private:
    std::string serverIp;
    uint16_t serverPort;
    NetworkManager networkManager;

    float coolDown;    // Time passed
    float currentTime; // Current time
    float currentTimeSend;

    bool managePos = false;
};