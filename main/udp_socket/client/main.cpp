#include <iostream>
#include <array>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>

#include "SFML/Network.hpp"

const size_t timePointSize = sizeof(std::chrono::steady_clock::time_point);
const sf::Uint32 packetSentNmb = 100;

struct PingPacket
{
    sf::Uint32 seqNumber = 0;
    std::array<sf::Uint8, timePointSize> sentTime{};
	void SetSentTime(std::chrono::steady_clock::time_point timePoint)
	{
        const auto* timePointPtr = reinterpret_cast<const sf::Uint8*>(&timePoint);
		for(size_t i = 0; i < timePointSize; i++)
		{
            sentTime[i] = timePointPtr[i];
		}
	}
    [[nodiscard]] std::chrono::steady_clock::time_point GetSentTime() const
	{
        std::chrono::steady_clock::time_point timePoint;
        auto* timePointPtr = reinterpret_cast<sf::Uint8*>(&timePoint);
        for (size_t i = 0; i < timePointSize; i++)
        {
            timePointPtr[i] = sentTime[i];
        }
        return timePoint;
	}
};
struct PingResult
{
    float deltaTime = 0.0f;
    bool received = false;
};
sf::Packet& operator <<(sf::Packet& packet, const PingPacket& ping)
{
    packet << ping.seqNumber;
	for(size_t i = 0; i < timePointSize; i++)
	{
        packet << ping.sentTime[i];
	}
    return packet;
}

sf::Packet& operator >>(sf::Packet& packet, PingPacket& ping)
{
    packet >> ping.seqNumber;
    for (size_t i = 0; i < timePointSize; i++)
    {
        packet >> ping.sentTime[i];
    }
    return packet;
}

static std::array<PingResult, packetSentNmb> results;
void ReceivePacket(sf::Packet& packet)
{
    PingPacket ping;
    packet >> ping;
    const auto time = std::chrono::steady_clock::now();
    auto& pingResult = results[ping.seqNumber];
    pingResult.deltaTime = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(
        time - ping.GetSentTime()).count();
    pingResult.received = true;
}

int main(int argc, char** argv)
{
    // ----- The client -----
    const sf::IpAddress destinationHost = "eliasfarhan.ch";
    const unsigned short destinationPort = 12345;
	// Create a socket and bind it to the port 55001
    sf::UdpSocket socket;
    socket.bind(12345);
    socket.setBlocking(false);
	
    size_t successfulSentPacketNmb = 0;
	for(sf::Uint32 i = 0; i < packetSentNmb; i++)
	{
        const auto time = std::chrono::steady_clock::now();
        PingPacket ping{ i, {} };
        ping.SetSentTime(time);
        sf::Packet packet;
        packet << ping;
        if(socket.send(packet, destinationHost, destinationPort) == sf::Socket::Done)
        {
            successfulSentPacketNmb++;
        }
        sf::Packet receivePacket;
        sf::IpAddress sender;
        unsigned short port;
		if(socket.receive(receivePacket, sender, port) == sf::Socket::Status::Done)
		{
            ReceivePacket(receivePacket);
		}
	}
    const auto startTime = std::chrono::system_clock::now();
	auto currentTime = std::chrono::system_clock::now();
	while(std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1, 1>>>(currentTime-startTime).count() < 10.0f)
	{
        sf::Packet receivePacket;
        sf::IpAddress sender;
        unsigned short port;
        if (socket.receive(receivePacket, sender, port) == sf::Socket::Status::Done)
        {
            ReceivePacket(receivePacket);
        }
        currentTime = std::chrono::system_clock::now();
	}
    const auto receiveCount = std::count_if(results.cbegin(), results.cend(), [](const PingResult& result)
        {
            return result.received;
        });
    const float avgPing = receiveCount == 0 ? -1.0f : std::accumulate(results.cbegin(), results.cend(), 0.0f, [](float sum, const PingResult& result)
    {
            return sum + result.deltaTime;
    })/static_cast<float>(receiveCount);
	const float varPing = receiveCount == 0 ? -1.0f : std::accumulate(results.cbegin(), results.cend(), 0.0f, [](float sum, const PingResult& result)
    {
        return sum + result.deltaTime*result.deltaTime;
    }) / static_cast<float>(receiveCount)-avgPing*avgPing;
	
    std::cout << "Sent success: "<<successfulSentPacketNmb<<
        "\nReceived Success: "<< receiveCount << " / "<<packetSentNmb<<" packet sent\nAvg RTT: "<< avgPing << 
        "\nVar RTT: "<< varPing  <<"\nStandard deviation: "<<std::sqrt(varPing)<<std::endl;
}