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

#include <fmt/format.h>

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
            for (auto& client : clients_)
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
    float minDelay = avgDelay_ - marginDelay_;
    float maxDelay = avgDelay_ + marginDelay_;
    bool hasDelayChanged = false;
    hasDelayChanged = hasDelayChanged || ImGui::SliderFloat("Min Delay", &minDelay, 0.01f, maxDelay);
    hasDelayChanged = hasDelayChanged || ImGui::SliderFloat("Max Delay", &maxDelay, minDelay, 1.0f);
    if (hasDelayChanged)
    {
        avgDelay_ = (maxDelay + minDelay) / 2.0f;
        marginDelay_ = (maxDelay - minDelay) / 2.0f;
    }
    ImGui::End();
}

void SimulationServer::PutPacketInSendingQueue(std::unique_ptr<asteroid::Packet> packet)
{
    sentPackets_.push_back({ avgDelay_ + RandomRange(-marginDelay_, marginDelay_), std::move(packet) });
}

void SimulationServer::PutPacketInReceiveQueue(std::unique_ptr<asteroid::Packet> packet)
{
    receivedPackets_.push_back({ avgDelay_ + RandomRange(-marginDelay_, marginDelay_), std::move(packet) });
}

void SimulationServer::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    PutPacketInSendingQueue(std::move(packet));
}

void SimulationServer::SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet)
{
    PutPacketInSendingQueue(std::move(packet));
}

void SimulationServer::ProcessReceivePacket(std::unique_ptr<asteroid::Packet> packet)
{
    Server::ReceivePacket(std::move(packet));
}

void SimulationServer::SpawnNewPlayer(ClientId clientId, PlayerNumber playerNumber)
{

    auto spawnPlayer = std::make_unique<asteroid::SpawnPlayerPacket>();
    spawnPlayer->packetType = asteroid::PacketType::SPAWN_PLAYER;
    spawnPlayer->clientId = ConvertToBinary(clientId);
    spawnPlayer->playerNumber = playerNumber;

    const auto pos = asteroid::spawnPositions[playerNumber] * 3.0f;
    spawnPlayer->pos = ConvertToBinary(pos);
    const auto rotation = asteroid::spawnRotations[playerNumber];
    spawnPlayer->angle = ConvertToBinary(rotation);
    gameManager_.SpawnPlayer(playerNumber, pos, rotation);
    SendReliablePacket(std::move(spawnPlayer));
}
}
