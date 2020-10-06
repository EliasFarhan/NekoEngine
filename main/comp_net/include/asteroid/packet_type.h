/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#pragma once

#include <cstdint>

#include "game.h"
#include "comp_net/type.h"
#include "SFML/Network/Packet.hpp"


namespace neko::asteroid
{
enum class PacketType : std::uint8_t
{
    JOIN = 0u,
    SPAWN_PLAYER,
    INPUT,
    SPAWN_BULLET,
    VALIDATE_STATE,
    START_GAME,
    JOIN_ACK,
    NONE,
};

using PhysicsState = std::uint16_t;

struct Packet
{
    virtual ~Packet() = default;
    PacketType packetType = PacketType::NONE;
};

inline sf::Packet& operator<<(sf::Packet& packetReceived, Packet& packet)
{
    const std::uint8_t packetType = static_cast<std::uint8_t>(packet.packetType);
    packetReceived << packetType;
    return packetReceived;
}

inline sf::Packet& operator>>(sf::Packet& packetReceived, Packet& packet)
{
    std::uint8_t packetType;
    packetReceived >> packetType;
    packet.packetType = static_cast<PacketType>(packetType);
    return packetReceived;
}

template<PacketType type>
struct TypedPacket : Packet
{
    TypedPacket() { packetType = type; }
};

template<typename T, size_t N>
sf::Packet& operator<<(sf::Packet& packet, const std::array<T, N>& t)
{
	for (auto& tmp : t)
	{
		packet << tmp;
	}
	return packet;
}

template <typename T, size_t N>
sf::Packet& operator>>(sf::Packet& packet, std::array<T, N>& t) {
  for (auto& tmp : t) {
    packet >> tmp;
  }
  return packet;
}

/**
 * \brief TCP Packet sent by a client to the server to join a game
 */
struct JoinPacket : TypedPacket<PacketType::JOIN>
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
    std::array<std::uint8_t, sizeof(unsigned long)> startTime{};
};

inline sf::Packet& operator<<(sf::Packet& packet, const JoinPacket& joinPacket)
{
	return packet << joinPacket.clientId << joinPacket.startTime;
}

inline sf::Packet& operator>>(sf::Packet& packet, JoinPacket& joinPacket)
{
	return packet >> joinPacket.clientId >> joinPacket.startTime;
}
/**
 * \brief TCP Packet sent by the server to the client to answer a join packet
 */
struct JoinAckPacket : TypedPacket<PacketType::JOIN_ACK>
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
    std::array<std::uint8_t, sizeof(unsigned short)> udpPort{};
};


inline sf::Packet& operator<<(sf::Packet& packet, const JoinAckPacket& spawnPlayerPacket)
{
    return packet << spawnPlayerPacket.clientId << spawnPlayerPacket.udpPort;
}

inline sf::Packet& operator>>(sf::Packet& packet, JoinAckPacket& joinPacket)
{
    return packet >> joinPacket.clientId >> joinPacket.udpPort;
}
/**
 * \brief Packet sent by the server to all clients to notify of the spawn of a new player
 */
struct SpawnPlayerPacket : TypedPacket<PacketType::SPAWN_PLAYER>
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
    net::PlayerNumber playerNumber = net::INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(Vec2f)> pos{};
    std::array<std::uint8_t, sizeof(degree_t)> angle{};
};

inline sf::Packet& operator<<(sf::Packet& packet, const SpawnPlayerPacket& spawnPlayerPacket)
{
    return packet << spawnPlayerPacket.clientId << spawnPlayerPacket.playerNumber << 
        spawnPlayerPacket.pos << spawnPlayerPacket.angle;
}

inline sf::Packet& operator>>(sf::Packet& packet, SpawnPlayerPacket& spawnPlayerPacket)
{
    return packet >> spawnPlayerPacket.clientId >> spawnPlayerPacket.playerNumber >>
        spawnPlayerPacket.pos >> spawnPlayerPacket.angle;
}

//
const size_t maxInputNmb = 50;
/**
 * \brief Packet sent by the player client and then replicated by the server to all clients to share the currentFrame
 * and all the previous ones player inputs.
 */
struct PlayerInputPacket : TypedPacket<PacketType::INPUT>
{
    net::PlayerNumber playerNumber = net::INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(net::Frame)> currentFrame{};
    std::array<std::uint8_t, maxInputNmb> inputs{};
};

inline sf::Packet& operator<<(sf::Packet& packet, const PlayerInputPacket& playerInputPacket)
{
    return packet << playerInputPacket.playerNumber <<
        playerInputPacket.currentFrame << playerInputPacket.inputs;
}

