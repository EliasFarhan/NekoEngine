#pragma once

#include <cstdint>

#include "game.h"
#include "comp_net/type.h"


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

template<PacketType type>
struct TypedPacket : Packet
{
    TypedPacket() { packetType = type; }
};
/**
 * \brief TCP Packet sent by a client to the server to join a game
 */
struct JoinPacket : TypedPacket<PacketType::JOIN>
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
};
/**
 * \brief TCP Packet sent by the server to the client to answer a join packet
 */
struct JoinAckPacket : TypedPacket<PacketType::JOIN_ACK>
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
    std::array<std::uint8_t, sizeof(unsigned int)> udpPort{};
};
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

//
const size_t maxInputNmb = 150;
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

struct StartGamePacket : TypedPacket<PacketType::START_GAME>
{
    std::array<std::uint8_t, sizeof(unsigned )> startTime{};
};

struct ValidateFramePacket : TypedPacket<PacketType::VALIDATE_STATE>
{
    std::array<std::uint8_t, sizeof(net::Frame)> newValidateFrame;
    std::array<std::uint8_t, sizeof(asteroid::PhysicsState) * maxPlayerNmb> physicsState;
};

class PacketSenderInterface
{
public:
    virtual void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) = 0;
    virtual void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) = 0;
};
} // namespace neko::asteroid
