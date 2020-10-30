#include <chrono>
#include <iostream>
#include "SFML/Network.hpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    // ----- The server -----
    // Create a socket and bind it to the port 55002
    sf::UdpSocket socket;
    socket.bind(12345);
    std::cout << "Server binded\n";
    while (true)
    {
        // Receive a message from anyone
        sf::Packet packet;
        std::size_t received = 0;
        sf::IpAddress sender;
        unsigned short port;
        socket.receive(packet, sender, port);
        std::cout << "Received packet from " <<sender.toString() << ':' << port << std::endl;

        socket.send(packet, sender, port);
    }
    return 0;
}
