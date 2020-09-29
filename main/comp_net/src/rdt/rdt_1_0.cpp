//
// Created by efarhan on 7/24/20.
//

#include "rdt/rdt_1_0.h"
#include <cstring>

namespace neko::rdt
{
template<ClientType type>
void Rdt10Client<type>::Send(std::string_view msg)
{
    std::cout << "Sending msg: "<<msg<<'\n';
    size_t pktNumber = msg.size()/Packet::packetSize;
	if(pktNumber*Packet::packetSize != msg.size())
	{
        pktNumber++;
	}
    for(size_t i = 0; i < pktNumber;i++)
    {
        Packet p;
        for(size_t j = 0; j < Packet::packetSize; j++)
        {
            const auto index = i * Packet::packetSize + j;
        	if(index == msg.size())
                break;
            const char c = msg[index];
            if(c == 0)
                break;
            p.data[j] = c;
        }
        this->SendRaw(p);
    }
}
template<ClientType type>
void Rdt10Client<type>::ReceiveRaw(const Packet& packet)
{
    {
        char content[Packet::packetSize+1];
        content[Packet::packetSize] = 0;
        std::memcpy(content, packet.data.data(), Packet::packetSize);
        std::cout << "Client receives data: "<<content<<'\n';
    }
    std::array<char, Packet::packetSize+1> msg{};
    msg[Packet::packetSize] = 0;
    std::memcpy(&msg[0], &packet.data[0], Packet::packetSize);
    this->receivedMsg_ += &msg[0];
}
template<>
bool Rdt10Sender::IsComplete() const
{
    const auto& packets = channel_.GetPackets();
    return std::all_of(packets.cbegin(), packets.cend(), [](const Packet& p) {return p.currentDelay > 0.0f; });
}
template<>
bool Rdt10Receiver::IsComplete() const
{
    const auto& packets = channel_.GetPackets();
    return packets.size() == 0;
}

void Rdt10Channel::SendRaw(const Packet& packet)
{
    {
        char content[Packet::packetSize+1];
        content[Packet::packetSize] = 0;
        std::memcpy(content, packet.data.data(), Packet::packetSize);
        std::cout << "Channel send raw data: "<<content<<'\n';
    }
    Packet p = packet;
    p.totalDelay = baseDelay;
    p.currentDelay = - float(packetNmb) * pktDelay;
    packetNmb++;
    packets_.push_back(p);
}

void Rdt10Channel::Update(seconds dt)
{
    auto packetIt = packets_.begin();
    while(packetIt != packets_.end())
    {
        packetIt->currentDelay += dt.count();
        if(packetIt->currentDelay > packetIt->totalDelay)
        {
            receiver_.ReceiveRaw(*packetIt);
            packetIt = packets_.erase(packetIt);
        }
        else
        {
            ++packetIt;
        }

    }
}

Rdt10Manager::Rdt10Manager() :
    sender_(channel_), receiver_(channel_),
    channel_(sender_, receiver_), RdtManager(channel_)
{

}

}