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
#include "asteroid/player_character.h"

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
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Validate Frame");
#endif
    if (rollbackManager_.GetCurrentFrame() < newValidateFrame)
    {
        rollbackManager_.StartNewFrame(newValidateFrame);
    }
    rollbackManager_.ValidateFrame(newValidateFrame);
}

Entity GameManager::SpawnBullet(net::PlayerNumber playerNumber, Vec2f position, Vec2f velocity)
{
    const Entity entity = entityManager_.CreateEntity();
    entityManager_.AddComponentType(entity, static_cast<EntityMask>(ComponentType::BULLET));
    transformManager_.AddComponent(entity);
    transformManager_.SetPosition(entity, position);
    transformManager_.SetScale(entity, Vec2f::one * bulletScale);
    transformManager_.SetRotation(entity, degree_t(0.0f));
    transformManager_.UpdateDirtyComponent(entity);
    rollbackManager_.SpawnBullet(playerNumber, entity, position, velocity);
    return entity;
}

void GameManager::DestroyBullet(Entity entity)
{
    rollbackManager_.DestroyEntity(entity);
}

net::PlayerNumber GameManager::CheckWinner() const
{
    int alivePlayer = 0;
    net::PlayerNumber winner = net::INVALID_PLAYER;
    const auto& playerManager = rollbackManager_.GetPlayerCharacterManager();
    for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if(!entityManager_.HasComponent(entity, EntityMask(ComponentType::PLAYER_CHARACTER)))
            continue;
        const auto& player = playerManager.GetComponent(entity);
        if(player.health > 0)
        {
            alivePlayer++;
            winner = player.playerNumber;
        }
    }

    return alivePlayer == 1 ? winner : net::INVALID_PLAYER;
}

void GameManager::WinGame(net::PlayerNumber winner)
{
    winner_ = winner;
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
    fontManager_.Init();

    const auto& config = BasicEngine::GetInstance()->config;
    fontId_ = fontManager_.LoadFont(config.dataRootPath + "font/8-bit-hud.ttf", 36);

    GameManager::Init();
}

