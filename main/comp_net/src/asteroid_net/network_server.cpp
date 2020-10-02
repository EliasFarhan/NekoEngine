//
// Created by efarhan on 9/30/20.
//

#include "asteroid_net/network_server.h"

namespace neko::net
{
void ServerNetworkManager::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    for(PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb; playerNumber++)
    {
        if (clientMap_[playerNumber].udpRemotePort == 0)
            continue;
        tcpPackets_.emplace_back();
        auto& sendingPacket = tcpPackets_.back();
        GeneratePacket(sendingPacket, *packet);
        auto status = sf::Socket::Partial;
        while(status == sf::Socket::Partial)
        {
            status = tcpSockets_[playerNumber].send(sendingPacket);
        }
        tcpPackets_.pop_back();
    }
}

void ServerNetworkManager::SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
	for(PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb; playerNumber++)
	{
		if (clientMap_[playerNumber].udpRemotePort == 0)
			continue;
		sf::Packet sendingPacket;
		GeneratePacket(sendingPacket, *packet);
		udpSocket_.send(sendingPacket, tcpSockets_[playerNumber].getRemoteAddress(), clientMap_[playerNumber].udpRemotePort);
	}

	

}

void ServerNetworkManager::Init()
{
    sf::Socket::Status status = sf::Socket::Error;
    while(status != sf::Socket::Done)
    {
        // bind the listener to a port
        status = tcpListener_.listen(tcpPort_);
        if(status != sf::Socket::Done)
        {
            tcpPort_++;
        }
    }
    tcpListener_.setBlocking(false);
    std::string log = "[Server] Tcp Socket on port: ";
    log += std::to_string(tcpPort_);
    logDebug(log);

    status = sf::Socket::Error;
    while(status != sf::Socket::Done)
    {
        status = udpSocket_.bind((udpPort_));
        if(status != sf::Socket::Done)
        {
            udpPort_++;
        }
    }
    udpSocket_.setBlocking(false);
    log = "[Server] Udp Socket on port: ";
    log += std::to_string(udpPort_);
    logDebug(log);
}

void ServerNetworkManager::Update(seconds dt)
{
    if(lastPlayerNumber_ < 2)
    {
        sf::Socket::Status status = tcpListener_.accept(tcpSockets_[lastPlayerNumber_]);
        if(status == sf::Socket::Done)
        {
            const auto remoteAddress = tcpSockets_[lastPlayerNumber_].getRemoteAddress().toString();
            const auto port = std::to_string(tcpSockets_[lastPlayerNumber_].getRemotePort());
            const std::string log = "New player connection with address: "+remoteAddress+" and port: "+port;
            logDebug(log);
            lastPlayerNumber_++;
        }
    }

    for(PlayerNumber playerNumber = 0; playerNumber < lastPlayerNumber_; playerNumber++)
    {
        sf::Packet tcpPacket;
        const sf::Socket::Status status = tcpSockets_[playerNumber].receive(tcpPacket);
        if(status == sf::Socket::Done)
        {
            ReceivePacket(tcpPacket, PacketSocketSource::TCP);
        }
    }
	sf::Packet packet;
	sf::IpAddress sender;
	unsigned short port;
	const sf::Socket::Status status = udpSocket_.receive(packet, sender, port);
	if(status == sf::Socket::Done)
	{
		ReceivePacket(packet, PacketSocketSource::UDP);
	}



}

void ServerNetworkManager::Destroy()
{

}

void ServerNetworkManager::SetTcpPort(unsigned short i)
{
    tcpPort_ = i;
}

