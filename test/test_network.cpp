//
// Created by efarhan on 13.07.19.
//

#include <gtest/gtest.h>
#include <Network/net_server.h>
#include <Network/net_client.h>
#include <Network/net_command.h>
#include "engine/log.h"
#include "SFML/Window/Keyboard.hpp"

#include <iostream>

const size_t clientNmb = 4;

enum class HelloCommandType : neko::NetCommandType
{
    HELLO,
    CLOSE
};

TEST(Network, SimpleHello)
{
    auto serverLoop = []()
    {

        sf::TcpListener listener;
        if (listener.listen(53000) != sf::Socket::Done)
        {
            // erreur...
        }

        for (int i = 0; i < clientNmb; i++)
        {
            // accepte une nouvelle connexion
            sf::TcpSocket client;
            if (listener.accept(client) != sf::Socket::Done)
            {
                // erreur...
            }
            auto command = std::make_shared<neko::NetCommand>();
            command->netCommandType = neko::NetCommandType(HelloCommandType::HELLO);

            //Send hello packet
            std::cout << "[Server] Sending hello packet to client: " << i << std::endl;
            sf::Packet packet;
            packet << command;
            client.send(packet);
            //Receive hello packet
            client.receive(packet);
            packet >> command;
            if (command->netCommandType == neko::NetCommandType(HelloCommandType::HELLO))
            {
                std::cout << "[Server] Received packet from client: " << i << std::endl;
            }
            else
            {
                //erreur
            }
            //Send close packet
            command->netCommandType = neko::NetCommandType(HelloCommandType::CLOSE);
            packet << command;
            client.send(packet);
        }
    };
    auto clientLoop = []()
    {
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect("localhost", 53000);
        if (status != sf::Socket::Done)
        {
            // erreur...
        }

        auto command = std::make_shared<neko::NetCommand>();
        sf::Packet packet;
        socket.receive(packet);

        packet >> command;
        if (command->netCommandType == neko::NetCommandType(HelloCommandType::HELLO))
        {
            std::cout << "[Client] Received hello packet from server! " << std::endl;
        }
        //Send hello packet
        packet << command;
        socket.send(packet);
        //Receive quit packet
        socket.receive(packet);
        packet >> command;
        if (command->netCommandType == neko::NetCommandType(HelloCommandType::CLOSE))
        {
            std::cout << "[Client] Received close packet from server! " << std::endl;
        }
    };

    std::thread serverThread(serverLoop);
    serverThread.detach();
    std::array<std::thread, clientNmb> clientThreads;
    for (int i = 0; i < clientNmb - 1; i++)
    {
        clientThreads[i] = std::thread(clientLoop);
        clientThreads[i].detach();
    }
    clientThreads.back() = std::thread(clientLoop);
    clientThreads.back().join();
}

class HelloServer : public neko::Server
{
protected:
    void ParseCommand(sf::Packet& packet, const neko::Index clientId) override
    {
        auto command = std::make_shared<neko::NetCommand>();
        packet >> command;
        switch (HelloCommandType(command->netCommandType))
        {
            case HelloCommandType::HELLO:
            {
                logDebug("[Server] Hello from " + std::to_string(clientId));
                auto newCommand = std::make_shared<neko::NetCommand>();
                newCommand->netCommandType = neko::NetCommandType(HelloCommandType::CLOSE);
                SendReliable(newCommand, clientId);
                break;
            }
            case HelloCommandType::CLOSE:
            {
                //Client should not close the server xD
                break;
            }
        }

    }

};

class HelloClient : public neko::Client
{
protected:
    void ParseCommand(sf::Packet& packet) override
    {
        auto command = std::make_shared<neko::NetCommand>();
        packet >> command;
        switch (HelloCommandType(command->netCommandType))
        {
            case HelloCommandType::HELLO:
            {
                logDebug("[Client] Hello from Server");
                auto newCommand = std::make_shared<neko::NetCommand>();
                newCommand->netCommandType = neko::NetCommandType(HelloCommandType::HELLO);
                SendReliable(newCommand);
                break;
            }
            case HelloCommandType::CLOSE:
                logDebug("[Client] Close from Server");
                isRunning = false;
                clientData.commandSync.notify_one();
                break;
        }
    }

};

TEST(Network, Hello)
{

    initLog();

    HelloServer server;
    server.Start();

    std::array<HelloClient, clientNmb> clients = {};
    for (auto& client : clients)
    {
        client.Start();
    }

    while (true)
    {

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
        {
            using namespace std::chrono_literals;
            server.Stop();
            for (auto& client : clients)
            {
                client.Stop();
            }
            std::this_thread::sleep_for(1s);
            break;
        }
        server.Update();
        for (auto& client : clients)
        {
            client.Update();
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(16ms);
    }
    destroyLog();
}