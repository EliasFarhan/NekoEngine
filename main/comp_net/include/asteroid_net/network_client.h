#pragma once

#include <asteroid/game_manager.h>
#include <gl/framebuffer.h>
#include "SFML/Network.hpp"
#include "asteroid/game.h"
#include "asteroid/packet_type.h"

namespace neko::net
{

class ClientNetworkManager : public RenderProgram, public DrawImGuiInterface, public asteroid::PacketSenderInterface
{
public:
    ClientNetworkManager();
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) override;

    void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) override;

private:
    sf::UdpSocket udpSocket_;
    sf::TcpSocket tcpSocket_;

    std::string host_ = "localhost";
    unsigned short tcpPort_ = 12345;

    asteroid::ClientGameManager gameManager_;
    Vec2u windowSize_;
    ClientId clientId_ = 0;

};



}