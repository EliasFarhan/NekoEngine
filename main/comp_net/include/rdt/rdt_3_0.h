#pragma once

#include "rdt_base.h"
namespace neko::rdt
{
template<ClientType type>
class Rdt30Client : public Client<type>
{
public:
    using Client<type>::Client;
    void Send(std::string_view msg) override;
    void SendRaw(const Packet &packet) override;
    void ClearMsg() override;
    void ReceiveRaw(const Packet& packet) override;
    //Only called on sender
    void Update(seconds dt) override;
    bool IsComplete() const override;
private:
    std::vector<Packet> sentPackets_;
    int lastSequenceId_ = 0;
    float currentTimer_ = 0.0f;
    float timerPeriod_;
};

using Rdt30Receiver = Rdt30Client<ClientType::RECEIVER>;
using Rdt30Sender = Rdt30Client<ClientType::SENDER>;

class Rdt30Channel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;
    void Update(seconds dt) override;
private:

    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class Rdt30Manager : public RdtManager
{
public:
    enum Rdt30Status
    {
        TYPE = Packet::packetSize - 3,
        SEQUENCE_NMB = Packet::packetSize - 2,
        CHECKSUM = Packet::packetSize - 1,
    };
    Rdt30Manager();
private:
    Rdt30Sender sender_;
    Rdt30Receiver receiver_;
    Rdt30Channel channel_;
};
}