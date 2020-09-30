//
// Created by efarhan on 9/30/20.
//

#include "asteroid_net/network_server.h"

namespace neko::net
{
void ServerNetworkManager::SendReliablePacket(std::unique_ptr<asteroid::Packet> packet)
{

}

void ServerNetworkManager::SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet)
{

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
            auto remoteAddress = tcpSockets_[lastPlayerNumber_].getRemoteAddress().toString();
            auto port = std::to_string(tcpSockets_[lastPlayerNumber_].getRemotePort());
            std::string log = "New player connection with address: "+remoteAddress+" and port: "+port;
            logDebug(log);
            lastPlayerNumber_++;
        }
    }
    for(PlayerNumber playerNumber = 0; playerNumber < lastPlayerNumber_; playerNumber++)
    {
        sf::Packet packet;
        sf::Socket::Status status = tcpSockets_[playerNumber].receive(packet);
        if(status == sf::Socket::Done)
        {
            ReceivePacket(packet);
        }
    }

}

void ServerNetworkManager::Destroy()
{

}

void ServerNetworkManager::SetTcpPort(unsigned short i)
{
    tcpPort_ = i;
}

void ServerNetworkManager::ReceivePacket(sf::Packet& packet)
{

}
}