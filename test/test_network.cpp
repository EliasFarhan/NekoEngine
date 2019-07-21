//
// Created by efarhan on 13.07.19.
//

#include <gtest/gtest.h>
#include <Network/net_server.h>
#include <Network/net_client.h>
#include <Network/net_command.h>
#include "engine/log.h"
#include "SFML/Window/Keyboard.hpp"

const size_t clientNmb = 4;

enum class HelloCommandType : neko::NetCommandType
{
	HELLO
};

class HelloServer : public neko::Server
{
protected:
	void ParseCommand(sf::Packet& packet, const neko::Index clientId) override
	{
		auto command = std::make_shared<neko::NetCommand>();
		packet >> command;
		switch (HelloCommandType(command->netCommandType))
		{
		case HelloCommandType::HELLO:
			logDebug("Hello from " + std::to_string(clientId));
			auto newCommand = std::make_shared<neko::NetCommand>();
			newCommand->netCommandType = neko::NetCommandType(HelloCommandType::HELLO);
			SendReliable(newCommand, clientId);
			break;
		}

	}

};

class HelloClient : public neko::Client
{
	
};

TEST(Network, Hello)
{
	HelloServer server;
	server.Start();

	std::array<HelloClient, clientNmb> clients = {};
	
	while(1)
	{
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			using namespace std::chrono_literals;
			server.Stop();
			std::this_thread::sleep_for(16.67ms);
		}
	}
	
}