//
// Created by efarhan on 24.07.20.
//

#include "rdt/rdt_2_2.h"

namespace neko::rdt
{

const static auto rdt22PacketSize = Packet::packetSize - 3;
template<ClientType type>
void Rdt22Client<type>::Send(std::string_view msg)
{
	std::cout << "Sending msg: " << msg << '\n';
	sentPackets_.clear();
	lastSequenceId_ = 0;

	size_t pktNumber = msg.size() / rdt22PacketSize;
	if (pktNumber * rdt22PacketSize != msg.size())
	{
		pktNumber++;
	}
	sentPackets_.reserve(pktNumber);
	for (size_t i = 0; i < pktNumber; i++)
	{
		Packet p;
		char checkSum = 0;
		for (size_t j = 0; j < rdt22PacketSize; j++)
		{
			const auto index = i * rdt22PacketSize + j;
			if (index == msg.size())
				break;
			const char c = msg[index];
			if (c == 0)
				break;
			p.data[j] = c;
			checkSum += c;
		}

		p.data[Rdt22Manager::SEQUENCE_NMB] = static_cast<int>(i) + 1 - 128;
		checkSum += p.data[Rdt22Manager::SEQUENCE_NMB];
		checkSum += p.data[Rdt22Manager::TYPE];
		p.data[Rdt22Manager::CHECKSUM] = ~checkSum; //Invert the checksum
		sentPackets_.push_back(p);
	}
	this->SendRaw(sentPackets_[0]);
}
template<ClientType type>
void Rdt22Client<type>::ClearMsg()
{
	Client<type>::ClearMsg();
	lastSequenceId_ = 0;
}
template<>
void Rdt22Sender::ReceiveRaw(const Packet& packet)
{
	bool isCorrupt = false;

	const auto packetChecksum = packet.data[Rdt22Manager::CHECKSUM];
	char checkSum = 0;
	for (size_t j = 0; j < Rdt22Manager::CHECKSUM; j++)
	{
		char c = packet.data[j];
		checkSum += c;
	}
	if (checkSum + packetChecksum != -1)
	{
		isCorrupt = true;
	}
	const auto type = packet.data[Rdt22Manager::TYPE];
	const auto sequenceNmb = static_cast<int>(packet.data[Rdt22Manager::SEQUENCE_NMB]) + 128;


	//Check corruption
	if (isCorrupt)
	{
		SendRaw(sentPackets_[lastSequenceId_ - 1]);
		return;
	}

	if (type & Packet::ACK)
	{
		if (sequenceNmb == lastSequenceId_ + 1)
		{
			lastSequenceId_++;
		}
		if (lastSequenceId_ == sentPackets_.size())
			return;
		SendRaw(sentPackets_[lastSequenceId_]);
	}

}

template<>
void Rdt22Receiver::ReceiveRaw(const Packet& packet)
{
	{
		char content[rdt22PacketSize + 1];
		content[rdt22PacketSize] = 0;
		std::memcpy(content, packet.data.data(), rdt22PacketSize);
		std::cout << "Client receives data: " << content << '\n';
	}
	bool isCorrupt = false;

	const auto packetChecksum = packet.data[Rdt22Manager::CHECKSUM];
	char checkSum = 0;
	for (size_t j = 0; j < Rdt22Manager::CHECKSUM; j++)
	{
		char c = packet.data[j];
		checkSum += c;
	}
	if (checkSum + packetChecksum != -1)
	{
		isCorrupt = true;
	}
	const auto type = packet.data[Rdt22Manager::TYPE];
	const auto sequenceNmb = static_cast<int>(packet.data[Rdt22Manager::SEQUENCE_NMB]) + 128;

	
	if (isCorrupt)
	{
		std::cout << "Check sum value: " << static_cast<int>(checkSum) <<
			", packet Checksum: " << static_cast<int>(packetChecksum) << " total: " << checkSum + packetChecksum << '\n';
		Packet p;
		p.flags = Packet::ACK;
		p.data[Rdt22Manager::TYPE] = Packet::ACK;
		p.data[Rdt22Manager::SEQUENCE_NMB] = static_cast<int>(lastSequenceId_) - 128;
		p.data[Rdt22Manager::CHECKSUM] = ~(p.data[Rdt22Manager::SEQUENCE_NMB] + p.data[Rdt22Manager::TYPE]);
		channel_.SendRaw(p);
		return;
	}

	if (sequenceNmb != lastSequenceId_ + 1)
	{
		std::cout << "Packet sequence Nmb: " << sequenceNmb <<
			", last sequence Nmb: " << lastSequenceId_ << '\n';
		Packet p;
		p.flags = Packet::ACK;
		p.data[Rdt22Manager::TYPE] = Packet::ACK;
		p.data[Rdt22Manager::SEQUENCE_NMB] = static_cast<int>(lastSequenceId_) - 128;
		p.data[Rdt22Manager::CHECKSUM] = ~(p.data[Rdt22Manager::SEQUENCE_NMB] + p.data[Rdt22Manager::TYPE]);
		channel_.SendRaw(p);
		return;
	}

	std::array<char, rdt22PacketSize + 1> msg{};
	msg[rdt22PacketSize] = 0;
	std::memcpy(&msg[0], &packet.data[0], rdt22PacketSize);
	receivedMsg_ += &msg[0];
	lastSequenceId_++;

	Packet p;
	p.flags = Packet::ACK;
	p.data[Rdt22Manager::TYPE] = Packet::ACK;
	p.data[Rdt22Manager::SEQUENCE_NMB] = static_cast<int>(lastSequenceId_) - 128;
	p.data[Rdt22Manager::CHECKSUM] = ~(p.data[Rdt22Manager::SEQUENCE_NMB] + p.data[Rdt22Manager::TYPE]);
	channel_.SendRaw(p);
}

template <>
bool Rdt22Sender::IsComplete() const
{
	return lastSequenceId_ == sentPackets_.size();
}
template <>
bool Rdt22Receiver::IsComplete() const
{
	return channel_.GetSender().IsComplete();
}

void Rdt22Channel::SendRaw(const Packet& packet)
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
	if (RandomRange(0.0f, 1.0f) < 0.2f)
	{
		p.flags = p.flags | Packet::CORRUPTED;
		p.data[RandomRange<int>(0, Packet::packetSize - 1)] = RandomRange(-128, 127);
	}
	packets_.push_back(p);

}

void Rdt22Channel::Update(seconds dt)
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

Rdt22Manager::Rdt22Manager() : sender_(channel_), receiver_(channel_),
channel_(sender_, receiver_), RdtManager(channel_)
{
}
}