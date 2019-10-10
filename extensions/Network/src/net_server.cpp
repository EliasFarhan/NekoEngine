#include <Network/net_server.h>
#include <engine/log.h>
#include <engine/globals.h>
#include <utilities/vector_utility.h>
#include <algorithm>

namespace neko
{
void Server::Start()
{
    serverThread = std::thread(&Server::ServerLoop, this);
    serverThread.detach();
}

void Server::Stop()
{
    isRunning = false;
    for (auto& client : clients_)
    {
        if (client.tcpSocket.getRemoteAddress() != sf::IpAddress::None)
        {
            client.commandSync.notify_one();
        }
    }
}

void Server::SendReliable(const std::shared_ptr<NetCommand> command, Index clientId)
{
    logDebug("[Server] Send command to queue client "+std::to_string(clientId));
    auto& client = clients_[clientId];
    client.sentCommands.push(command);
    client.commandSync.notify_one();
}

void Server::SendUnreliable(const NetCommand& command)
{
    (void)command;
}

void Server::ServerLoop()
{
    listener_.setBlocking(false);
    if (listener_.listen(SERVER_PORT) != sf::Socket::Done)
    {
        logDebug("[Error] Server: Cannot link listener to port: " + std::to_string(SERVER_PORT));
    }
    while (isRunning || currentIndex == INVALID_INDEX)
    {
        auto status = listener_.accept(clients_[currentIndex].tcpSocket);
        if (status != sf::Socket::Done)
        {
            if(status == sf::Socket::Error)
            {
                logDebug("[Error] Server client id: " + std::to_string(currentIndex) + " could not connect");
            }
            continue;
        }
        clients_[currentIndex].thread = std::thread(&Server::TcpSendSocketLoop, this, currentIndex);
        clients_[currentIndex].thread.detach();
        //Wait 1 ms before preparing the new connection
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1ms);
        {
            auto command = std::make_shared<NetCommand>();
            command->netCommandType = NetCommandType(NetCommandEnum::HELLO);
            SendReliable(command, currentIndex);
        }
        const auto clientDataResult = std::find_if(clients_.begin(), clients_.end(), [](const ClientData& client)
        {
            return client.tcpSocket.getRemoteAddress() == sf::IpAddress::None;
        });
        if (clientDataResult == clients_.end())
        {
            logDebug("[Error] Server: no more available client");
            currentIndex = INVALID_INDEX;
        }
        else
        {
            currentIndex = clientDataResult - clients_.begin();
        }
    }
}

void Server::TcpSendSocketLoop(const Index clientId)
{

    while (isRunning)
    {

        auto& clientData = clients_[clientId];
        bool isSentCommandEmpty = true;
        {
            //waiting to have something to send
            std::unique_lock<std::mutex> lock(clientData.commandMutex);
            clientData.commandSync.wait(lock);
            isSentCommandEmpty = clientData.sentCommands.empty();
        }
        if (!isSentCommandEmpty)
        {
            std::shared_ptr<neko::NetCommand> command = std::make_shared<NetCommand>();
            {
                std::unique_lock<std::mutex> lock(clientData.commandMutex);
                command = clientData.sentCommands.front();
                clientData.sentCommands.pop();
            }
            logDebug("[Server] Send command to client "+std::to_string(clientId));
            sf::Packet sentPacket;
            sentPacket << command;
            clientData.tcpSocket.send(sentPacket);

            sf::Packet receivedPacket;
            clientData.tcpSocket.receive(receivedPacket);
            {
                std::unique_lock<std::mutex> lock(clientData.commandMutex);
                clientData.receivedCommands.push(receivedPacket);
            }
        }
    }

}

void Server::Update()
{

    auto command = std::make_shared<NetCommand>();
    for(size_t i = 0; i < clients_.size();i++)
    {
        auto& client = clients_[i];
        std::unique_lock<std::mutex> lock(client.commandMutex, std::try_to_lock);
        if(!lock.owns_lock())
            continue;
        while(!client.receivedCommands.empty())
        {
            auto packet = client.receivedCommands.front();
            ParseCommand(packet, Index(i));
            client.receivedCommands.pop();
        }
    }
}

void Server::TcpReceiveSocketLoop(const Index clientId)
{
    (void)clientId;
}

}
