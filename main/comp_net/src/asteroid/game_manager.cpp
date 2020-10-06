/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <engine/conversion.h>
#include "asteroid/game_manager.h"
#include "engine/engine.h"
#include "imgui.h"
#include "asteroid/rollback_manager.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::asteroid
{

GameManager::GameManager() :
	transformManager_(entityManager_),
	rollbackManager_(*this, entityManager_)
{
}

void GameManager::Init()
{
    entityMap_.fill(INVALID_ENTITY);
}

void GameManager::Update(seconds dt)
{
}

void GameManager::Destroy()
{

}

void GameManager::SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation)
{
    if (GetEntityFromPlayerNumber(playerNumber) != INVALID_ENTITY)
        return;
    logDebug("[GameManager] Spawning new player");
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

Entity GameManager::SpawnBullet(net::PlayerNumber playerNumber, Vec2f position, Vec2f velocity)
{
    Entity entity = entityManager_.CreateEntity();
    entityManager_.AddComponentType(entity, static_cast<EntityMask>(ComponentType::BULLET));
    transformManager_.AddComponent(entity);
    transformManager_.SetPosition(entity, position);
    transformManager_.SetScale(entity, Vec2f::one*bulletScale);
    transformManager_.SetRotation(entity, degree_t(0.0f));
    transformManager_.UpdateDirtyComponent(entity);
    rollbackManager_.SpawnBullet(playerNumber, entity, position, velocity);
    return entity;
}

ClientGameManager::ClientGameManager(PacketSenderInterface& packetSenderInterface) :
    GameManager(),
    spriteManager_(entityManager_, textureManager_, transformManager_),
    packetSenderInterface_(packetSenderInterface)
{
}

void ClientGameManager::Init()
{
	camera_.position = Vec3f::back;
	camera_.WorldLookAt(Vec3f::zero);
	camera_.nearPlane = 0.0f;
	camera_.farPlane = 2.0f;

	textureManager_.Init();
	spriteManager_.Init();

	GameManager::Init();
}

void ClientGameManager::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(renderMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Update");
#endif
	fixedTimer_ += dt.count();
	while (fixedTimer_> FixedPeriod)
	{
		FixedUpdate();
		fixedTimer_ -= FixedPeriod;
	}
	textureManager_.Update(dt);
    spriteManager_.Update(dt);
    transformManager_.Update();
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
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Render");
#endif
    std::lock_guard<std::mutex> lock(renderMutex_);
	glViewport(0, 0, windowSize_.x, windowSize_.y);
    CameraLocator::provide(&camera_);
	spriteManager_.Render();
}

void ClientGameManager::SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation)
{
	logDebug("Spawn player: " + std::to_string(playerNumber));
	GameManager::SpawnPlayer(playerNumber, position, rotation);
	const auto entity = GetEntityFromPlayerNumber(playerNumber);
	const auto& config = BasicEngine::GetInstance()->config;
	if(shipTextureId_ == INVALID_TEXTURE_ID)
    {
	    shipTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/asteroid/ship.png");
    }
	spriteManager_.AddComponent(entity);
	spriteManager_.SetTexture(entity, shipTextureId_);

}

Entity ClientGameManager::SpawnBullet(net::PlayerNumber playerNumber, Vec2f position, Vec2f velocity)
{
    auto entity = GameManager::SpawnBullet(playerNumber, position, velocity);
    const auto& config = BasicEngine::GetInstance()->config;
    if(bulletTextureId_ == INVALID_TEXTURE_ID)
    {
        bulletTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/asteroid/bullet.png");
    }
    spriteManager_.AddComponent(entity);
    spriteManager_.SetTexture(entity, bulletTextureId_);
    return entity;
}


void ClientGameManager::FixedUpdate()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Fixed Update");
#endif
    if(!(state_ & STARTED))
    {
        if(startingTime_ != 0)
        {
            using namespace std::chrono;
            unsigned long long ms = duration_cast< milliseconds >(
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
	for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
	{
        if (!entityManager_.HasComponent(entity, EntityMask(neko::ComponentType::TRANSFORM2D)))
            continue;
        transformManager_.SetPosition(entity, rollbackManager_.GetTransformManager().GetPosition(entity));
        transformManager_.SetScale(entity, rollbackManager_.GetTransformManager().GetScale(entity));
        transformManager_.SetRotation(entity, rollbackManager_.GetTransformManager().GetRotation(entity));
        transformManager_.UpdateDirtyComponent(entity);
    }
    //We send the player inputs when the game started

    const auto& inputs = rollbackManager_.GetInputs(GetPlayerNumber());
    std::unique_ptr<asteroid::PlayerInputPacket> playerInputPacket = std::make_unique<asteroid::PlayerInputPacket>();
    playerInputPacket->playerNumber = GetPlayerNumber();
    playerInputPacket->currentFrame = ConvertToBinary(currentFrame_);
    for (size_t i = 0; i < playerInputPacket->inputs.size(); i++)
    {
        if(i > currentFrame_)
        {
            break;
        }

        playerInputPacket->inputs[i] = inputs[i];
    }
    packetSenderInterface_.SendUnreliablePacket(std::move(playerInputPacket));


    currentFrame_++;
	rollbackManager_.StartNewFrame(currentFrame_);
}


void ClientGameManager::SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame)
{
	if (playerNumber == net::INVALID_PLAYER)
		return;
	GameManager::SetPlayerInput(playerNumber, playerInput, inputFrame);
}

void ClientGameManager::StartGame(unsigned long long int startingTime)
{
    logDebug("Start game at starting time: "+std::to_string(startingTime));
    startingTime_ = startingTime;
}

void ClientGameManager::DrawImGui()
{
    ImGui::Text(state_ & STARTED ? "Game has started" : "Game has not started");
	if(startingTime_ != 0)
	{
		ImGui::Text("Starting Time: %llu", startingTime_);
		using namespace std::chrono;
        unsigned long long ms = duration_cast<milliseconds>(
			system_clock::now().time_since_epoch()
			).count();
		ImGui::Text("Current Time: %llu", ms);
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
