#pragma once

#include <queue>
#include "SFML/Network.hpp"
#include "asteroid/game.h"
#include "asteroid/packet_type.h"
#include "asteroid/game_manager.h"

namespace neko::net
{
struct ClientInfo
{
    ClientId clientId = 0;
    unsigned long long timeDifference = 0;
    sf::IpAddress udpRemoteAddress;
    unsigned short udpRemotePort = 0;


};
class ServerNetworkManager : public asteroid::PacketSenderInterface, public SystemInterface
{
public:
    enum class PacketSocketSource
    {
        TCP,
        UDP
    };
    void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) override;

    void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void SetTcpPort(unsigned short i);

private:
    void ProcessReceivePacket(std::unique_ptr<asteroid::Packet> packet, 
        PacketSocketSource packetSource, 
        sf::IpAddress address = "localhost", 
        unsigned short port = 0);
    void ReceivePacket(sf::Packet& packet, PacketSocketSource packetSource,
        sf::IpAddress address = "localhost",
        unsigned short port = 0);

    sf::UdpSocket udpSocket_;
    sf::TcpListener tcpListener_;
    std::array<sf::TcpSocket, asteroid::maxPlayerNmb> tcpSockets_;

    std::array<ClientInfo, asteroid::maxPlayerNmb> clientMap_{};
    //Server game manager
    asteroid::GameManager gameManager_;

    unsigned short tcpPort_ = 12345;
    unsigned short udpPort_ = 12345;
    Index lastSocketIndex_ = 0;
    PlayerNumber lastPlayerNumber_ = 0;
    const std::array<Vec2f, std::max(4u, asteroid::maxPlayerNmb)> spawnPositions_{
            Vec2f(0,1),
            Vec2f(0,-1),
            Vec2f(1,0),
            Vec2f(-1,0),
    };
    const std::array<degree_t, std::max(4u, asteroid::maxPlayerNmb)> spawnRotations_{
            degree_t(0.0f),
            degree_t(180.0f),
            degree_t(-90.0f),
            degree_t(90.0f)
    };
};
}