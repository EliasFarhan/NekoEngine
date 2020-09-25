#include "asteroid/game_manager.h"
#include "engine/engine.h"
#include "imgui.h"
#include "asteroid/rollback_manager.h"

namespace neko::asteroid
{

GameManager::GameManager() :
	transformManager_(entityManager_),
	rollbackManager_(*this, entityManager_)
{
}

void GameManager::Init()
{
    /*
    for(net::PlayerNumber i = 0; i < maxPlayerNmb ; i++)
    {
        playerCharacters_.emplace_back(*this);
    }
     */
	//playerCharacters_.resize(4, PlayerCharacter{ *this });
}

void GameManager::Update(seconds dt)
{
}

void GameManager::Destroy()
{

}

void GameManager::SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation)
{
	const auto entity = entityManager_.CreateEntity(playerNumber);
	entityMap_[playerNumber] = entity;
	entityManager_.AddComponentType(entity, static_cast<EntityMask>(ComponentType::PLAYER_CHARACTER));
	transformManager_.AddComponent(entity);
	transformManager_.SetPosition(entity, position);
    transformManager_.SetRotation(entity, rotation);
	rollbackManager_.SpawnPlayer(playerNumber, entity, position, degree_t(rotation));
}

Entity GameManager::GetEntityFromPlayerNumber(net::PlayerNumber playerNumber) const
{
	return entityMap_[playerNumber];
}

ClientGameManager::ClientGameManager(PacketSenderInterface& packetSenderInterface) :
    GameManager(),
    spriteManager_(*this, entityManager_, transformManager_),
    packetSenderInterface_(packetSenderInterface)
{
}

void ClientGameManager::Init()
{
	camera_.position = Vec3f::back;
	camera_.WorldLookAt(Vec3f::zero);
	camera_.nearPlane = 0.0f;
	camera_.farPlane = 2.0f;
	spriteManager_.Init();
	GameManager::Init();
}

void ClientGameManager::Update(seconds dt)
{
    textureManager_.Update();
    spriteManager_.Update(dt);
	fixedTimer_ += dt.count();
	while (fixedTimer_> FixedPeriod)
	{
		FixedUpdate();
		fixedTimer_ -= FixedPeriod;
	}
}

void ClientGameManager::Destroy()
{
	GameManager::Destroy();
    textureManager_.Destroy();
	spriteManager_.Destroy();
}

void ClientGameManager::SetWindowSize(Vec2u windowsSize)
{
	windowSize_ = windowsSize;
	camera_.SetSize(Vec2f(windowsSize) / PixelPerUnit);
}

void ClientGameManager::Render()
{
	glViewport(0, 0, windowSize_.x, windowSize_.y);
	spriteManager_.Render();
}

void ClientGameManager::SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation)
{
	logDebug("Spawn player: " + std::to_string(playerNumber));
	GameManager::SpawnPlayer(playerNumber, position, rotation);
	const auto entity = GetEntityFromPlayerNumber(playerNumber);
	entityManager_.AddComponentType(entity, EntityMask(neko::ComponentType::SPRITE2D));

}

void ClientGameManager::FixedUpdate()
{
    if(!(state_ & STARTED))
    {
        if(startingTime_ != 0)
        {
            using namespace std::chrono;
            const auto ms = duration_cast< milliseconds >(
                    system_clock::now().time_since_epoch()
            ).count();
            if(ms > startingTime_)
            {
                state_ = state_ | STARTED;
            }
            else
            {
                return;
            }
        }
        else
        {
            return;
        }
    }


    rollbackManager_.SimulateToCurrentFrame();
    //Copy rollback transform position to our own
	for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
	{
	    Entity playerEntity = GetEntityFromPlayerNumber(playerNumber);
		if (playerEntity != INVALID_ENTITY &&
			entityManager_.HasComponent(playerEntity,
			static_cast<EntityMask>(
				ComponentType::PLAYER_CHARACTER)))
		{
		    transformManager_.SetPosition(playerEntity, rollbackManager_.GetTransformManager().GetPosition(playerEntity));
		    transformManager_.SetRotation(playerEntity, rollbackManager_.GetTransformManager().GetRotation(playerEntity));
		}
	}
    //We send the player inputs when the game started

    const auto& inputs = rollbackManager_.GetInputs(GetPlayerNumber());
    const auto* framePtr = reinterpret_cast<std::uint8_t*>(&currentFrame_);

    std::unique_ptr<asteroid::PlayerInputPacket> playerInputPacket = std::make_unique<asteroid::PlayerInputPacket>();
    playerInputPacket->packetType = asteroid::PacketType::INPUT;
    playerInputPacket->playerNumber = GetPlayerNumber();
    for(size_t i = 0; i < sizeof(net::Frame);i++)
    {
        playerInputPacket->currentFrame[i] = framePtr[i];
    }
    for (size_t i = 0; i < playerInputPacket->inputs.size(); i++)
    {
        if(i > currentFrame_)
        {
            break;
        }

        playerInputPacket->inputs[i] = inputs[i];
    }
    packetSenderInterface_.SendPacket(std::move(playerInputPacket));


    currentFrame_++;
	rollbackManager_.StartNewFrame(currentFrame_);
}

void GameManager::SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame)
{
	if (playerNumber == net::INVALID_PLAYER)
		return;

	rollbackManager_.SetPlayerInput(playerNumber, playerInput, inputFrame);
	
}
void GameManager::Validate(net::Frame newValidateFrame)
{
	if(rollbackManager_.GetCurrentFrame() < newValidateFrame)
	{
		rollbackManager_.StartNewFrame(newValidateFrame);
	}
    rollbackManager_.ValidateFrame(newValidateFrame);
}
void ClientGameManager::SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame)
{
	if (playerNumber == net::INVALID_PLAYER)
		return;
	GameManager::SetPlayerInput(playerNumber, playerInput, inputFrame);
}

void ClientGameManager::StartGame(long startingTIme)
{
    logDebug("Start game at starting time: "+std::to_string(startingTIme));
    startingTime_ = startingTIme;
}

void ClientGameManager::DrawImGui()
{
    ImGui::Text(state_ & STARTED ? "Game has started" : "Game has not started");
	if(startingTime_ != 0)
	{
		ImGui::Text("Starting Time: %u", startingTime_);
		using namespace std::chrono;
		const auto ms = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
			).count();
		ImGui::Text("Current Time: %u", ms);
	}
}

void ClientGameManager::ConfirmValidateFrame(net::Frame newValidateFrame,
                                             const std::array<PhysicsState, maxPlayerNmb>& physicsStates)
{
    if(newValidateFrame < rollbackManager_.GetLastValidateFrame())
    {
        return;
    }
    for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        if(rollbackManager_.GetLastReceivedFrame(playerNumber) < newValidateFrame)
        {
            return;
        }
    }
    rollbackManager_.ConfirmFrame(newValidateFrame, physicsStates);
}




}
