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
	server_(server)
{
}

void SimulationClient::Init()
{
	const auto& config = BasicEngine::GetInstance()->GetConfig();

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
	gameManager_.Destroy();

}

void SimulationClient::Render()
{
	gameManager_.Render();
}


void SimulationClient::SetPlayerInput(net::PlayerInput playerInput)
{
    const auto currentFrame = gameManager_.GetCurrentFrame();
	gameManager_.SetPlayerInput(
		gameManager_.GetPlayerNumber(),
		playerInput, 
		currentFrame);

}

void SimulationClient::SetWindowSize(Vec2u windowSize)
{
    gameManager_.SetWindowSize(windowSize);
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
    server_.PutPacketInReceiveQueue(std::move(packet));
}

void SimulationClient::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    server_.PutPacketInReceiveQueue(std::move(packet));
}

}
