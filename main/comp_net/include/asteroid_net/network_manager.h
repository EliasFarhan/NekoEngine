#pragma once
#include "SFML/Network.hpp"
#include "asteroid/game.h"
namespace neko::net
{

class NetworkManager
{
public:
	virtual ~NetworkManager() = default;
	virtual void SendReliable() = 0;
	virtual void SendUnreliable() = 0;
};

class ClientNetworkManager : public NetworkManager
{
public:
private:
    sf::UdpSocket udpSocket_;
	std::array<sf::TcpSocket, asteroid::maxPlayerNmb> tcpSockets_;
	
};

class ServerNetworkManager : public NetworkManager
{
public:
private:
	sf::UdpSocket udpSocket_;
	sf::TcpSocket tcpSocket_;
};

}