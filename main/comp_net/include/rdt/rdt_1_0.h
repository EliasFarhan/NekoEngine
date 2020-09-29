#pragma once

#include "rdt_base.h"

namespace neko::rdt
{
template<ClientType type>
class Rdt10Client : public Client<type>
{
public:
    using Client<type>::Client;
    void Send(std::string_view msg) override;

    void ReceiveRaw(const Packet& packet) override;

    bool IsComplete() const override;
};
using Rdt10Sender = Rdt10Client < ClientType::SENDER>;
using Rdt10Receiver = Rdt10Client < ClientType::RECEIVER>;

class Rdt10Channel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;

    void Update(seconds dt) override;

private:
    size_t packetNmb = 0;
};

class Rdt10Manager : public RdtManager
{
public:
    Rdt10Manager();
private:
    Rdt10Sender sender_;
    Rdt10Receiver receiver_;
    Rdt10Channel channel_;
};
}