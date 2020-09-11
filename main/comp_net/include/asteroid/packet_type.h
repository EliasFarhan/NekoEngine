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
    NONE,
};

using PhysicsState = std::uint16_t;

struct Packet
{
    virtual ~Packet() = default;
    PacketType packetType = PacketType::NONE;
};
/**
 * \brief Packet sent by a client to the server to join a game
 */
struct JoinPacket : Packet
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
};
/**
 * \brief Packet sent by the server to all clients to notify of the spawn of a new player
 */
struct SpawnPlayerPacket : Packet
{
    std::array<std::uint8_t, sizeof(net::ClientId)> clientId{};
    net::PlayerNumber playerNumber = net::INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(Vec2f)> pos{};
    std::array<std::uint8_t, sizeof(degree_t)> angle{};
};


const size_t maxInputNmb = 50;
/**
 * \brief Packet sent by the player client and then replicated by the server to all clients to share the currentFrame
 * and all the previous ones player inputs.
 */
struct PlayerInputPacket : Packet
{
    net::PlayerNumber playerNumber = net::INVALID_PLAYER;
    std::array<std::uint8_t, sizeof(net::Frame)> currentFrame{};
    std::array<std::uint8_t, maxInputNmb> inputs{};
};

struct StartGamePacket : Packet
{
    std::array<std::uint8_t, sizeof(unsigned )> startTime{};
};

struct ValidateFramePacket : Packet
{
    std::array<std::uint8_t, sizeof(net::Frame)> newValidateFrame;
    std::array<std::uint8_t, sizeof(asteroid::PhysicsState) * maxPlayerNmb> physicsState;
};

class PacketSenderInterface
{
public:
    virtual void SendPacket(std::unique_ptr<asteroid::Packet> packet) = 0;
};
} // namespace neko::asteroid
