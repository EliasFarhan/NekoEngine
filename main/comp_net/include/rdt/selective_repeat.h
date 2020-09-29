#pragma once

#include "rdt/rdt_base.h"
namespace neko::rdt
{

const static auto selRepNPacketSize = Packet::packetSize - 3;
template<ClientType type>
class SelRepClient : public Client<type>
{
public:
    using Client<type>::Client;
    void Send(std::string_view msg) override;
    void SendRaw(const Packet& packet) override;
    void ClearMsg() override;
    void ReceiveRaw(const Packet& packet) override;
    //Only called on sender
    void Update(seconds dt) override;
	[[nodiscard]] bool IsComplete() const override;
    static const int windowSize = 5;
protected:
    friend class SelRepManager;
    void MoveBase();
    void SendNPacket(int start, int n = windowSize);

    std::vector<Packet> sentPackets_;
    std::vector<Packet> receivePackets_;
    std::array<bool, windowSize> ackPackets_{};
    int base_ = 0;
    int nextSendNmb_ = 0;
    std::array<float, windowSize> packetTimers_;
    float timerPeriod_{};
};

using SelRepReceiver = SelRepClient<ClientType::RECEIVER>;
using SelRepSender = SelRepClient<ClientType::SENDER>;

class SelRepChannel : public Channel
{
public:
    using Channel::Channel;
    void SendRaw(const Packet& packet) override;
    void Update(seconds dt) override;
private:
    size_t packetNmb = 0;
    std::vector<Packet> arrivedPackets_;
};

class SelRepManager : public RdtManager
{
public:
    enum SelRepStatus
    {
        TYPE = Packet::packetSize - 3,
        SEQUENCE_NMB = Packet::packetSize - 2,
        CHECKSUM = Packet::packetSize - 1,
    };
    SelRepManager();

    void DrawImGui() override;

private:
    SelRepSender sender_;
    SelRepReceiver receiver_;
    SelRepChannel channel_;
};
}