/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include "asteroid_net/network_server.h"
#include "engine/conversion.h"

#include <fmt/format.h>
namespace neko::net
{
void ServerNetworkManager::SendReliablePacket(
	std::unique_ptr<asteroid::Packet> packet)
{
	logDebug("[Server] Sending TCP packet: " +
		std::to_string(static_cast<int>(packet->packetType)));
	for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb;
		playerNumber++)
	{
		sf::Packet sendingPacket;
		GeneratePacket(sendingPacket, *packet);

		auto status = sf::Socket::Partial;
		while (status == sf::Socket::Partial)
		{
			status = tcpSockets_[playerNumber].send(sendingPacket);
			switch(status)
			{
			case sf::Socket::NotReady:
				logDebug(fmt::format(
					"[Server] Error trying to send packet to Player: {} socket is not ready",
					playerNumber));
				break;
			case sf::Socket::Disconnected:

				break;
			}
		}
	}
}

void ServerNetworkManager::SendUnreliablePacket(
	std::unique_ptr<asteroid::Packet> packet)
{
	for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb;
		playerNumber++)
	{
		if (clientMap_[playerNumber].udpRemotePort == 0)
			continue;


		sf::Packet sendingPacket;
		GeneratePacket(sendingPacket, *packet);
		const auto status = udpSocket_.send(sendingPacket, clientMap_[playerNumber].udpRemoteAddress,
			clientMap_[playerNumber].udpRemotePort);
		switch (status)
		{
		case sf::Socket::Done:
			//logDebug("[Server] Sending UDP packet: " +
				//std::to_string(static_cast<int>(packet->packetType)));
			break;

		case sf::Socket::Disconnected:
		{
			logDebug("[Server] Error while sending UDP packet, DISCONNECTED");
			break;
		}
		case sf::Socket::NotReady:
			logDebug("[Server] Error while sending UDP packet, NOT READY");

			break;

		case sf::Socket::Error:
			logDebug("[Server] Error while sending UDP packet, DISCONNECTED");
			break;
		default:
			break;
		}

	}

}

void ServerNetworkManager::Init()
{
	sf::Socket::Status status = sf::Socket::Error;
	while (status != sf::Socket::Done)
	{
		// bind the listener to a port
		status = tcpListener_.listen(tcpPort_);
		if (status != sf::Socket::Done)
		{
			tcpPort_++;
		}
	}
	tcpListener_.setBlocking(false);
	for (auto& socket : tcpSockets_)
	{
		socket.setBlocking(false);
	}
	logDebug(fmt::format("[Server] Tcp Socket on port: {}", tcpPort_));

	status = sf::Socket::Error;
	while (status != sf::Socket::Done)
	{
		status = udpSocket_.bind(udpPort_);
		if (status != sf::Socket::Done)
		{
			udpPort_++;
		}
	}
	udpSocket_.setBlocking(false);
	logDebug(fmt::format("[Server] Udp Socket on port: {}", udpPort_));
	gameManager_.Init();
	status_ = status_ | OPEN;

}

void ServerNetworkManager::Update(seconds dt)
{
	if (lastSocketIndex_ < asteroid::maxPlayerNmb)
	{
		const sf::Socket::Status status = tcpListener_.accept(
			tcpSockets_[lastSocketIndex_]);
		if (status == sf::Socket::Done)
		{
			const auto remoteAddress = tcpSockets_[lastSocketIndex_].
				getRemoteAddress();
			logDebug(fmt::format("[Server] New player connection with address: {} and port: {}", 
				remoteAddress.toString(), tcpSockets_[lastSocketIndex_].getRemotePort()));
			status_ = status_ | (FIRST_PLAYER_CONNECT << lastSocketIndex_);
		    lastSocketIndex_++;
		}
	}

	for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb;
		playerNumber++)
	{
		sf::Packet tcpPacket;
		const auto status = tcpSockets_[playerNumber].receive(
			tcpPacket);
		switch(status)
		{
		case sf::Socket::Done:
			ReceivePacket(tcpPacket, PacketSocketSource::TCP);
			break;
		case sf::Socket::Disconnected:
		{
			logDebug(fmt::format(
				"[Error] Player Number {} is disconnected when receiving",
				playerNumber + 1));
			status_ = status_ & ~(FIRST_PLAYER_CONNECT << playerNumber);
			auto endGame = std::make_unique<asteroid::WinGamePacket>();
			SendReliablePacket(std::move(endGame));
			status_ = status_ & ~OPEN; //Close the server
			break;
		}
		default: break;
		}
	}
	sf::Packet udpPacket;
	sf::IpAddress address;
	unsigned short port;
	const auto status = udpSocket_.receive(udpPacket, address, port);
	if (status == sf::Socket::Done)
	{
		ReceivePacket(udpPacket, PacketSocketSource::UDP, address, port);
	}
	gameManager_.Update(dt);
}

