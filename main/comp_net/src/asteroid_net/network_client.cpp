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
#include <chrono>
#include "asteroid_net/network_client.h"
#include "engine/engine.h"
#include "imgui.h"
#include "engine/conversion.h"

namespace neko::net
{

void ClientNetworkManager::Init()
{

    clientId_ = RandomRange(std::numeric_limits<ClientId>::lowest(),
        std::numeric_limits<ClientId>::max());
    //JOIN packet
    gameManager_.Init();
    tcpSocket_.setBlocking(false);
    udpSocket_.setBlocking(true);
    auto status = sf::Socket::Error;
    while (status != sf::Socket::Done)
    {
        status = udpSocket_.bind(sf::Socket::AnyPort);
    }
    udpSocket_.setBlocking(false);
}

void ClientNetworkManager::Update(seconds dt)
{

    if (currentState_ != State::NONE)
    {
        auto status = sf::Socket::Done;
        //Receive TCP Packet
        while (status == sf::Socket::Done)
        {
            sf::Packet packet;
            status = tcpSocket_.receive(packet);
            switch (status)
            {
            case sf::Socket::Done:
                ReceivePacket(packet, PacketSource::TCP);
                break;
            case sf::Socket::NotReady:
                //logDebug("[Client] Error while receiving tcp socket is not ready");
                break;
            case sf::Socket::Partial:
                logDebug("[Client] Error while receiving TCP packet, PARTIAL");
                break;
            case sf::Socket::Disconnected: break;
            case sf::Socket::Error: break;
            default:;
            }
        }
        //Receive UDP packet
        status = sf::Socket::Done;
        while (status == sf::Socket::Done)
        {
            sf::Packet packet;
            sf::IpAddress sender;
            unsigned short port;
            status = udpSocket_.receive(packet, sender, port);
            switch (status)
            {
            case sf::Socket::Done:
                ReceivePacket(packet, PacketSource::UDP);
                break;
            case sf::Socket::NotReady: break;
            case sf::Socket::Partial:
                logDebug("[Client] Error while receiving UDP packet, PARTIAL");
                break;
            case sf::Socket::Disconnected:
                logDebug("[Client] Error while receiving UDP packet, DISCONNECTED");
                break;
            case sf::Socket::Error:
                logDebug("[Client] Error while receiving UDP packet, ERROR");
                break;
            default:;
            }
        }
        switch (currentState_)
        {
        case State::JOINING:
        {
            if (serverUdpPort_ != 0)
            {
                //Need to send a join packet on the unreliable channel
                auto joinPacket = std::make_unique<asteroid::JoinPacket>();
                joinPacket->clientId = ConvertToBinary<ClientId>(clientId_);
                SendUnreliablePacket(std::move(joinPacket));
            }
            break;
        }
        default:
            break;
        }
    }

    gameManager_.Update(dt);
}

void ClientNetworkManager::Destroy()
{
    gameManager_.Destroy();
}

void ClientNetworkManager::DrawImGui()
{
    const auto windowName = "Client " + std::to_string(clientId_);
    ImGui::Begin(windowName.c_str());
    const size_t bufferSize = 100;
    char hostBuffer[bufferSize];
    std::strncpy(hostBuffer, serverAddress_.c_str(), bufferSize);
    if (ImGui::InputText("Host", hostBuffer, bufferSize))
    {
        serverAddress_ = hostBuffer;
    }
    int portBuffer = serverTcpPort_;
    if (ImGui::InputInt("Port", &portBuffer))
    {
        serverTcpPort_ = static_cast<unsigned short>(portBuffer);
    }
    if (currentState_ == State::NONE &&
        ImGui::Button("Join"))
    {
        tcpSocket_.setBlocking(true);
        const auto status = tcpSocket_.connect(serverAddress_, serverTcpPort_);
        tcpSocket_.setBlocking(false);
        if (status == sf::Socket::Done)
        {
            logDebug("[Client] Connect to server " + serverAddress_ + " with port: " + std::to_string(serverTcpPort_));
            auto joinPacket = std::make_unique<asteroid::JoinPacket>();
            joinPacket->clientId = ConvertToBinary<ClientId>(clientId_);
            using namespace std::chrono;
            const unsigned long clientTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch())).count();
            joinPacket->startTime = ConvertToBinary<unsigned long>(clientTime);
            SendReliablePacket(std::move(joinPacket));
            currentState_ = State::JOINING;
        }
        else
        {
            logDebug("[Client] Error trying to connect to " + serverAddress_ + " with port: " +
                std::to_string(serverTcpPort_) + " with status: " + std::to_string(status));
        }
    }
    ImGui::Text("Server UDP port: %u", serverUdpPort_);
    gameManager_.DrawImGui();
    ImGui::End();
}

void ClientNetworkManager::Render()
{
    gameManager_.Render();
}

void ClientNetworkManager::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{

    //logDebug("[Client] Sending reliable packet to server");
    sf::Packet tcpPacket;
    GeneratePacket(tcpPacket, *packet);
    auto status = sf::Socket::Partial;
    while (status == sf::Socket::Partial)
    {
        status = tcpSocket_.send(tcpPacket);
    }
}

void ClientNetworkManager::SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet)
{

    sf::Packet udpPacket;
    GeneratePacket(udpPacket, *packet);
    const auto status = udpSocket_.send(udpPacket, serverAddress_, serverUdpPort_);
    switch (status)
    {
    case sf::Socket::Done:
        //logDebug("[Client] Sending UDP packet to server at host: " +
        //	serverAddress_.toString() + " port: " + std::to_string(serverUdpPort_));
        break;
    case sf::Socket::NotReady:
        logDebug("[Client] Error sending UDP to server, NOT READY");
        break;
    case sf::Socket::Partial:
        logDebug("[Client] Error sending UDP to server, PARTIAL");
        break;
    case sf::Socket::Disconnected:
        logDebug("[Client] Error sending UDP to server, DISCONNECTED");
        break;
    case sf::Socket::Error:
        logDebug("[Client] Error sending UDP to server, ERROR");
        break;
    default:
        break;
    }
}

void ClientNetworkManager::SetPlayerInput(PlayerInput input)
{
    const auto currentFrame = gameManager_.GetCurrentFrame();
    gameManager_.SetPlayerInput(
        gameManager_.GetPlayerNumber(),
        input,
        currentFrame);
}

void ClientNetworkManager::ReceivePacket(sf::Packet& packet, PacketSource source)
{
    const auto receivePacket = asteroid::GenerateReceivedPacket(packet);
    Client::ReceivePacket(receivePacket.get());
    switch (receivePacket->packetType)
    {
    case asteroid::PacketType::JOIN_ACK:
    {
        logDebug("[Client] Receive " + std::string(source == PacketSource::UDP ? "UDP" : "TCP") + " Join ACK Packet");
        auto* joinAckPacket = static_cast<asteroid::JoinAckPacket*>(receivePacket.get());

        serverUdpPort_ = ConvertFromBinary<unsigned short>(joinAckPacket->udpPort);
        const auto clientId = ConvertFromBinary<ClientId>(joinAckPacket->clientId);
        if (clientId != clientId_)
            return;
        if (source == PacketSource::TCP)
        {
            //Need to send a join packet on the unreliable channel
            auto joinPacket = std::make_unique<asteroid::JoinPacket>();
            joinPacket->clientId = ConvertToBinary<ClientId>(clientId_);
            SendUnreliablePacket(std::move(joinPacket));
        }
        else
        {
            if (currentState_ == State::JOINING)
            {
                currentState_ = State::JOINED;
            }
        }
        break;
    }
    default:
        break;
    }
}
}
