#pragma once
#include <thread>
#include <array>
#include <atomic>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include "engine/globals.h"
#include <queue>
#include "net_command.h"
#include <mutex>
#include <condition_variable>


namespace neko
{

const std::string SERVER_IP = "localhost";
const unsigned short SERVER_PORT = 53'000;

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
	virtual ~Server() = default;
	void Start();
	void Stop();
	void Update();
	void SendReliable(const std::shared_ptr<NetCommand> command, Index clientId);
	void SendUnreliable(const NetCommand& command);
protected:
	void ServerLoop();
	void TcpSendSocketLoop(const Index clientId);
	void TcpReceiveSocketLoop(const Index clientId);
	virtual void ParseCommand(sf::Packet& packet, const Index clientId) = 0;


	std::thread serverThread;
    sf::TcpListener listener_;
	std::array<ClientData, MAX_CLIENT_NMB> clients_;
    std::atomic<bool> isRunning = true;
	Index currentIndex = 0;
};
}