void ServerNetworkManager::ProcessReceivePacket(std::unique_ptr<asteroid::Packet> packet, 
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
		ClientId clientId = 0;
		auto* clientIdPtr = reinterpret_cast<std::uint8_t*>(&clientId);
		for (size_t i = 0; i < sizeof(ClientId); i++)
		{
			clientIdPtr[i] = clientId;
		}
		if (packetSource == PacketSocketSource::UDP)
		{
			const auto it = std::find_if(clientMap_.begin(), clientMap_.end(), [&clientId](const auto& clientInfo)
				{
					return clientInfo.clientId == clientId;
				});
			if (it != clientMap_.end())
			{
				const PlayerNumber playerNumber = std::distance(clientMap_.begin(), it);
				clientMap_[playerNumber].udpRemoteAddress = address;
				clientMap_[playerNumber].udpRemotePort = port;
			}
		}
		if (std::find_if(clientMap_.begin(), clientMap_.end(), [&clientId](const auto& clientInfo)
		    {
			    return clientInfo.clientId == clientId;
		    }) != clientMap_.end())
		{
			//Player joined twice!
			return;
		}
		
		//Spawning the new player in the arena
		logDebug("Managing Received Packet Join from: " + std::to_string(clientId));
		auto spawnPlayer = std::make_unique<asteroid::SpawnPlayerPacket>();
		spawnPlayer->packetType = asteroid::PacketType::SPAWN_PLAYER;
		spawnPlayer->clientId = joinPacket->clientId;
		spawnPlayer->playerNumber = lastPlayerNumber_;

		const auto pos = spawnPositions_[lastPlayerNumber_] * 3.0f;
		const auto* const posPtr = reinterpret_cast<const std::uint8_t*>(&pos[0]);
		for (size_t i = 0; i < sizeof(Vec2f); i++)
		{
			spawnPlayer->pos[i] = posPtr[i];
		}
		const auto rotation = spawnRotations_[lastPlayerNumber_];
		const auto* const rotationPtr = reinterpret_cast<const std::uint8_t*>(&rotation);
		for (size_t i = 0; i < sizeof(degree_t); i++)
		{
			spawnPlayer->angle[i] = rotationPtr[i];
		}
		clientMap_[lastPlayerNumber_] = {
		        clientId,
		        packetSource == PacketSocketSource::UDP ? address : "",
		        packetSource == PacketSocketSource::UDP ? port:static_cast<unsigned short>(0u)};
		gameManager_.SpawnPlayer(lastPlayerNumber_, pos, rotation);
		lastPlayerNumber_++;
		SendReliablePacket(std::move(spawnPlayer));

		if (lastPlayerNumber_ == asteroid::maxPlayerNmb)
		{
			auto startGamePacket = std::make_unique<asteroid::StartGamePacket>();
			startGamePacket->packetType = asteroid::PacketType::START_GAME;
			using namespace std::chrono;
			auto ms = (duration_cast<milliseconds>(
				system_clock::now().time_since_epoch()
				) + milliseconds(3000)).count();
			const auto* msPtr = reinterpret_cast<std::uint8_t*>(&ms);
			for (size_t i = 0; i < sizeof(ms); i++)
			{
				startGamePacket->startTime[i] = msPtr[i];
			}
			SendReliablePacket(std::move(startGamePacket));
		}

		break;
	}
	case asteroid::PacketType::SPAWN_PLAYER: break;
	case asteroid::PacketType::INPUT:
	{
		//Manage internal state
		const auto* playerInputPacket = static_cast<const asteroid::PlayerInputPacket*>(packet.get());
		const auto playerNumber = playerInputPacket->playerNumber;
		std::uint32_t inputFrame = 0;
		auto* inputPtr = reinterpret_cast<std::uint8_t*>(&inputFrame);
		for (size_t i = 0; i < sizeof(std::uint32_t); i++)
		{
			inputPtr[i] = playerInputPacket->currentFrame[i];
		}
		for (std::uint32_t i = 0; i < playerInputPacket->inputs.size(); i++)
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
		std::uint32_t lastReceiveFrame = gameManager_.GetRollbackManager().GetLastReceivedFrame(0);
		for (PlayerNumber i = 1; i < asteroid::maxPlayerNmb; i++)
		{
			const auto playerLastFrame = gameManager_.GetRollbackManager().GetLastReceivedFrame(i);
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
			validatePacket->packetType = asteroid::PacketType::VALIDATE_STATE;
			const auto* framePtr = reinterpret_cast<std::uint8_t*>(&lastReceiveFrame);
			for (size_t i = 0; i < sizeof(std::uint32_t); i++)
			{
				validatePacket->newValidateFrame[i] = framePtr[i];
			}
			//copy physics state
			for (PlayerNumber i = 0; i < asteroid::maxPlayerNmb; i++)
			{
				auto physicsState = gameManager_.GetRollbackManager().GetValidatePhysicsState(i);
				const auto* statePtr = reinterpret_cast<const std::uint8_t*>(&physicsState);
				for (size_t j = 0; j < sizeof(asteroid::PhysicsState); j++)
				{
					validatePacket->physicsState[i * sizeof(asteroid::PhysicsState) + j] = statePtr[j];
				}
			}
			SendUnreliablePacket(std::move(validatePacket));
		}

		break;
	}

	case asteroid::PacketType::SPAWN_BULLET: break;
	case asteroid::PacketType::VALIDATE_STATE: break;
	case asteroid::PacketType::START_GAME: break;
	case asteroid::PacketType::NONE: break;
	default:;
	}
}

void ServerNetworkManager::ReceivePacket(sf::Packet& packet, PacketSocketSource packetSource,
	sf::IpAddress address,
	unsigned short port )
{
    auto receivedPacket = asteroid::GenerateReceivedPacket(packet);
    if(receivedPacket != nullptr)
    {
		ProcessReceivePacket(std::move(receivedPacket), packetSource);
    }
}
}