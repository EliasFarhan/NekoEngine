//
// Created by efarhan on 7/31/20.
//

#include "rdt/selective_repeat.h"
#include "imgui.h"

namespace neko::rdt
{

constexpr static auto selRepPacketSize = Packet::packetSize - 3;
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
template<ClientType type>
void SelRepClient<type>::MoveBase()
{
    const auto it = std::find(ackPackets_.begin(), ackPackets_.end(), false);
    const auto delta = std::distance(ackPackets_.begin(), it);
    for(int i = 0; i < windowSize-delta; i++)
    {
        ackPackets_[i] = ackPackets_[i+delta];
        packetTimers_[i] = packetTimers_[i+delta];
    }
    for(int i = windowSize-delta; i < windowSize; i++)
    {
        ackPackets_[i] = false;
        packetTimers_[i] = 0.0f;
    }

    base_ += delta;
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
    const auto sequenceNmb = static_cast<int>(packet.data[SelRepManager::SEQUENCE_NMB]) + 128;


    //Check corruption
    if (isCorrupt)
    {
        return;
    }
    if(sequenceNmb >= base_+1 && sequenceNmb < base_+1+windowSize)
    {
        ackPackets_[sequenceNmb-base_-1] = true;
        if(sequenceNmb == base_+1)
        {
            const auto oldBase = base_;
            MoveBase();
            const auto delta = base_-oldBase;

            SendNPacket(base_, delta);
        }
    }
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
    const auto sequenceNmb = static_cast<int>(packet.data[SelRepManager::SEQUENCE_NMB]) + 128;

    if(isCorrupt)
    {
        return;
    }
    if(sequenceNmb >= base_+1 + windowSize)
    {
        return;
    }
    if(sequenceNmb >= base_+1 && sequenceNmb < base_+1+windowSize)
    {
        ackPackets_[sequenceNmb-1-base_] = true;
        const auto it = std::find_if(receivePackets_.cbegin(), receivePackets_.cend(),
                                     [sequenceNmb](const Packet& packet){
                                         const auto seqNum = static_cast<int>(packet.data[SelRepManager::SEQUENCE_NMB]) + 128;
                                         return seqNum == sequenceNmb; });
        if(it == receivePackets_.cend())
        {
            receivePackets_.push_back(packet);
            std::sort(receivePackets_.begin(), receivePackets_.end(),[](const Packet& p1, const Packet& p2){
                const auto seqNum1 = static_cast<int>(p1.data[SelRepManager::SEQUENCE_NMB]) + 128;
                const auto seqNum2 = static_cast<int>(p2.data[SelRepManager::SEQUENCE_NMB]) + 128;

                return seqNum1 < seqNum2;
            });
        }
        if(sequenceNmb == base_+1)
        {
            const auto oldBase = base_;
            MoveBase();
            const auto delta = base_-oldBase;
            for(int i = oldBase; i < oldBase+delta; i++)
            {
                std::array<char, selRepPacketSize + 1> msg{};
                msg[selRepPacketSize] = 0;
                std::memcpy(&msg[0], &receivePackets_[i].data[0], selRepPacketSize);
                this->receivedMsg_ += &msg[0];
            }
        }
    }
    Packet p;
    p.flags = Packet::ACK;
    p.data[SelRepManager::TYPE] = Packet::ACK;
    p.data[SelRepManager::SEQUENCE_NMB] = static_cast<int>(sequenceNmb) - 128;
    p.data[SelRepManager::CHECKSUM] = ~(p.data[SelRepManager::SEQUENCE_NMB] + p.data[SelRepManager::TYPE]);
    channel_.SendRaw(p);
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
    //20% corruption chance
    if (RandomRange(0.0f, 1.0f) < 0.2f)
    {
        p.flags = p.flags | Packet::CORRUPTED;
        p.data[RandomRange<int>(0, Packet::packetSize - 1)] = RandomRange(-128, 127);
    }
    //20% loss chance
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