void ClientGameManager::Update(seconds dt)
{
    std::lock_guard<std::mutex> lock(renderMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Update");
#endif
    if (state_ & STARTED)
    {
        rollbackManager_.SimulateToCurrentFrame();
        //Copy rollback transform position to our own
        for (Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
        {
            if (entityManager_.HasComponent(entity,
                EntityMask(ComponentType::PLAYER_CHARACTER) |
                EntityMask(neko::ComponentType::SPRITE2D)))
            {
                const auto& player = rollbackManager_.GetPlayerCharacterManager().GetComponent(entity);
                auto sprite = spriteManager_.GetComponent(entity);
                if (player.invincibilityTime > 0.0f)
                {
                    auto leftV = std::fmod(player.invincibilityTime, invincibilityFlashPeriod);
                    auto rightV = invincibilityFlashPeriod / 2.0f;
                    //logDebug(fmt::format("Comparing {} and {} with time: {}", leftV, rightV, player.invincibilityTime));
                }
                if (player.invincibilityTime > 0.0f &&
                    std::fmod(player.invincibilityTime, invincibilityFlashPeriod)
                    > invincibilityFlashPeriod / 2.0f)
                {
                    sprite.color = Color4(Color::black, 1.0f);
                }
                else
                {
                    sprite.color = playerColors[player.playerNumber];
                }
                spriteManager_.SetComponent(entity, sprite);
            }

            if (entityManager_.HasComponent(entity, EntityMask(neko::ComponentType::TRANSFORM2D)))
            {
                transformManager_.SetPosition(entity, rollbackManager_.GetTransformManager().GetPosition(entity));
                transformManager_.SetScale(entity, rollbackManager_.GetTransformManager().GetScale(entity));
                transformManager_.SetRotation(entity, rollbackManager_.GetTransformManager().GetRotation(entity));
                transformManager_.UpdateDirtyComponent(entity);
            }
        }
    }
    fixedTimer_ += dt.count();
    while (fixedTimer_ > FixedPeriod)
    {
        FixedUpdate();
        fixedTimer_ -= FixedPeriod;

    }


    if(state_ & FINISHED)
    {
        if(winner_ == GetPlayerNumber())
        {
            const std::string winnerText = fmt::format("You won!");
            fontManager_.RenderText(fontId_, winnerText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
        }
        else if(winner_ != net::INVALID_PLAYER)
        {
            const std::string winnerText = fmt::format("P{} won!", winner_+1);
            fontManager_.RenderText(fontId_, winnerText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
        }
        else
        {
            const std::string errorMessage = fmt::format("Error with other players");
            fontManager_.RenderText(fontId_, errorMessage, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
        }
    }
    if (!(state_ & STARTED))
    {
        if (startingTime_ != 0)
        {
            using namespace std::chrono;
            unsigned long long ms = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
                ).count();
            if (ms < startingTime_)
            {
                const std::string countDownText = fmt::format("Starts in {}", ((startingTime_ - ms) / 1000 + 1));
                fontManager_.RenderText(fontId_, countDownText, Vec2f::zero, TextAnchor::CENTER_LEFT, 1.0f, Color4(Color::white, 1.0f));
            }
        }
    }
    else
    {
        std::string health;
        const auto& playerManager = rollbackManager_.GetPlayerCharacterManager();
        for(net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            const auto playerEntity = GetEntityFromPlayerNumber(playerNumber);
            health += fmt::format("P{} health: {} ",playerNumber+1,playerManager.GetComponent(playerEntity).health);
        }
        fontManager_.RenderText(fontId_, health, Vec2f(0.0f, -40.0f), TextAnchor::TOP_LEFT, 0.75f, Color4(Color::white, 1.0f));
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
    fontManager_.Destroy();
}

void ClientGameManager::SetWindowSize(Vec2u windowsSize)
{
    windowSize_ = windowsSize;
    camera_.SetExtends(Vec2f(windowsSize) / PixelPerUnit);
    fontManager_.SetWindowSize(Vec2f(windowsSize));
}

void ClientGameManager::Render()
{
    std::lock_guard<std::mutex> lock(renderMutex_);
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Render")
#endif
    glViewport(0, 0, windowSize_.x, windowSize_.y);
    CameraLocator::provide(&camera_);
    spriteManager_.Render();
    fontManager_.Render();
}

void ClientGameManager::SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation)
{
    logDebug("Spawn player: " + std::to_string(playerNumber));
    GameManager::SpawnPlayer(playerNumber, position, rotation);
    const auto entity = GetEntityFromPlayerNumber(playerNumber);
    const auto& config = BasicEngine::GetInstance()->config;
    if (shipTextureId_ == INVALID_TEXTURE_ID)
    {
        shipTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/asteroid/ship.png");
    }
    spriteManager_.AddComponent(entity);
    spriteManager_.SetTexture(entity, shipTextureId_);
    auto sprite = spriteManager_.GetComponent(entity);
    sprite.color = playerColors[playerNumber];
    spriteManager_.SetComponent(entity, sprite);

}

Entity ClientGameManager::SpawnBullet(net::PlayerNumber playerNumber, Vec2f position, Vec2f velocity)
{
    const auto entity = GameManager::SpawnBullet(playerNumber, position, velocity);
    const auto& config = BasicEngine::GetInstance()->config;
    if (bulletTextureId_ == INVALID_TEXTURE_ID)
    {
        bulletTextureId_ = textureManager_.LoadTexture(config.dataRootPath + "sprites/asteroid/bullet.png");
    }
    spriteManager_.AddComponent(entity);
    spriteManager_.SetTexture(entity, bulletTextureId_);
    auto sprite = spriteManager_.GetComponent(entity);
    sprite.color = playerColors[playerNumber];
    spriteManager_.SetComponent(entity, sprite);
    return entity;
}


void ClientGameManager::FixedUpdate()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Game Manager Fixed Update");
#endif
    if (!(state_ & STARTED))
    {
        if (startingTime_ != 0)
        {
            using namespace std::chrono;
            unsigned long long ms = duration_cast<milliseconds>(
                system_clock::now().time_since_epoch()
                ).count();
            if (ms > startingTime_)
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
    if(state_ & FINISHED)
    {
        return;
    }
    
    //We send the player inputs when the game started

    const auto& inputs = rollbackManager_.GetInputs(GetPlayerNumber());
    auto playerInputPacket = std::make_unique<PlayerInputPacket>();
    playerInputPacket->playerNumber = GetPlayerNumber();
    playerInputPacket->currentFrame = ConvertToBinary(currentFrame_);
    for (size_t i = 0; i < playerInputPacket->inputs.size(); i++)
    {
        if (i > currentFrame_)
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
    logDebug("Start game at starting time: " + std::to_string(startingTime));
    startingTime_ = startingTime;
}

void ClientGameManager::DrawImGui()
{
    ImGui::Text(state_ & STARTED ? "Game has started" : "Game has not started");
    if (startingTime_ != 0)
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
    if (newValidateFrame < rollbackManager_.GetLastValidateFrame())
    {
        //logDebug(fmt::format("[Warning] New validate frame is too old"));
        return;
    }
    for (net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        if (rollbackManager_.GetLastReceivedFrame(playerNumber) < newValidateFrame)
        {
            /*
            logDebug(fmt::format("[Warning] Trying to validate frame {} while playerNumber {} is at input frame {}, client player {}",
                newValidateFrame,
                playerNumber + 1,
                rollbackManager_.GetLastReceivedFrame(playerNumber),
                GetPlayerNumber()+1));
            */
            return;
        }
    }
    rollbackManager_.ConfirmFrame(newValidateFrame, physicsStates);
}

void ClientGameManager::WinGame(net::PlayerNumber winner)
{
    GameManager::WinGame(winner);
    state_ = state_ | FINISHED;
}


}
