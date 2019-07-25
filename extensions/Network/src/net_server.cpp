#include <Network/net_server.h>
#include <engine/log.h>
#include <engine/globals.h>
#include <utilities/vector_utility.h>
#include <algorithm>

namespace neko
{
void Server::Start()
{
	serverThread = std::thread(&Server::ServerLoop, this);
	serverThread.detach();
}

void Server::Stop()
{
	isRunning = false;
	for(auto& client : clients_)
	{
		if(client.tcpSocket.getRemoteAddress() != sf::IpAddress::None)
		{
			client.commandSync.notify_one();
		}
	}
}

void Server::SendReliable(const std::shared_ptr<NetCommand> command, Index clientId)
{
	auto& client = clients_[clientId];
	std::unique_lock<std::mutex> lock(client.commandMutex);
	client.sentCommands.push(command);
	client.commandSync.notify_one();
}

void Server::SendUnreliable(const NetCommand& command)
{
}

void Server::ServerLoop()
{
	if (listener_.listen(SERVER_PORT) == sf::Socket::Done)
	{
		while (isRunning || currentIndex == INVALID_INDEX)
		{
			if (listener_.accept(clients_[currentIndex].tcpSocket) != sf::Socket::Done)
			{
				logDebug("[Error] Server client id: " + std::to_string(currentIndex) + " could not connect");
				continue;
			}
			clients_[currentIndex].thread = std::thread(&Server::TcpSocketLoop, this, currentIndex);
			
			
			const auto clientDataResult = std::find_if(clients_.begin(), clients_.end(), [](const ClientData& client)
			{
				return client.tcpSocket.getRemoteAddress() == sf::IpAddress::None;
			});
			if(clientDataResult == clients_.end())
			{
				logDebug("[Error] Server: no more available client");
				currentIndex = INVALID_INDEX;
			}
			else
			{
				currentIndex = clientDataResult - clients_.begin();
			}
			
		}
	}
	else
	{
		logDebug("[Error] Server: Cannot link listener to port: " + std::to_string(SERVER_PORT));
	}
}

void Server::TcpSocketLoop(const Index clientId)
{
	while(isRunning)
	{
		std::unique_lock<std::mutex> lock(clients_[currentIndex].commandMutex);
		clients_[currentIndex].commandSync.wait(lock);
		if(!clients_[clientId].sentCommands.empty())
		{

			std::shared_ptr<neko::NetCommand> command;
			
			{ 
				auto& client = clients_[clientId];
				std::unique_lock<std::mutex> lock(client.commandMutex);
				command = clients_[clientId].sentCommands.front();
				clients_[clientId].sentCommands.pop();
			}
			sf::Packet sentPacket;
			//TODO: correctly transfer the data from the command with inheritance
			sentPacket << command;
			clients_[clientId].tcpSocket.send(sentPacket);

			sf::Packet receivedPacket;
			clients_[clientId].tcpSocket.receive(receivedPacket);
			clients_[clientId].receivedCommands.push(receivedPacket);
		}
	}

}

}
