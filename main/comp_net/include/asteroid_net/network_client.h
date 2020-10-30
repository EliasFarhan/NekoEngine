#pragma once

#include <asteroid/game_manager.h>
#include <gl/framebuffer.h>
#include "SFML/Network.hpp"
#include "asteroid/game.h"
#include "asteroid/packet_type.h"
#include "asteroid/client.h"

namespace neko::net
{

class ClientNetworkManager : public Client
{
public:
	enum class State
	{
		NONE,
		JOINING,
		JOINED,
		GAME_STARTING,
		GAME
		
	};
	enum class PacketSource
	{
		TCP,
		UDP
	};
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) override;

    void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) override;
	void SetPlayerInput(PlayerInput input);


private:
    void ReceivePacket(sf::Packet& packet, PacketSource source);
    sf::UdpSocket udpSocket_;
    sf::TcpSocket tcpSocket_;

    std::string serverAddress_ = "localhost";
    unsigned short serverTcpPort_ = 12345;
    unsigned short serverUdpPort_ = 0;


    State currentState_ = State::NONE;
};

}
