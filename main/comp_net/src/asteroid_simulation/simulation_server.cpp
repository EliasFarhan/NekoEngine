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

#include "asteroid_simulation/simulation_server.h"
#include "engine/conversion.h"
#include "asteroid_simulation/simulation_client.h"
#include "asteroid/game_manager.h"
#include "asteroid/packet_type.h"
#include "comp_net/packet.h"
#include "imgui.h"

namespace neko::net
{
SimulationServer::SimulationServer(std::array<std::unique_ptr<SimulationClient>, 2>& clients) : clients_(clients)
{
}

void SimulationServer::Init()
{
	gameManager_.Init();
	
}

void SimulationServer::Update(seconds dt)
{

	auto packetIt = receivedPackets_.begin();
	while (packetIt != receivedPackets_.end())
	{
		packetIt->currentTime -= dt.count();
		if (packetIt->currentTime <= 0.0f)
		{
            ProcessReceivePacket(std::move(packetIt->packet));
			
			packetIt = receivedPackets_.erase(packetIt);
		}
		else
		{
			++packetIt;
		}

	}

	packetIt = sentPackets_.begin();
	while (packetIt != sentPackets_.end())
	{
		packetIt->currentTime -= dt.count();
		if (packetIt->currentTime <= 0.0f)
		{
			for (auto& client: clients_)
			{
				client->ReceivePacket(packetIt->packet.get());
			}
            packetIt->packet = nullptr;
			packetIt = sentPackets_.erase(packetIt);
		}
		else
		{
			++packetIt;
		}
	}
}

void SimulationServer::Destroy()
{
}

void SimulationServer::DrawImGui()
{
	ImGui::Begin("Server");
	float minDelay = avgDelay_-marginDelay_;
	float maxDelay = avgDelay_+marginDelay_;
	bool hasDelayChanged = false;
	hasDelayChanged = hasDelayChanged || ImGui::SliderFloat("Min Delay", &minDelay, 0.01f, maxDelay);
	hasDelayChanged = hasDelayChanged || ImGui::SliderFloat("Max Delay", &maxDelay, minDelay, 1.0f);
	if(hasDelayChanged)
	{
		avgDelay_ = (maxDelay + minDelay) / 2.0f;
		marginDelay_ = (maxDelay - minDelay) / 2.0f;
	}
	ImGui::End();
}

void SimulationServer::SendPacket(std::unique_ptr<asteroid::Packet> packet)
{
    sentPackets_.push_back({ avgDelay_+RandomRange(-marginDelay_, marginDelay_), std::move(packet) });
}

void SimulationServer::ReceivePacket(std::unique_ptr<asteroid::Packet> packet)
{
    receivedPackets_.push_back({ avgDelay_ + RandomRange(-marginDelay_, marginDelay_), std::move(packet) });
}

void SimulationServer::ProcessReceivePacket(std::unique_ptr<asteroid::Packet> packet)
{
	const auto packetType = static_cast<asteroid::PacketType>(packet->packetType);
	switch (packetType)
	{
	case asteroid::PacketType::JOIN:
	{
		const auto* joinPacket = static_cast<asteroid::JoinPacket*>(packet.get());
		ClientId clientId = 0;
		auto* clientIdPtr = reinterpret_cast<std::uint8_t*>(&clientId);
		for(size_t i = 0; i < sizeof(ClientId);i++)
        {
		    clientIdPtr[i] = joinPacket->clientId[i];
        }
		if(std::find(clientMap_.begin(), clientMap_.end(), clientId) != clientMap_.end())
        {
		    //Player joined twice!
		    return;
        }
		logDebug("Managing Received Packet Join from: "+std::to_string(clientId));
		auto spawnPlayer = std::make_unique<asteroid::SpawnPlayerPacket>();
		spawnPlayer->packetType = asteroid::PacketType::SPAWN_PLAYER;
		spawnPlayer->clientId = joinPacket->clientId;
		spawnPlayer->playerNumber = lastPlayerNumber_;

		const auto pos = spawnPositions_[lastPlayerNumber_]*3.0f;
		const auto* const posPtr = reinterpret_cast<const std::uint8_t*>(&pos[0]);
		for (size_t i = 0; i < sizeof(Vec2f);i++)
		{
			spawnPlayer->pos[i] = posPtr[i];
		}
		const auto rotation = spawnRotations_[lastPlayerNumber_];
		const auto* const rotationPtr = reinterpret_cast<const std::uint8_t*>(&rotation);
        for (size_t i = 0; i < sizeof(degree_t);i++)
        {
            spawnPlayer->angle[i] = rotationPtr[i];
        }
		clientMap_[lastPlayerNumber_] = clientId;
		gameManager_.SpawnPlayer(lastPlayerNumber_, pos, rotation);
		lastPlayerNumber_++;
		SendPacket(std::move(spawnPlayer));
		
		if(lastPlayerNumber_ == asteroid::maxPlayerNmb)
        {
		    auto startGamePacket = std::make_unique<asteroid::StartGamePacket>();
            startGamePacket->packetType = asteroid::PacketType::START_GAME;
            using namespace std::chrono;
            unsigned long ms = (duration_cast< milliseconds >(
                    system_clock::now().time_since_epoch()
            )+milliseconds(3000)).count();
            startGamePacket->startTime = ConvertToBinary(ms);
            SendPacket(std::move(startGamePacket));
        }

		break;
	}
	case asteroid::PacketType::SPAWN_PLAYER: break;
	case asteroid::PacketType::INPUT:
	{
		//Manage internal state
		const auto* playerInputPacket = static_cast<const asteroid::PlayerInputPacket*>(packet.get());
		const auto playerNumber = playerInputPacket->playerNumber;
		std::uint32_t inputFrame = ConvertFromBinary<net::Frame>(playerInputPacket->currentFrame);

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
		SendPacket(std::move(packet));

		//Validate new frame if needed
		std::uint32_t lastReceiveFrame = gameManager_.GetRollbackManager().GetLastReceivedFrame(0);
		for(PlayerNumber i = 1; i < asteroid::maxPlayerNmb; i++)
		{
			const auto playerLastFrame = gameManager_.GetRollbackManager().GetLastReceivedFrame(i);
			if(playerLastFrame < lastReceiveFrame)
			{
				lastReceiveFrame = playerLastFrame;
			}
		}
		if(lastReceiveFrame > gameManager_.GetLastValidateFrame())
		{
			//Validate frame
            gameManager_.Validate(lastReceiveFrame);

			auto validatePacket = std::make_unique<asteroid::ValidateFramePacket>();
			validatePacket->packetType = asteroid::PacketType::VALIDATE_STATE;
			const auto* framePtr = reinterpret_cast<std::uint8_t*>(&lastReceiveFrame);
			for(size_t i = 0; i < sizeof(std::uint32_t); i++)
			{
				validatePacket->newValidateFrame[i] = framePtr[i];
			}
			//copy physics state
			for (PlayerNumber i = 0; i < asteroid::maxPlayerNmb; i++)
			{
				auto physicsState = gameManager_.GetRollbackManager().GetValidatePhysicsState(i);
				const auto* statePtr = reinterpret_cast<const std::uint8_t*>(&physicsState);
				for(size_t j = 0; j < sizeof(asteroid::PhysicsState); j++)
				{
					validatePacket->physicsState[i * sizeof(asteroid::PhysicsState) + j] = statePtr[j];
				}
			}
			SendPacket(std::move(validatePacket));
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
}
