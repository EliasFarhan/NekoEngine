//
// Created by efarhan on 7/31/20.
//

#include "01_rdt/selective_repeat.h"
#include "imgui.h"

namespace neko::rdt
{

const static auto selRepPacketSize = Packet::packetSize - 3;
template<ClientType type>
void SelRepClient<type>::Send(std::string_view msg)
{
    std::cout << "Sending msg: " << msg << '\n';
    sentPackets_.clear();
    base_ = 0;

    size_t pktNumber = msg.size() / selRepPacketSize;
    if (pktNumber * selRepPacketSize != msg.size())
    {
        pktNumber++;
    }
    sentPackets_.reserve(pktNumber);
    for (size_t i = 0; i < pktNumber; i++)
    {
        Packet p;
        char checkSum = 0;
        for (size_t j = 0; j < selRepPacketSize; j++)
        {
            const auto index = i * selRepPacketSize + j;
            if (index == msg.size())
            {
                p.data[j] = 0;
                break;
            }
            const char c = msg[index];
            if (c == 0)
                break;
            p.data[j] = c;
            checkSum += c;
        }

        p.data[SelRepManager::SEQUENCE_NMB] = static_cast<int>(i) + 1 - 128;
        checkSum += p.data[SelRepManager::SEQUENCE_NMB];
        checkSum += p.data[SelRepManager::TYPE];
        p.data[SelRepManager::CHECKSUM] = ~checkSum; //Invert the checksum
        sentPackets_.push_back(p);
    }
    SendNPacket(0);
}
template<ClientType type>
void SelRepClient<type>::ClearMsg()
{
    Client<type>::ClearMsg();
    base_ = 0;
}

template<>
void SelRepSender::ReceiveRaw(const Packet& packet)
{
    if (base_ >= sentPackets_.size())
    {
        return;
    }
    bool isCorrupt = false;
    const auto packetChecksum = packet.data[SelRepManager::CHECKSUM];
    char checkSum = 0;
    for (size_t j = 0; j < SelRepManager::CHECKSUM; j++)
    {
        char c = packet.data[j];
        checkSum += c;
    }
    if (checkSum + packetChecksum != -1)
    {
        isCorrupt = true;
    }
    const auto type = packet.data[SelRepManager::TYPE];
    const auto sequenceNmb = static_cast<int>(packet.data[SelRepManager::SEQUENCE_NMB]) + 128;


    //Check corruption
    if (isCorrupt)
    {

        return;
    }
    //TODO increase base if necessary

}

template<>
void SelRepReceiver::ReceiveRaw(const Packet& packet)
{
    char content[selRepPacketSize + 1];
    content[selRepPacketSize] = 0;
    std::memcpy(content, packet.data.data(), selRepPacketSize);
    std::cout << "Client receives data: " << content << '\n';
    bool isCorrupt = false;

    const auto packetChecksum = packet.data[SelRepManager::CHECKSUM];
    char checkSum = 0;
    for (size_t j = 0; j < SelRepManager::CHECKSUM; j++)
    {
        char c = packet.data[j];
        checkSum += c;
    }
    if (checkSum + packetChecksum != -1)
    {
        isCorrupt = true;
    }
    const auto type = packet.data[SelRepManager::TYPE];
    const auto sequenceNmb = static_cast<int>(packet.data[SelRepManager::SEQUENCE_NMB]) + 128;

    // Receiver part
    if (isCorrupt)
    {
    }


}

template<ClientType type>
void SelRepClient<type>::Update(seconds dt)
{
    Client<type>::Update(dt);
    if (base_ + 1 == sentPackets_.size())
        return;
    for(auto& currentTimer : packetTimers_)
    {
        const auto index = std::distance(&packetTimers_[0], &currentTimer);
        currentTimer += dt.count();
        if (currentTimer > timerPeriod_)
        {
            SendNPacket(base_+index, 1);
        }
    }

}
template<ClientType type>
void SelRepClient<type>::SendRaw(const Packet& packet)
{
    Client<type>::SendRaw(packet);

    timerPeriod_ = this->channel_.baseDelay * 2.7f;
}
template<ClientType type>
void SelRepClient<type>::SendNPacket(int start, int n)
{
    for (int i = start; i < n + start; i++)
    {
        if (i >= sentPackets_.size())
        {
            if (nextSendNmb_ < i)
                nextSendNmb_ = i;
            return;
        }
        sentPackets_[i].currentDelay = -float(i - start) * this->channel_.pktDelay;
        packetTimers_[i-base_] = -float(i - start) * this->channel_.pktDelay;
        SendRaw(sentPackets_[i]);
        if (nextSendNmb_ < i)
            nextSendNmb_ = i;
    }
}
template <>
bool SelRepSender::IsComplete() const
{
    return base_ == sentPackets_.size();
}
template <>
bool SelRepReceiver::IsComplete() const
{
    return channel_.GetSender().IsComplete();
}
void SelRepChannel::SendRaw(const Packet& packet)
{
    {
        char content[Packet::packetSize + 1];
        content[Packet::packetSize] = 0;
        std::memcpy(content, packet.data.data(), Packet::packetSize);
        std::cout << "Channel send raw data: " << content << '\n';
    }

    Packet p = packet;
    p.totalDelay = baseDelay;
    //10% corruption chance
    if (RandomRange(0.0f, 1.0f) < 0.2f)
    {
        p.flags = p.flags | Packet::CORRUPTED;
        p.data[RandomRange<int>(0, Packet::packetSize - 1)] = RandomRange(-128, 127);
    }
    //loss chance
    if (RandomRange(0.0f, 1.0f) < 0.2f)
    {
        p.flags = p.flags | Packet::LOST;
    }
    packets_.push_back(p);

}

void SelRepChannel::Update(seconds dt)
{
    sender_.Update(dt);

    auto packetIt = packets_.begin();
    while (packetIt != packets_.end())
    {
        packetIt->currentDelay += dt.count();
        if (packetIt->currentDelay > packetIt->totalDelay)
        {
            arrivedPackets_.push_back(*packetIt);
            packetIt = packets_.erase(packetIt);
        }
        else
        {
            if (packetIt->flags & Packet::LOST && packetIt->currentDelay > packetIt->totalDelay / 2)
            {
                packetIt = packets_.erase(packetIt);
            }
            else
            {
                ++packetIt;
            }
        }
    }
    for (auto& p : arrivedPackets_)
    {
        if (p.flags & Packet::ACK)
        {
            sender_.ReceiveRaw(p);
        }
        else
        {
            receiver_.ReceiveRaw(p);
        }
    }
    arrivedPackets_.clear();
}

SelRepManager::SelRepManager() : sender_(channel_), receiver_(channel_),
channel_(sender_, receiver_), RdtManager(channel_)
{
}

void SelRepManager::DrawImGui()
{
    ImGui::Begin("Go Back N");
    ImGui::LabelText("Sender Base", "%d", sender_.base_);
    ImGui::LabelText("Sender Packets Nmb", "%ld", sender_.sentPackets_.size());
    ImGui::LabelText("Receive Base", "%d", receiver_.base_);
    ImGui::End();
}
}