inline sf::Packet& operator>>(sf::Packet& packet, PlayerInputPacket& playerInputPacket)
{
    return packet >> playerInputPacket.playerNumber >>
        playerInputPacket.currentFrame >> playerInputPacket.inputs;
}

struct StartGamePacket : TypedPacket<PacketType::START_GAME>
{
    std::array<std::uint8_t, sizeof(unsigned long)> startTime{};
};


inline sf::Packet& operator<<(sf::Packet& packet, const StartGamePacket& startGamePacket)
{
    return packet << startGamePacket.startTime;
}

inline sf::Packet& operator>>(sf::Packet& packet, StartGamePacket& startGamePacket)
{
    return packet >> startGamePacket.startTime;
}

struct ValidateFramePacket : TypedPacket<PacketType::VALIDATE_STATE>
{
    std::array<std::uint8_t, sizeof(net::Frame)> newValidateFrame{};
    std::array<std::uint8_t, sizeof(asteroid::PhysicsState) * maxPlayerNmb> physicsState{};
};

inline sf::Packet& operator<<(sf::Packet& packet, const ValidateFramePacket& validateFramePacket)
{
    return packet << validateFramePacket.newValidateFrame << validateFramePacket.physicsState;
}

inline sf::Packet& operator>>(sf::Packet& packet, ValidateFramePacket& ValidateFramePacket)
{
    return packet >> ValidateFramePacket.newValidateFrame >> ValidateFramePacket.physicsState;
}

inline void GeneratePacket(sf::Packet& packet, asteroid::Packet& sendingPacket)
{
    packet << sendingPacket;
	switch (sendingPacket.packetType)
	{
    case PacketType::JOIN: 
    {
        auto& packetTmp = static_cast<JoinPacket&>(sendingPacket);
		packet << packetTmp;
    	break;
    }
    case PacketType::SPAWN_PLAYER: 
    {
		auto& packetTmp = static_cast<SpawnPlayerPacket&>(sendingPacket);
		packet << packetTmp;
		break;
    }
    case PacketType::INPUT:
    {
        auto& packetTmp = static_cast<PlayerInputPacket&>(sendingPacket);
        packet << packetTmp;
        break;
    }
    case PacketType::VALIDATE_STATE:
    {
        auto& packetTmp = static_cast<ValidateFramePacket&>(sendingPacket);
        packet << packetTmp;
        break;
    }
    case PacketType::START_GAME:
    {
        auto& packetTmp = static_cast<StartGamePacket&>(sendingPacket);
        packet << packetTmp;
        break;
    }
    case PacketType::JOIN_ACK:
    {
        auto& packetTmp = static_cast<JoinAckPacket&>(sendingPacket);
        packet << packetTmp;
        break;
    }
	default: ;
	}
}
inline std::unique_ptr<Packet> GenerateReceivedPacket(sf::Packet& packet)
{
    Packet packetTmp;
    packet >> packetTmp;
    switch (packetTmp.packetType)
    {
    case PacketType::JOIN:
    {
        auto joinPacket = std::make_unique<JoinPacket>();
        joinPacket->packetType = packetTmp.packetType;
        packet >> *joinPacket;
        return joinPacket;
    }
    case PacketType::SPAWN_PLAYER:
    {
        auto spawnPlayerPacket = std::make_unique<SpawnPlayerPacket>();
        spawnPlayerPacket->packetType = packetTmp.packetType;
        packet >> *spawnPlayerPacket;
        return spawnPlayerPacket;
    }
    case PacketType::INPUT:
    {
        auto playerInputPacket = std::make_unique<PlayerInputPacket>();
        playerInputPacket->packetType = packetTmp.packetType;
        packet >> *playerInputPacket;
        return playerInputPacket;
    }
    case PacketType::VALIDATE_STATE: 
    {
        auto validateFramePacket = std::make_unique<ValidateFramePacket>();
        validateFramePacket->packetType = packetTmp.packetType;
        packet >> *validateFramePacket;
        return validateFramePacket;
    }
    case PacketType::START_GAME:
    {
        auto startGamePacket = std::make_unique<StartGamePacket>();
        startGamePacket->packetType = packetTmp.packetType;
        packet >> *startGamePacket;
        return startGamePacket;
    }
    case PacketType::JOIN_ACK:
    {
        auto joinAckPacket = std::make_unique<JoinAckPacket>();
        joinAckPacket->packetType = packetTmp.packetType;
        packet >> *joinAckPacket;
        return joinAckPacket;
    }
    default: ;
    }
    return nullptr;
}

class PacketSenderInterface
{
public:
    virtual void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) = 0;
    virtual void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) = 0;
};
} // namespace neko::asteroid
