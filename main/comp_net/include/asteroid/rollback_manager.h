#pragma once
#include "game.h"
#include "engine/transform.h"
#include "asteroid/packet_type.h"

namespace neko::asteroid
{
class GameManager;

class RollbackManager
{
public:
	explicit RollbackManager(GameManager& gameManager, EntityManager& entityManager);
	/**
	 * \brief Simulate all players with new inputs
	 */
	void SimulateToCurrentFrame();
	void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, net::Frame inputFrame);
	void StartNewFrame(net::Frame newFrame);
	void ValidateFrame(net::Frame newValidateFrame);
	void ConfirmFrame(net::Frame newValidatedFrame, const std::array<PhysicsState, maxPlayerNmb>& physicsStates);
	[[nodiscard]] PhysicsState GetValidatePhysicsState(net::PlayerNumber playerNumber) const;
	[[nodiscard]] net::Frame GetLastValidateFrame() const { return lastValidateFrame_; }
	[[nodiscard]] net::Frame GetLastReceivedFrame(net::PlayerNumber playerNumber) const { return lastReceivedFrame_[playerNumber]; }
	[[nodiscard]] net::Frame GetCurrentFrame()const { return currentFrame_; }
    [[nodiscard]] const Transform2dManager& GetTransformManager()const{ return currentTransformManager_; }
	void SpawnPlayer(net::PlayerNumber playerNumber, Entity entity, Vec2f position, degree_t rotation);
private:
    [[nodiscard]] static PlayerCharacter SimulateOneFrame(const PlayerCharacter& playerCharacter, net::PlayerInput input) ;
	net::PlayerInput GetInputAtFrame(net::PlayerNumber playerNumber, net::Frame frame);
	GameManager& gameManager_;
	Transform2dManager currentTransformManager_;
    PlayerCharacterManager currentPlayerManager_;
	PlayerCharacterManager lastValidatePlayerManager_;


	std::uint32_t lastValidateFrame_ = 0;
	std::uint32_t currentFrame_ = 0;
	static const size_t windowBufferSize = 3 * 50;
	std::array<std::uint32_t, maxPlayerNmb> lastReceivedFrame_{};
	std::array<std::array<net::PlayerInput, windowBufferSize>, maxPlayerNmb> inputs_{};
public:
	[[nodiscard]] const std::array<net::PlayerInput, windowBufferSize>& GetInputs(net::PlayerNumber playerNumber) const
	{
		return inputs_[playerNumber];
	}
};
}
