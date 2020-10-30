#pragma once
#include <array>
#include "asteroid/packet_type.h"
#include "asteroid/game_manager.h"

namespace neko::net
{
class Server : public asteroid::PacketSenderInterface, public SystemInterface
{
protected:
    virtual void SpawnNewPlayer(ClientId clientId, PlayerNumber playerNumber) = 0;
    virtual void ReceivePacket(std::unique_ptr<asteroid::Packet> packet);

    //Server game manager
    asteroid::GameManager gameManager_;
    PlayerNumber lastPlayerNumber_ = 0;
    std::array<ClientId, asteroid::maxPlayerNmb> clientMap_{};

};
}
