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

#include "asteroid_simulation/simulation_client.h"
#include "asteroid_simulation/simulation_server.h"
#include "asteroid/packet_type.h"
#include "engine/engine.h"
#include "engine/conversion.h"

#include "imgui.h"

namespace neko::net
{
SimulationClient::SimulationClient(SimulationServer& server) :
	server_(server), gameManager_(*this)
{
}

void SimulationClient::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	windowSize_ = config.windowSize;
	gameManager_.SetWindowSize(windowSize_ / Vec2u(2, 1));
	framebuffer_.SetSize(windowSize_ / Vec2u(2, 1));
	framebuffer_.Create();

	clientId_ = RandomRange(std::numeric_limits<ClientId>::lowest(),
		std::numeric_limits<ClientId>::max());
	//JOIN packet
	gameManager_.Init();


}

void SimulationClient::Update(seconds dt)
{
	gameManager_.Update(dt);
}



void SimulationClient::Destroy()
{
	framebuffer_.Destroy();
	gameManager_.Destroy();

}

void SimulationClient::Render()
{
	const auto& config = BasicEngine::GetInstance()->config;

	if (config.windowSize != windowSize_)
	{
		windowSize_ = config.windowSize;
		framebuffer_.SetSize(windowSize_ / Vec2u(2, 1));
		framebuffer_.Reload();
		gameManager_.SetWindowSize(windowSize_ / Vec2u(2, 1));

	}

	framebuffer_.Bind();
	framebuffer_.Clear(Vec3f(0.0f));
	gameManager_.Render();
	gl::Framebuffer::Unbind();


}


void SimulationClient::SetPlayerInput(net::PlayerInput playerInput)
{
    auto currentFrame = gameManager_.GetCurrentFrame();
	gameManager_.SetPlayerInput(
		gameManager_.GetPlayerNumber(),
		playerInput, 
		currentFrame);

}

void SimulationClient::DrawImGui()
{
	const auto windowName = "Client "+std::to_string(clientId_);
    ImGui::Begin(windowName.c_str());
    if(gameManager_.GetPlayerNumber() == INVALID_PLAYER && ImGui::Button("Spawn Player"))
    {
        auto joinPacket = std::make_unique<asteroid::JoinPacket>();
        auto* clientIdPtr = reinterpret_cast<std::uint8_t*>(&clientId_);
        for(int i = 0; i < sizeof(clientId_); i++)
        {
            joinPacket->clientId[i] = clientIdPtr[i];
        }
        SendReliablePacket(std::move(joinPacket));
    }
    gameManager_.DrawImGui();
    ImGui::End();
}

void SimulationClient::SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    server_.ReceivePacket(std::move(packet));
}

void SimulationClient::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    server_.ReceivePacket(std::move(packet));
}

void SimulationClient::ReceivePacket(const asteroid::Packet* packet)
{
    const auto packetType = packet->packetType;
    switch (packetType)
    {
        case asteroid::PacketType::SPAWN_PLAYER:
        {
            const auto* spawnPlayerPacket = static_cast<const asteroid::SpawnPlayerPacket*>(packet);
            const ClientId clientId = ConvertFromBinary<ClientId>(spawnPlayerPacket->clientId);

            const PlayerNumber playerNumber = spawnPlayerPacket->playerNumber;
            if (clientId == clientId_)
            {
                gameManager_.SetClientPlayer(playerNumber);
            }

            const Vec2f pos = ConvertFromBinary<Vec2f>(spawnPlayerPacket->pos);

            const degree_t rotation = ConvertFromBinary<degree_t>(spawnPlayerPacket->angle);
           
            gameManager_.SpawnPlayer(playerNumber, pos, rotation);
            break;
        }
        case asteroid::PacketType::START_GAME:
        {
            const auto* startGamePacket = static_cast<const asteroid::StartGamePacket*>(packet);
            unsigned long startingTime = ConvertFromBinary<unsigned long>(startGamePacket->startTime);
            gameManager_.StartGame(startingTime);
            break;
        }
        case asteroid::PacketType::INPUT:
        {
            const auto* playerInputPacket = static_cast<const asteroid::PlayerInputPacket*>(packet);
            const auto playerNumber = playerInputPacket->playerNumber;
            const net::Frame inputFrame = ConvertFromBinary<Frame>(playerInputPacket->currentFrame);
            
            if(playerNumber == gameManager_.GetPlayerNumber())
            {
                //Verify the inputs coming back from the server
                const auto& inputs = gameManager_.GetRollbackManager().GetInputs(playerNumber);
                const auto currentFrame = gameManager_.GetRollbackManager().GetCurrentFrame();
                for(size_t i = 0; i < playerInputPacket->inputs.size(); i++)
                {
                    const auto index = currentFrame-inputFrame+i;
                    if(index > inputs.size())
                    {
                        break;
                    }
                    if(inputs[index] != playerInputPacket->inputs[i])
                    {
                        neko_assert(false, "Inputs coming back from server are not coherent!!!");
                    }
                    if(inputFrame - i == 0)
                    {
                        break;
                    }
                }
                break;
            }

            //discard delayed input packet
            if(inputFrame < gameManager_.GetRollbackManager().GetLastReceivedFrame(playerNumber))
            {
                break;
            }
            for(int i = 0; i < playerInputPacket->inputs.size(); i++)
            {
                gameManager_.SetPlayerInput(playerNumber, 
                    playerInputPacket->inputs[i], 
                    inputFrame - i);
            	
                if(inputFrame-i == 0)
                {
                    break;
                }
            }
            break;
        }
        case asteroid::PacketType::VALIDATE_STATE:
        {
        	
            const auto* validateFramePacket = static_cast<const asteroid::ValidateFramePacket*>(packet);
            const Frame newValidateFrame = ConvertFromBinary<Frame>(validateFramePacket->newValidateFrame);

        	std::array<asteroid::PhysicsState, asteroid::maxPlayerNmb> physicsStates{};
        	for(size_t i = 0; i < validateFramePacket->physicsState.size(); i++)
            {
        	    auto* statePtr = reinterpret_cast<std::uint8_t*>(physicsStates.data());
        	    statePtr[i] = validateFramePacket->physicsState[i];
            }
            gameManager_.ConfirmValidateFrame(newValidateFrame, physicsStates);
            //logDebug("Client received validate frame " + std::to_string(newValidateFrame));
        	break;
        }
        case asteroid::PacketType::SPAWN_BULLET: break;
        default: ;
    }
}
}
