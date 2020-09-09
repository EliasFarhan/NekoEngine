#include "asteroid/rollback_manager.h"
#include "asteroid/game_manager.h"

namespace neko::asteroid
{


RollbackManager::RollbackManager(GameManager& gameManager, EntityManager& entityManager):
	gameManager_(gameManager), currentTransformManager_(entityManager),
	currentPlayerManager_(entityManager), lastValidatePlayerManager_(entityManager)
{
	for(auto& input: inputs_)
	{
		std::fill(input.begin(), input.end(), 0u);
	}
}

void RollbackManager::SimulateToCurrentFrame()
{
	auto currentFrame = gameManager_.GetCurrentFrame();
	auto lastValidateFrame = gameManager_.GetLastValidateFrame();
	std::array<PlayerCharacter, maxPlayerNmb> players;
    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
        players[playerNumber] = lastValidatePlayerManager_.GetComponent(playerEntity);
    }
	for(net::Frame frame = lastValidateFrame + 1; frame <= currentFrame; frame++)
    {
	    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            const auto playerInput = GetInputAtFrame(playerNumber, frame);
            players[playerNumber] = SimulateOneFrame(players[playerNumber], playerInput);
        }
    }
	for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
	    const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
	    currentTransformManager_.SetPosition(playerEntity, players[playerNumber].position);
	    currentTransformManager_.SetRotation(playerEntity, players[playerNumber].rotation);
	    currentPlayerManager_.SetComponent(playerEntity, players[playerNumber]);
    }
}
void RollbackManager::SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame)
{
	//Should only be called on the server
	if(currentFrame_ < inputFrame)
	{
		StartNewFrame(inputFrame);
	}
	inputs_[playerNumber][currentFrame_ - inputFrame] = playerInput;
	if(lastReceivedFrame_[playerNumber] < inputFrame)
	{
		lastReceivedFrame_[playerNumber] = inputFrame;
		//Repeat the same inputs until currentFrame
		for(size_t i = 0; i < currentFrame_-inputFrame;i++)
		{
			inputs_[playerNumber][i] = playerInput;
		}
	}
}

void RollbackManager::StartNewFrame(net::Frame newFrame)
{
	const auto delta = newFrame - currentFrame_;
	if(delta == 0)
	{
		return;
	}
	for (auto& inputs : inputs_)
	{
		for(auto i = inputs.size()-1; i >= delta; i--)
		{
			inputs[i] = inputs[i-delta];
		}

		for (std::uint32_t i = 0; i < delta; i++)
		{
			inputs[i] = inputs[delta];
		}
	}
	currentFrame_ = newFrame;
}

void RollbackManager::ValidateFrame(net::Frame newValidateFrame)
{
	for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++ )
	{
		if(GetLastReceivedFrame(playerNumber) < newValidateFrame)
		{
			neko_assert(false, "We should not validate a frame if we did not receive all inputs!!!");
			return;
		}
	}
    std::array<PlayerCharacter, maxPlayerNmb> players;
    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
        players[playerNumber] = lastValidatePlayerManager_.GetComponent(playerEntity);
    }
    for(net::Frame frame = lastValidateFrame_ + 1; frame <= newValidateFrame; frame++)
    {
        for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            const auto playerInput = GetInputAtFrame(playerNumber, frame);
            players[playerNumber] = SimulateOneFrame(players[playerNumber], playerInput);
        }
    }
    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
        lastValidatePlayerManager_.SetComponent(playerEntity, players[playerNumber]);
    }
	lastValidateFrame_ = newValidateFrame;
}
void RollbackManager::ConfirmFrame(net::Frame newValidateFrame, const std::array<PhysicsState, maxPlayerNmb>& physicsStates)
{
    ValidateFrame(newValidateFrame);
    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        const PhysicsState lastPhysicsState = GetValidatePhysicsState(playerNumber);
        if(physicsStates[playerNumber] != lastPhysicsState)
        {
            neko_assert(false, "Physics State are not equal");
        }
    }
}
PhysicsState RollbackManager::GetValidatePhysicsState(net::PlayerNumber playerNumber) const
{
	PhysicsState state = 0;
    const Entity playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
    const auto& playerCharacter = lastValidatePlayerManager_.GetComponent(playerEntity);
	auto pos = playerCharacter.position;
	const auto* posPtr = reinterpret_cast<const PhysicsState*>(&pos);
	//Adding position
	for(size_t i = 0; i < sizeof(Vec2f)/sizeof(PhysicsState); i++)
	{
		state += posPtr[i];
	}

	//Adding velocity
	auto velocity = playerCharacter.velocity;
	const auto* velocityPtr = reinterpret_cast<const PhysicsState*>(&velocity);
	for(size_t i = 0; i < sizeof(Vec2f)/sizeof(PhysicsState); i++)
    {
	    state += velocityPtr[i];
    }
    //Adding rotation
    auto angle = playerCharacter.rotation;
    const auto* anglePtr = reinterpret_cast<const PhysicsState*>(&angle);
    for (size_t i = 0; i < sizeof(float) / sizeof(PhysicsState); i++)
    {
        state += anglePtr[i];
    }

	return state;
}

void RollbackManager::SpawnPlayer(net::PlayerNumber playerNumber, Entity entity, Vec2f position, degree_t rotation)
{
    PlayerCharacter playerCharacter;
    playerCharacter.position = position;
    playerCharacter.rotation = rotation;
    currentPlayerManager_.AddComponent(entity);
    currentPlayerManager_.SetComponent(entity, playerCharacter);
    lastValidatePlayerManager_.AddComponent(entity);
    lastValidatePlayerManager_.SetComponent(entity, playerCharacter);

    currentTransformManager_.AddComponent(entity);
    currentTransformManager_.SetPosition(entity, position);
    currentTransformManager_.SetRotation(entity, rotation);
}

net::PlayerInput RollbackManager::GetInputAtFrame(net::PlayerNumber playerNumber, net::Frame frame)
{
	return inputs_[playerNumber][currentFrame_ - frame];
}

PlayerCharacter RollbackManager::SimulateOneFrame(const PlayerCharacter& playerCharacterInput, net::PlayerInput playerInput)
{
    PlayerCharacter playerCharacterOutput;

    const bool right = playerInput & PlayerInput::RIGHT;
    const bool left = playerInput & PlayerInput::LEFT;
    const bool up = playerInput & PlayerInput::UP;
    const bool down = playerInput & PlayerInput::DOWN;

    auto angularVelocity = ((left ? 1.0f : 0.0f) + (right ? -1.0f : 0.0f)) * playerAngularSpeed;

    auto& rotation = playerCharacterOutput.rotation;
    rotation = playerCharacterInput.rotation;
    rotation += angularVelocity * GameManager::FixedPeriod;

    auto dir = Vec2f::up;
    dir = dir.Rotate(-rotation);

    auto acceleration = ((down ? -1.0f : 0.0f) + (up ? 1.0f : 0.0f)) * dir;

    auto& velocity = playerCharacterOutput.velocity;
    velocity = playerCharacterInput.velocity;
    velocity += acceleration * GameManager::FixedPeriod;

    auto& position = playerCharacterOutput.position;
    position = playerCharacterInput.position;
    position += velocity * GameManager::FixedPeriod;
    return playerCharacterOutput;
}


}
