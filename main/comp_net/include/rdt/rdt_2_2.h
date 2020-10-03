#pragma once

#include "rdt_base.h"
namespace neko::rdt
{
template<ClientType type>
class Rdt22Client : public Client<type>
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

using Rdt22Sender = Rdt22Client < ClientType::SENDER>;
using Rdt22Receiver = Rdt22Client < ClientType::RECEIVER>;

class Rdt22Channel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;

    void Update(seconds dt) override;
private:

    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class Rdt22Manager : public RdtManager
{
public:
    enum Rdt22Status
    {
        TYPE = Packet::packetSize - 3,
        SEQUENCE_NMB = Packet::packetSize - 2,
        CHECKSUM = Packet::packetSize - 1,
    };
    Rdt22Manager();
private:
    Rdt22Client<ClientType::SENDER> sender_;
    Rdt22Client<ClientType::RECEIVER> receiver_;
    Rdt22Channel channel_;
};
}