#pragma once

#include "asteroid/game_manager.h"
#include "graphics/graphics.h"
#include "gl/framebuffer.h"
#include "mathematics/vector.h"
#include "gl/shape.h"
#include "asteroid/packet_type.h"

namespace neko::net
{
class SimulationServer;

class SimulationClient : public RenderProgram, public DrawImGuiInterface, public asteroid::PacketSenderInterface
{
public:
	explicit SimulationClient(SimulationServer& server);

	void Init() override;
	void Update(seconds dt) override;

	void Destroy() override;
	void Render() override;

	[[nodiscard]] const gl::Framebuffer& GetFramebuffer() const { return framebuffer_; }

	void SendUnreliablePacket(std::unique_ptr<asteroid::Packet> packet) override;
	void SendReliablePacket(std::unique_ptr<asteroid::Packet> packet) override;
    void ReceivePacket(const asteroid::Packet* packet);

    void DrawImGui() override;
	void SetPlayerInput(net::PlayerInput input);

private:
	asteroid::ClientGameManager gameManager_;
	gl::Framebuffer framebuffer_;
	Vec2u windowSize_;
	SimulationServer& server_;
	ClientId clientId_ = 0;

};
}
