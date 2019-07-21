#include <Network/net_client.h>
#include "SFML/Network/Socket.hpp"
#include "engine/log.h"

namespace neko
{
void Client::Start()
{
	clientData.thread = std::thread(&Client::TcpSocketLoop, this);
}

void Client::Stop()
{
	isRunning = false;
}

void Client::SendReliable(std::shared_ptr<NetCommand> command)
{
	std::unique_lock<std::mutex> lock(clientData.commandMutex);
	clientData.sentCommands.push(command);
	clientData.commandSync.notify_one();
}

void Client::TcpSocketLoop()
{
	const sf::Socket::Status status = clientData.tcpSocket.connect(SERVER_IP, SERVER_PORT);
	if (status != sf::Socket::Done)
	{
		logDebug("Could not connect to server at: " + SERVER_IP);
		return;
	}

	while(isRunning)
	{
		sf::Packet receivedPacket;
		clientData.tcpSocket.receive(receivedPacket);
		clientData.receivedCommands.push(receivedPacket);


	}
}
}
