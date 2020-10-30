#pragma once

namespace neko::net
{
using PlayerNumber = std::uint8_t;
const PlayerNumber INVALID_PLAYER = std::numeric_limits<PlayerNumber>::max();
using ClientId = std::uint16_t;
using Frame = std::uint32_t;
using PlayerInput = std::uint8_t;
}