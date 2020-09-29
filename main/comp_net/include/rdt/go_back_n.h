#pragma once

#include "rdt_base.h"
namespace neko::rdt
{
template<ClientType type>
class GoBackNClient : public Client<type>
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
    static const int windowSize = 10;
private:
    friend class GoBackNManager;
    void SendNPacket(int start, int n = windowSize);
    std::vector<Packet> sentPackets_;
    int base_ = 0;
    //For debug purpose, we store the last sent packet
    int nextSendNmb_ = 0;
    float currentTimer_ = 0.0f;
    float timerPeriod_;
};

using GoBackNReceiver = GoBackNClient<ClientType::RECEIVER>;
using GoBackNSender = GoBackNClient<ClientType::SENDER>;

class GoBackNChannel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;
    void Update(seconds dt) override;
private:

    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class GoBackNManager : public RdtManager
{
public:
    enum GoBackNStatus
    {
        TYPE = Packet::packetSize - 3,
        SEQUENCE_NMB = Packet::packetSize - 2,
        CHECKSUM = Packet::packetSize - 1,
    };
    GoBackNManager();

    void DrawImGui() override;

private:
    GoBackNClient<ClientType::SENDER> sender_;
    GoBackNClient<ClientType::RECEIVER> receiver_;
    GoBackNChannel channel_;
};
}