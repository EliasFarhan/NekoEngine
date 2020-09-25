#pragma once
#include "player_character.h"
#include "sprite_manager.h"
#include "engine/component.h"
#include "engine/transform.h"
#include "graphics/camera.h"
#include "comp_net/type.h"
#include "asteroid/rollback_manager.h"
#include "asteroid/game.h"

namespace neko::asteroid
{
class GameManager : public SystemInterface
{
public:
	GameManager();
	
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	virtual void SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation);
	[[nodiscard]] Entity GetEntityFromPlayerNumber(net::PlayerNumber playerNumber) const;
	[[nodiscard]] net::Frame GetCurrentFrame() const { return currentFrame_; }
	[[nodiscard]] net::Frame GetLastValidateFrame() const { return rollbackManager_.GetLastValidateFrame(); }
	const Transform2dManager& GetTransformManager() const { return transformManager_; }
	const RollbackManager& GetRollbackManager() const { return rollbackManager_; }
	virtual void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame);
	/*
	 * \brief Called by the server to validate a frame
	 */
    void Validate(net::Frame newValidateFrame);
    static constexpr float PixelPerUnit = 100.0f;
	static constexpr float FixedPeriod = 0.02f; //50fps
protected:
	EntityManager entityManager_;
	Transform2dManager transformManager_;
	RollbackManager rollbackManager_;
	std::array<Entity, maxPlayerNmb> entityMap_{};
	net::Frame currentFrame_ = 0;
};

class ClientGameManager : public GameManager, public RenderCommandInterface, public DrawImGuiInterface
{
public:
    enum State : std::uint32_t
    {
        STARTED = 1u << 0u,
        FINISHED = 1u << 1u,
    };
	explicit ClientGameManager(PacketSenderInterface& packetSenderInterface);
	void StartGame(long startingTIme);
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void SetWindowSize(Vec2u windowsSize);
	[[nodiscard]] Vec2u GetWindowSize() const { return windowSize_; }
	void Render() override;
	void SetClientPlayer(net::PlayerNumber clientPlayer) { clientPlayer_ = clientPlayer; }
	[[nodiscard]] const Camera2D& GetCamera() const { return camera_; }
	void SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation) override;
	void FixedUpdate();
	void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame) override;
    void DrawImGui() override;
    void ConfirmValidateFrame(net::Frame newValidateFrame, const std::array<PhysicsState, maxPlayerNmb>& physicsStates);
	[[nodiscard]] net::PlayerNumber GetPlayerNumber() const { return clientPlayer_; }

    [[nodiscard]] std::uint32_t GetState() const {return state_;}
protected:
    PacketSenderInterface& packetSenderInterface_;
	Vec2u windowSize_;
	Camera2D camera_;
	net::PlayerNumber clientPlayer_ = net::INVALID_PLAYER;
	gl::TextureManager textureManager_;
	asteroid::SpriteManager spriteManager_;
	float fixedTimer_ = 0.0f;
	unsigned startingTime_ = 0;
	std::uint32_t state_ = 0;	

};

}
