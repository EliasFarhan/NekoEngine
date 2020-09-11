#pragma once

#include "rdt_base.h"
namespace neko::rdt
{
template<ClientType type>
class Rdt20Client : public Client<type>
{
public:
    using Client<type>::Client;
    void Send(std::string_view msg) override;

    void ClearMsg() override;
    void ReceiveRaw(const Packet& packet) override;
    bool IsComplete() const override;
private:
    std::vector<Packet> sentPackets_;
    unsigned char lastSequenceId_ = 0;
};
using Rdt20Sender = Rdt20Client < ClientType::SENDER>;
using Rdt20Receiver = Rdt20Client < ClientType::RECEIVER>;
class Rdt20Channel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;

    void Update(seconds dt) override;
private:

    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class Rdt20Manager : public RdtManager
{
public:
    Rdt20Manager();
private:
    Rdt20Client<ClientType::SENDER> sender_;
    Rdt20Client<ClientType::RECEIVER> receiver_;
    Rdt20Channel channel_;
};
}