#pragma once
#include <engine/globals.h>
#include <SFML/Network/Packet.hpp>
#include <memory>

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
template <class T = neko::NetCommand>
sf::Packet& operator <<(sf::Packet& packet, const std::shared_ptr<T> command)
{
	return packet << command->netCommandType;
}
template <class T = neko::NetCommand>
sf::Packet& operator >>(sf::Packet& packet, std::shared_ptr <T> command)
{
	return packet >> command->netCommandType;
}