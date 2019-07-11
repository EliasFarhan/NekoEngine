#pragma once
#include <engine/globals.h>
#include <SFML/Network/Packet.hpp>

namespace neko
{
using NetCommandType = sf::Uint32;
const NetCommandType INVALID_NET_COMMAND = 0u;


class NetCommand
{
public:
	NetCommand();
	NetCommandType netCommandType = 0u;
};


}
inline sf::Packet& operator <<(sf::Packet& packet, const neko::NetCommand& command)
{
	return packet << command.netCommandType;
}

inline sf::Packet& operator >>(sf::Packet& packet, neko::NetCommand& command)
{
	return packet >> command.netCommandType;
}