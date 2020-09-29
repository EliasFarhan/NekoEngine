//
// Created by efarhan on 24.07.20.
//

#include "rdt/rdt_2_0.h"

namespace neko::rdt
{
template<ClientType type>
void Rdt20Client<type>::Send(std::string_view msg)
{
	std::cout << "Sending msg: " << msg << '\n';
	sentPackets_.clear();
	lastSequenceId_ = 0;

	size_t pktNumber = msg.size() / Packet::packetSize;
	if (pktNumber * Packet::packetSize != msg.size())
	{
		pktNumber++;
	}
	sentPackets_.reserve(pktNumber);
	for (size_t i = 0; i < pktNumber; i++)
	{
		Packet p;
		char checkSum = 0;
		for (size_t j = 0; j < Packet::packetSize; j++)
		{
			const auto index = i * Packet::packetSize + j;
			if (index == msg.size())
				break;
			const char c = msg[index];
			if (c == 0)
				break;
			p.data[j] = c;
			checkSum += c;
		}
		p.checksum = ~checkSum; //Invert the checksum
		p.sequenceNumber = i + 1;
		sentPackets_.push_back(p);
	}
	this->SendRaw(sentPackets_[0]);
}
template<ClientType type>
void Rdt20Client<type>::ClearMsg()
{
	Client<type>::ClearMsg();
	lastSequenceId_ = 0;
}

template<>
void Rdt20Sender::ReceiveRaw(const Packet& packet)
{
	if (packet.flags & Packet::ACK || packet.flags & Packet::NAK)
	{
		if (packet.flags & Packet::ACK)
		{
			lastSequenceId_++;
			if (lastSequenceId_ == sentPackets_.size())
				return;
			SendRaw(sentPackets_[lastSequenceId_]);
		}
		else if (packet.flags & Packet::NAK)
		{
			SendRaw(sentPackets_[lastSequenceId_]);
		}
		return;
	}
}
template<>
void Rdt20Receiver::ReceiveRaw(const Packet& packet)
{
	{
		char content[Packet::packetSize + 1];
		content[Packet::packetSize] = 0;
		std::memcpy(content, packet.data.data(), Packet::packetSize);
		std::cout << "Client receives data: " << content << '\n';
	}
	
	char checkSum = 0;
	for (size_t j = 0; j < Packet::packetSize; j++)
	{
		char c = packet.data[j];
		if (c == 0)
			break;
		checkSum += c;
	}
	if (checkSum + packet.checksum != -1)
	{
		std::cout << "Check sum value: " << checkSum <<
			", packet Checksum: " << checkSum << " total: " << checkSum + packet.checksum << '\n';
		Packet p;
		p.flags = Packet::NAK;
		channel_.SendRaw(p);
		return;
	}

	if (packet.sequenceNumber != lastSequenceId_ + 1)
	{
		//We wait for the next packet, so this one is discard
		neko_assert(false, "This should not happen!");
		return;
	}
	std::array<char, Packet::packetSize + 1> msg{};
	msg[Packet::packetSize] = 0;
	std::memcpy(&msg[0], &packet.data[0], Packet::packetSize);
	receivedMsg_ += &msg[0];
	lastSequenceId_ = packet.sequenceNumber;

	Packet p;
	p.flags = Packet::ACK;
	channel_.SendRaw(p);
}

template <>
bool Rdt20Sender::IsComplete() const
{
	return lastSequenceId_ == sentPackets_.size();
}
template <>
bool Rdt20Receiver::IsComplete() const
{
	return channel_.GetSender().IsComplete();
}


void Rdt20Channel::SendRaw(const Packet& packet)
{
	{
		char content[Packet::packetSize + 1];
		content[Packet::packetSize] = 0;
		std::memcpy(content, packet.data.data(), Packet::packetSize);
		std::cout << "Channel send raw data: " << content << '\n';
	}

	Packet p = packet;
	p.totalDelay = baseDelay;
	p.currentDelay = pktDelay;
	//10% corruption chance
	if (RandomRange(0.0f, 1.0f) < 0.5f)
	{
		p.flags = p.flags | Packet::CORRUPTED;
		p.data[RandomRange<int>(0, Packet::packetSize - 1)] = RandomRange(-128, 127);
	}
	packets_.push_back(p);

}

void Rdt20Channel::Update(seconds dt)
{
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
			++packetIt;
		}

	}
	for (auto& p : arrivedPackets_)
	{
		if (p.flags & Packet::ACK || p.flags & Packet::NAK)
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

Rdt20Manager::Rdt20Manager() : sender_(channel_), receiver_(channel_),
channel_(sender_, receiver_), RdtManager(channel_)
{
}
}