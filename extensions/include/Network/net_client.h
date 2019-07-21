#pragma once
#include <atomic>
#include <mutex>
#include <queue>

#include <SFML/Network/TcpSocket.hpp>

#include "Network/net_command.h"
#include "Network/net_server.h"

namespace neko
{

class Client
{
public:
	void Start();
	void Stop();
	void SendReliable(std::shared_ptr<NetCommand> command);
protected:
	void TcpSocketLoop();

	std::atomic<bool> isRunning;

	ClientData clientData;

};
}
