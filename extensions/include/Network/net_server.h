#pragma once
#include <thread>
#include <array>
#include <vector>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include "engine/globals.h"
#include <queue>
#include <Network/net_command.h>
#include <mutex>


namespace neko
{

const short SERVER_PORT = 12345;
const size_t MAX_CLIENT_NMB = 128;

struct ClientData
{
	sf::TcpSocket tcpSocket;
	std::thread thread;
	std::queue<std::shared_ptr<NetCommand>> sentCommands;
	std::queue<sf::Packet> receivedCommands;
	std::mutex commandMutex;
	std::condition_variable commandSync;
};

class Server
{
public:
	void Start();
	void SendReliable(const std::shared_ptr<NetCommand> command, Index clientId);
	void SendUnreliable(const NetCommand& command);
protected:
	void ServerLoop();
	void TcpSocketLoop(Index clientId);
	virtual void ParseCommand(const sf::Packet& packet);


	std::thread serverThread;
    sf::TcpListener listener_;
	std::array<ClientData, MAX_CLIENT_NMB> clients_;
    bool isRunning = true;
	Index currentIndex = 0;
};
}
