#pragma once

#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>

namespace neko
{

const short SERVER_PORT = 12345;

class Server
{
public:
protected:
    sf::TcpListener listener_;
    bool isRunning = true;
};
}