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
            switch (status)
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
        if (clientInfoMap_[playerNumber].udpRemotePort == 0)
        {
            logDebug(fmt::format("[Warning] Trying to send UDP packet, but missing port!"));
            continue;
        }

        sf::Packet sendingPacket;
        GeneratePacket(sendingPacket, *packet);
        const auto status = udpSocket_.send(sendingPacket, clientInfoMap_[playerNumber].udpRemoteAddress,
            clientInfoMap_[playerNumber].udpRemotePort);
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
        switch (status)
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

bool ServerNetworkManager::IsOpen() const
{
    return status_ & OPEN;
}

void ServerNetworkManager::SpawnNewPlayer(ClientId clientId, PlayerNumber playerNumber)
{
    //Spawning the new player in the arena
    for (PlayerNumber p = 0; p <= lastPlayerNumber_; p++)
    {
        auto spawnPlayer = std::make_unique<asteroid::SpawnPlayerPacket>();
        spawnPlayer->clientId = ConvertToBinary(clientMap_[p]);
        spawnPlayer->playerNumber = p;

        const auto pos = asteroid::spawnPositions[p] * 3.0f;
        spawnPlayer->pos = ConvertToBinary(pos);

        const auto rotation = asteroid::spawnRotations[p];
        spawnPlayer->angle = ConvertToBinary(rotation);
        gameManager_.SpawnPlayer(p, pos, rotation);

        SendReliablePacket(std::move(spawnPlayer));
    }
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
        const auto joinPacket = *static_cast<asteroid::JoinPacket*>(packet.get());
        Server::ReceivePacket(std::move(packet));
        auto clientId = ConvertFromBinary<ClientId>(joinPacket.clientId);
        logDebug(fmt::format("[Server] Received Join Packet from: {} {}", clientId,
            (packetSource == PacketSocketSource::UDP ? fmt::format(" UDP with port: {}", port) : " TCP")));
        const auto it = std::find(clientMap_.begin(), clientMap_.end(), clientId);
        PlayerNumber playerNumber;
        if (it != clientMap_.end())
        {
            playerNumber = std::distance(clientMap_.begin(), it);
            clientInfoMap_[playerNumber].clientId = clientId;
        }
        else
        {
            neko_assert(false, "Player Number is supposed to be already set!")
        }

        auto joinAckPacket = std::make_unique<asteroid::JoinAckPacket>();
        joinAckPacket->clientId = ConvertToBinary(clientId);
        joinAckPacket->udpPort = ConvertToBinary(udpPort_);
        if (packetSource == PacketSocketSource::UDP)
        {
            auto& clientInfo = clientInfoMap_[playerNumber];
            clientInfo.udpRemoteAddress = address;
            clientInfo.udpRemotePort = port;
            SendUnreliablePacket(std::move(joinAckPacket));
        }
        else
        {
            SendReliablePacket(std::move(joinAckPacket));
            //Calculate time difference
            const auto clientTime = ConvertFromBinary<unsigned long>(joinPacket.startTime);
            using namespace std::chrono;
            const unsigned long deltaTime = (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count()) - clientTime;
            logDebug(fmt::format("[Server] Client Server deltaTime: {}", deltaTime));
            clientInfoMap_[playerNumber].timeDifference = deltaTime;
        }
        break;
    }
    default:
        Server::ReceivePacket(std::move(packet));
        break;
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