void ServerNetworkManager::Destroy()
{

}

void ServerNetworkManager::SetTcpPort(unsigned short i)
{
	tcpPort_ = i;
}

bool ServerNetworkManager::IsOpen()
{
	return status_& OPEN;
}

void ServerNetworkManager::ProcessReceivePacket(
	std::unique_ptr<asteroid::Packet> packet,
	PacketSocketSource packetSource,
	sf::IpAddress address,
	unsigned short port)
{
	const auto packetType = static_cast<asteroid::PacketType>(packet->packetType);
	switch (packetType)
	{
	case asteroid::PacketType::JOIN:
	{
		const auto* joinPacket = static_cast<asteroid::JoinPacket*>(packet.get());
		auto clientId = ConvertFromBinary<ClientId>(joinPacket->clientId);
		logDebug(fmt::format("[Server] Received Join Packet from: {} {}", clientId, 
			(packetSource == PacketSocketSource::UDP ? fmt::format(" UDP with port: {}", port) : " TCP")));
		const auto it = std::find_if(clientMap_.begin(), clientMap_.end(),
			[&clientId](const auto& clientInfo)
			{
				return clientInfo.clientId == clientId;
			});
		PlayerNumber playerNumber;
		if (it != clientMap_.end())
		{
			playerNumber = std::distance(clientMap_.begin(), it);
			clientMap_[playerNumber].udpRemoteAddress = address;
			clientMap_[playerNumber].udpRemotePort = port;
		}
		else
		{
			playerNumber = lastPlayerNumber_;
		}
		auto joinAckPacket = std::make_unique<asteroid::JoinAckPacket>();
		joinAckPacket->clientId = ConvertToBinary(clientId);
		joinAckPacket->udpPort = ConvertToBinary(udpPort_);
		if (packetSource == PacketSocketSource::UDP)
		{
		    auto& clientInfo = clientMap_[playerNumber];
			clientInfo.udpRemoteAddress = address;
			clientInfo.udpRemotePort = port;
			SendUnreliablePacket(std::move(joinAckPacket));
		}
		else
		{
			SendReliablePacket(std::move(joinAckPacket));
		}
		if (it != clientMap_.end())
		{
			//Player joined twice!
			return;
		}
		//Calculate time difference
		const auto clientTime = ConvertFromBinary<unsigned long>(joinPacket->startTime);
        using namespace std::chrono;
		const unsigned long deltaTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) - clientTime;
		logDebug(fmt::format("Client Server deltaTime: ", deltaTime));
		clientMap_[lastPlayerNumber_] = {
			clientId,
			deltaTime,
			"",
			static_cast<unsigned short>(0u) };
		//Spawning the new player in the arena
		for (PlayerNumber p = 0; p <= lastPlayerNumber_; p++)
		{
			auto spawnPlayer = std::make_unique<asteroid::SpawnPlayerPacket>();
			spawnPlayer->packetType = asteroid::PacketType::SPAWN_PLAYER;
			spawnPlayer->clientId = ConvertToBinary(clientMap_[p].clientId);
			spawnPlayer->playerNumber = p;

			const auto pos = asteroid::spawnPositions[p] * 3.0f;
			spawnPlayer->pos = ConvertToBinary(pos);

			const auto rotation = asteroid::spawnRotations[p];
			spawnPlayer->angle = ConvertToBinary(rotation);
			gameManager_.SpawnPlayer(p, pos, rotation);

			SendReliablePacket(std::move(spawnPlayer));
		}
		
		lastPlayerNumber_++;
		//Starting the game when both players are connected
		if (lastPlayerNumber_ == asteroid::maxPlayerNmb)
		{
			auto startGamePacket = std::make_unique<asteroid::StartGamePacket>();
			startGamePacket->packetType = asteroid::PacketType::START_GAME;
			using namespace std::chrono;
			const unsigned long ms = (duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
				) + milliseconds(3000)).count();

			startGamePacket->startTime = ConvertToBinary(ms);
			SendReliablePacket(std::move(startGamePacket));
			status_ = status_ | STARTED;
		}

		break;
	}
	case asteroid::PacketType::SPAWN_PLAYER:
		break;
	case asteroid::PacketType::INPUT:
	{
		//Manage internal state
		const auto* playerInputPacket = static_cast<const
			asteroid::PlayerInputPacket*>(packet.get());
		const auto playerNumber = playerInputPacket->playerNumber;
		const auto inputFrame = ConvertFromBinary<Frame>(playerInputPacket->currentFrame);
		for (Frame i = 0; i < playerInputPacket->inputs.size(); i++)
		{
			gameManager_.SetPlayerInput(playerNumber,
				playerInputPacket->inputs[i],
				inputFrame - i);
			if (inputFrame - i == 0)
			{
				break;
			}
		}
		//Send input packet to everyone
		SendUnreliablePacket(std::move(packet));

		//Validate new frame if needed
		Frame lastReceiveFrame = gameManager_.GetRollbackManager().
			GetLastReceivedFrame(0);
		for (PlayerNumber i = 1; i < asteroid::maxPlayerNmb; i++)
		{
			const auto playerLastFrame = gameManager_.GetRollbackManager().
				GetLastReceivedFrame(i);
			if (playerLastFrame < lastReceiveFrame)
			{
				lastReceiveFrame = playerLastFrame;
			}
		}
		if (lastReceiveFrame > gameManager_.GetLastValidateFrame())
		{
			//Validate frame
			gameManager_.Validate(lastReceiveFrame);

			auto validatePacket = std::make_unique<asteroid::ValidateFramePacket>();
			validatePacket->newValidateFrame = ConvertToBinary(lastReceiveFrame);
			
			//copy physics state
			for (PlayerNumber i = 0; i < asteroid::maxPlayerNmb; i++)
			{
				auto physicsState = gameManager_.GetRollbackManager().
					GetValidatePhysicsState(i);
				const auto* statePtr = reinterpret_cast<const std::uint8_t*>(&
					physicsState);
				for (size_t j = 0; j < sizeof(asteroid::PhysicsState); j++)
				{
					validatePacket->physicsState[i * sizeof(asteroid::PhysicsState) + j] =
						statePtr[j];
				}
			}
			SendUnreliablePacket(std::move(validatePacket));
			const auto winner = gameManager_.CheckWinner();
			if(winner != INVALID_PLAYER)
            {
			    logDebug(fmt::format("Server declares P{} a winner", winner+1));
			    auto winGamePacket = std::make_unique<asteroid::WinGamePacket>();
			    winGamePacket->winner = winner;
			    SendReliablePacket(std::move(winGamePacket));
			    gameManager_.WinGame(winner);
				status_ = status_ & ~OPEN; // close the server
            }
		}
		break;
	}

	case asteroid::PacketType::SPAWN_BULLET:
		break;
	case asteroid::PacketType::VALIDATE_STATE:
		break;
	case asteroid::PacketType::START_GAME:
		break;
	default:;
	}
}

void ServerNetworkManager::ReceivePacket(sf::Packet& packet,
	PacketSocketSource packetSource,
	sf::IpAddress address,
	unsigned short port)
{
	auto receivedPacket = asteroid::GenerateReceivedPacket(packet);

	if (receivedPacket != nullptr)
	{
		ProcessReceivePacket(std::move(receivedPacket), packetSource, address, port);
	}
}
}
