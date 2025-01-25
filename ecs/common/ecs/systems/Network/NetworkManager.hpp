#pragma once
#include <cstring>
#include <stdexcept>
#include <vector>
#include <iostream>
#include "./Serializer.hpp"
#ifdef _WIN32
#include <mstcpip.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
#include <sys/socket.h>
#include <unistd.h>
#endif

struct Packet {
    std::string senderIp;
    uint16_t senderPort;
    const char *data;
    const char *ptr;
    Packet(std::string senderIp, uint16_t senderPort, const char *data) : senderIp(senderIp), senderPort(senderPort), data(data), ptr(data) {}
};

class NetworkManager
{
private:
    int m_sockfd;
    bool m_is_initialized;

#ifdef _WIN32
    WSADATA wsaData;
#endif

public:
    NetworkManager() : m_is_initialized(false)
    {
#ifdef _WIN32
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            throw std::runtime_error("Failed to initialize Winsock");
        }
#endif
        m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (m_sockfd < 0)
        {
            cleanup();
            throw std::runtime_error("Failed to create socket");
        }
        m_is_initialized = true;
    }

    ~NetworkManager()
    {
        if (m_is_initialized)
        {
            cleanup();
        }
    }

    // Only for server
    void bindSocket(const std::string &ip, uint16_t port)
    {
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(ip.c_str());

        if (bind(m_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        {
            throw std::runtime_error("Failed to bind socket");
        }
    }
    void sendTo(std::string &buffer, std::string Ip, uint16_t Port)
    {
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        addr.sin_family = AF_INET;
        addr.sin_port = htons(Port);
        inet_pton(AF_INET, Ip.c_str(), &addr.sin_addr);
        sendto(m_sockfd, buffer.data(), buffer.size(), 0, (const struct sockaddr *)&addr, len);
    }

    std::vector<Packet> receiveMessages(bool test)
    {
        std::vector<Packet> packets;
        char data[1024];
        std::size_t received;
        struct sockaddr_in senderAddr;
        socklen_t senderLen = sizeof(senderAddr);

        while (true)
        {
            ssize_t receivedBytes =
                recvfrom(m_sockfd, data, sizeof(data), MSG_DONTWAIT, (struct sockaddr *)&senderAddr, &senderLen);
            if (receivedBytes < 0)
            {
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    break;
                }
                throw std::runtime_error("Failed to receive data");
            }
            char senderIp[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &senderAddr.sin_addr, senderIp, INET_ADDRSTRLEN);
            uint16_t senderPort = ntohs(senderAddr.sin_port);
            std::cout << receivedBytes << std::endl;
            char *buffer = (char *) malloc(sizeof(char) * receivedBytes + 1);
            memcpy(buffer, data, receivedBytes);
            packets.push_back(Packet(std::string(senderIp), senderPort, buffer));
            std::cout << "---------------> " << std::string(senderIp) << std::endl;
        }
        return packets;
    }

private:
    void cleanup()
    {
#ifdef _WIN32
        closesocket(m_sockfd);
        WSACleanup();
#else
        close(m_sockfd);
#endif
        m_is_initialized = false;
    }
};