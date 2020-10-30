#pragma once

#include "rdt_base.h"
namespace neko::rdt
{
template<ClientType type>
class Rdt21Client : public Client<type>
{
public:
    using Client<type>::Client;
    void Send(std::string_view msg) override;

    void ClearMsg() override;
    void ReceiveRaw(const Packet& packet) override;
    bool IsComplete() const override;
private:
    std::vector<Packet> sentPackets_;
    int lastSequenceId_ = 0;
};
using Rdt21Sender = Rdt21Client < ClientType::SENDER>;
using Rdt21Receiver = Rdt21Client < ClientType::RECEIVER>;
class Rdt21Channel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;

    void Update(seconds dt) override;
private:

    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class Rdt21Manager : public RdtManager
{
public:
    enum Rdt21Status
    {
    	TYPE = Packet::packetSize - 3,
        SEQUENCE_NMB = Packet::packetSize - 2,
        CHECKSUM = Packet::packetSize - 1,
    };
    Rdt21Manager();
private:
    Rdt21Client<ClientType::SENDER> sender_;
    Rdt21Client<ClientType::RECEIVER> receiver_;
    Rdt21Channel channel_;
};
}