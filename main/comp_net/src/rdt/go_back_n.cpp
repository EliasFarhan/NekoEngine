//
// Created by efarhan on 7/31/20.
//

#include "rdt/go_back_n.h"
#include "imgui.h"

namespace neko::rdt
{

const static auto goBackNPacketSize = Packet::packetSize - 3;
template<ClientType type>
void GoBackNClient<type>::Send(std::string_view msg)
{
	std::cout << "Sending msg: " << msg << '\n';
	sentPackets_.clear();
	base_ = 0;

	size_t pktNumber = msg.size() / goBackNPacketSize;
	if (pktNumber * goBackNPacketSize != msg.size())
	{
		pktNumber++;
	}
	sentPackets_.reserve(pktNumber);
	for (size_t i = 0; i < pktNumber; i++)
	{
		Packet p;
		char checkSum = 0;
		for (size_t j = 0; j < goBackNPacketSize; j++)
		{
			const auto index = i * goBackNPacketSize + j;
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

		p.data[GoBackNManager::SEQUENCE_NMB] = static_cast<int>(i) + 1 - 128;
		checkSum += p.data[GoBackNManager::SEQUENCE_NMB];
		checkSum += p.data[GoBackNManager::TYPE];
		p.data[GoBackNManager::CHECKSUM] = ~checkSum; //Invert the checksum
		sentPackets_.push_back(p);
	}
	SendNPacket(0);
}
template<ClientType type>
void GoBackNClient<type>::ClearMsg()
{
	Client<type>::ClearMsg();
	base_ = 0;
}

template<>
void GoBackNSender::ReceiveRaw(const Packet& packet)
{
	if (base_ >= sentPackets_.size())
	{
		return;
	}
	bool isCorrupt = false;

	const auto packetChecksum = packet.data[GoBackNManager::CHECKSUM];
	char checkSum = 0;
	for (size_t j = 0; j < GoBackNManager::CHECKSUM; j++)
	{
		char c = packet.data[j];
		checkSum += c;
	}
	if (checkSum + packetChecksum != -1)
	{
		isCorrupt = true;
	}
	const auto type = packet.data[GoBackNManager::TYPE];
	const auto sequenceNmb = static_cast<int>(packet.data[GoBackNManager::SEQUENCE_NMB]) + 128;


	//Check corruption
	if (isCorrupt)
	{
		SendRaw(sentPackets_[base_]);
		return;
	}

	if (sequenceNmb > base_)
	{
		base_ = sequenceNmb;
		if (base_ >= sentPackets_.size())
			return;
		SendNPacket(nextSendNmb_, windowSize - (nextSendNmb_ - base_));
	}
	//something happened with packet seqNmb base
	else
	{
		//Resend all packet after the receive
		SendNPacket(sequenceNmb);
	}
}

template<>
void GoBackNReceiver::ReceiveRaw(const Packet& packet)
{
	{
		char content[goBackNPacketSize + 1];
		content[goBackNPacketSize] = 0;
		std::memcpy(content, packet.data.data(), goBackNPacketSize);
		std::cout << "Client receives data: " << content << '\n';
	}
	bool isCorrupt = false;

	const auto packetChecksum = packet.data[GoBackNManager::CHECKSUM];
	char checkSum = 0;
	for (size_t j = 0; j < GoBackNManager::CHECKSUM; j++)
	{
		char c = packet.data[j];
		checkSum += c;
	}
	if (checkSum + packetChecksum != -1)
	{
		isCorrupt = true;
	}
	const auto type = packet.data[GoBackNManager::TYPE];
	const auto sequenceNmb = static_cast<int>(packet.data[GoBackNManager::SEQUENCE_NMB]) + 128;

	// Receiver part
	if (isCorrupt)
	{
		std::cout << "Check sum value: " << static_cast<int>(checkSum) <<
			", packet Checksum: " << static_cast<int>(packetChecksum) << " total: " << checkSum + packetChecksum
			<< '\n';
		Packet p;
		p.flags = Packet::ACK;
		p.data[GoBackNManager::TYPE] = Packet::ACK;
		p.data[GoBackNManager::SEQUENCE_NMB] = static_cast<int>(base_) - 128;
		p.data[GoBackNManager::CHECKSUM] = ~(p.data[GoBackNManager::SEQUENCE_NMB] + p.data[GoBackNManager::TYPE]);
		channel_.SendRaw(p);
		return;
	}

	if (sequenceNmb != base_ + 1)
	{
		/*
		 * Discard wrongly ordered packet
		*/
		return;
	}

	std::array<char, goBackNPacketSize + 1> msg{};
	msg[goBackNPacketSize] = 0;
	std::memcpy(&msg[0], &packet.data[0], goBackNPacketSize);
	receivedMsg_ += &msg[0];
	base_++;

	Packet p;
	p.flags = Packet::ACK;
	p.data[GoBackNManager::TYPE] = Packet::ACK;
	p.data[GoBackNManager::SEQUENCE_NMB] = static_cast<int>(base_) - 128;
	p.data[GoBackNManager::CHECKSUM] = ~(p.data[GoBackNManager::SEQUENCE_NMB] + p.data[GoBackNManager::TYPE]);
	channel_.SendRaw(p);
}
template<ClientType type>
void GoBackNClient<type>::Update(seconds dt)
{
	Client<type>::Update(dt);
	if (base_ == sentPackets_.size())
		return;
	currentTimer_ += dt.count();
	if (currentTimer_ > timerPeriod_)
	{
		SendNPacket(base_);
	}
}
template<ClientType type>
void GoBackNClient<type>::SendRaw(const Packet& packet)
{
	Client<type>::SendRaw(packet);
	currentTimer_ = 0.0f;
	timerPeriod_ = this->channel_.baseDelay * 2.7f;
}
template<ClientType type>
void GoBackNClient<type>::SendNPacket(int start, int n)
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
		SendRaw(sentPackets_[i]);
		if (nextSendNmb_ < i)
			nextSendNmb_ = i;
	}

}

template <>
bool GoBackNSender::IsComplete() const
{
	return base_ == sentPackets_.size();
}
template <>
bool GoBackNReceiver::IsComplete() const
{
	return channel_.GetSender().IsComplete();
}

void GoBackNChannel::SendRaw(const Packet& packet)
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

void GoBackNChannel::Update(seconds dt)
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

GoBackNManager::GoBackNManager() : sender_(channel_), receiver_(channel_),
channel_(sender_, receiver_), RdtManager(channel_)
{
}

void GoBackNManager::DrawImGui()
{
	ImGui::Begin("Go Back N");
	ImGui::LabelText("Sender Base", "%d", sender_.base_);
	ImGui::LabelText("Sender Packets Nmb", "%ld", sender_.sentPackets_.size());
	ImGui::LabelText("Receive Base", "%d", receiver_.base_);
	ImGui::End();
}
}