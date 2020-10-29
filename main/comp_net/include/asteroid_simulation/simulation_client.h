#pragma once

#include "asteroid/game_manager.h"
#include "graphics/graphics.h"
#include "gl/framebuffer.h"
#include "mathematics/vector.h"
#include "gl/shape.h"
#include "asteroid/packet_type.h"
#include "asteroid/client.h"

namespace neko::net
{
class SimulationServer;

class SimulationClient : public Client
{
public:
    explicit SimulationClient(SimulationServer& server);

    void Init() override;
    void Update(seconds dt) override;

    void Destroy() override;
    void Render() override;

    
    void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) override;
    void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) override;
    

    void DrawImGui() override;
    void SetPlayerInput(net::PlayerInput input);

    void SetWindowSize(Vec2u windowSize);
private:
    SimulationServer& server_;

};
}
