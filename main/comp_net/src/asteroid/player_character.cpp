#include "asteroid/player_character.h"
#include "engine/engine.h"
#include "asteroid/game_manager.h"
namespace neko::asteroid
{
/*
PlayerCharacterSystem::PlayerCharacterSystem(GameManager& gameManager) : gameManager_(gameManager)
{
}

void PlayerCharacterSystem::SetPlayerInput(PlayerInput playerInput)
{
	currentInput_ = playerInput;
}

void PlayerCharacterSystem::FixedUpdate()
{
	const auto& transformManager =
		gameManager_.GetTransformManager();

	rotation_ = transformManager.GetRotation(entity_);

	const bool right = currentInput_ & RIGHT;
	const bool left = currentInput_ & LEFT;
	const bool up = currentInput_ & UP;
	const bool down = currentInput_ & DOWN;

	rotation_ += ((left ? 1.0f : 0.0f) + (right ? -1.0f : 0.0f)) * angularSpeed * gameManager_.FixedPeriod;
	
	auto dir = Vec2f::up;
	dir = dir.Rotate(-rotation_);

	acceleration_ = ((down ? -1.0f : 0.0f) + (up ? 1.0f : 0.0f)) * dir;
	velocity_ += acceleration_ * gameManager_.FixedPeriod;

	position_ = transformManager.GetPosition(entity_);
	position_ += velocity_ * gameManager_.FixedPeriod;


}
 */
}
