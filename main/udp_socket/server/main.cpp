#include <iostream>
#include "SFML/Network.hpp"

int main(int argc, char** argv)
{
    // ----- The server -----
    // Create a socket and bind it to the port 55002
    sf::UdpSocket socket;
    socket.bind(55002);
    std::cout << "Server binded\n";
    // Receive a message from anyone
    char buffer[1024];
    std::size_t received = 0;
    sf::IpAddress sender;
    unsigned short port;
    socket.receive(buffer, sizeof(buffer), received, sender, port);
    std::cout << sender.toString() << " said: " << buffer << std::endl;
    // Send an answer
    std::string message = "Welcome " + sender.toString();
    socket.send(message.c_str(), message.size() + 1, sender, port);
    return 0;
}