#include "../include/network/net_client.h"
#include "SFML/Network/Socket.hpp"
#include "engine/log.h"

namespace neko
{
void Client::Start()
{
	clientData.thread = std::thread(&Client::TcpReceiveSocketLoop, this);
	clientData.thread.detach();
	isRunning = true;
}

void Client::Stop()
{
	isRunning = false;
}

void Client::SendReliable(std::shared_ptr<NetCommand> command)
{

    logDebug("[Client] Sent packet to server queue");
	clientData.sentCommands.push(command);
	clientData.commandSync.notify_one();
}

void Client::TcpReceiveSocketLoop()
{
	const sf::Socket::Status status = clientData.tcpSocket.connect(SERVER_IP, SERVER_PORT);
	if (status != sf::Socket::Done)
	{
		logDebug("Could not connect to server at: " + SERVER_IP);
		return;
	}
	logDebug("[Client] Successfully connected to server");

	while(isRunning)
	{
		sf::Packet receivedPacket;
		clientData.tcpSocket.receive(receivedPacket);
        {
            std::unique_lock<std::mutex> lock(clientData.commandMutex);
            clientData.receivedCommands.push(receivedPacket);

            logDebug("[Client] Received packet from server, put in queue");
        }
        bool isSentCommandEmpty;
        {
            //waiting to have something to send
            std::unique_lock<std::mutex> lock(clientData.commandMutex);
            clientData.commandSync.wait(lock);
            isSentCommandEmpty = clientData.sentCommands.empty();
        }
        if (!isSentCommandEmpty)
        {
            std::shared_ptr<neko::NetCommand> command;
            {
                std::unique_lock<std::mutex> lock(clientData.commandMutex);
                command = clientData.sentCommands.front();
                clientData.sentCommands.pop();
            }
            sf::Packet sentPacket;
            logDebug("[Client] Sent packet to server");
            sentPacket << command;
            clientData.tcpSocket.send(sentPacket);
        }
        else
        {
            logDebug("[Client] packet to send queue is empty, probably means exit!");
        }
	}
}

void Client::Update()
{
    std::unique_lock<std::mutex> lock(clientData.commandMutex, std::try_to_lock);

    if(lock.owns_lock())
    {
        while(!clientData.receivedCommands.empty())
        {
            logDebug("[Client] Received packet from server, about to parse");
            auto packet = clientData.receivedCommands.front();
            ParseCommand(packet);
            clientData.receivedCommands.pop();
        }
    }

}

void Client::TcpSendSocketLoop()
{

}
}
