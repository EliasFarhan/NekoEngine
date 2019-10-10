#pragma once
#include <atomic>
#include <mutex>
#include <queue>

#include <SFML/Network/TcpSocket.hpp>

#include "net_command.h"
#include "net_server.h"

namespace neko
{

class Client
{
public:
	void Start();
	void Stop();
	void Update();
	void SendReliable(std::shared_ptr<NetCommand> command);
protected:
	void TcpReceiveSocketLoop();
	void TcpSendSocketLoop();
    virtual void ParseCommand(sf::Packet& packet) = 0;

	std::atomic<bool> isRunning;

	ClientData clientData;

};
}
