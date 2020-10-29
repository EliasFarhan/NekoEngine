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
#include "asteroid/rollback_manager.h"
#include "asteroid/game_manager.h"

#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif

namespace neko::asteroid
{


RollbackManager::RollbackManager(GameManager& gameManager, EntityManager& entityManager) :
    gameManager_(gameManager), entityManager_(entityManager),
    currentTransformManager_(entityManager),
    currentPhysicsManager_(entityManager), currentPlayerManager_(entityManager, currentPhysicsManager_, gameManager_),
    currentBulletManager_(entityManager, gameManager),
    lastValidatePhysicsManager_(entityManager),
    lastValidatePlayerManager_(entityManager, lastValidatePhysicsManager_, gameManager_), lastValidateBulletManager_(entityManager, gameManager)
{
    for (auto& input : inputs_)
    {
        std::fill(input.begin(), input.end(), 0u);
    }
    lastValidatePhysicsManager_.RegisterCollisionListener(*this);
}

void RollbackManager::SimulateToCurrentFrame()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Simulate To Current Frame");
#endif
    const auto currentFrame = gameManager_.GetCurrentFrame();
    const auto lastValidateFrame = gameManager_.GetLastValidateFrame();
    //Destroying all created Entities after the last validated frame
    for (const auto& createdEntity : createdEntities_)
    {
        if (createdEntity.createdFrame > lastValidateFrame)
        {
            entityManager_.DestroyEntity(createdEntity.entity);
        }
    }
    createdEntities_.clear();
    //Remove DESTROY flags
    for (Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if (entityManager_.HasComponent(entity, EntityMask(ComponentType::DESTROYED)))
        {
            entityManager_.RemoveComponentType(entity, EntityMask(ComponentType::DESTROYED));
        }
    }

    createdEntities_.clear();
    //Revert the current game state to the last validated game state
    currentBulletManager_ = lastValidateBulletManager_;
    currentPhysicsManager_ = lastValidatePhysicsManager_;
    currentPlayerManager_ = lastValidatePlayerManager_;

    for (net::Frame frame = lastValidateFrame + 1; frame <= currentFrame; frame++)
    {
        testedFrame_ = frame;
        //Copy player inputs to player manager
        for (net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            const auto playerInput = GetInputAtFrame(playerNumber, frame);
            const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
            auto playerCharacter = currentPlayerManager_.GetComponent(playerEntity);
            playerCharacter.input = playerInput;
            currentPlayerManager_.SetComponent(playerEntity, playerCharacter);
        }
        //Simulate one frame of the game
        currentBulletManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
        currentPlayerManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
        currentPhysicsManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
    }
    //Copy the physics states to the transforms
    for (Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if (!entityManager_.HasComponent(entity,
            EntityMask(neko::ComponentType::BODY2D) |
            EntityMask(neko::ComponentType::TRANSFORM2D)))
            continue;
        const auto body = currentPhysicsManager_.GetBody(entity);
        currentTransformManager_.SetPosition(entity, body.position);
        currentTransformManager_.SetRotation(entity, body.rotation);
        currentTransformManager_.UpdateDirtyComponent(entity);
    }
}
void RollbackManager::SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame)
{
    //Should only be called on the server
    if (currentFrame_ < inputFrame)
    {
        StartNewFrame(inputFrame);
    }
    inputs_[playerNumber][currentFrame_ - inputFrame] = playerInput;
    if (lastReceivedFrame_[playerNumber] < inputFrame)
    {
        lastReceivedFrame_[playerNumber] = inputFrame;
        //Repeat the same inputs until currentFrame
        for (size_t i = 0; i < currentFrame_ - inputFrame; i++)
        {
            inputs_[playerNumber][i] = playerInput;
        }
    }
}

void RollbackManager::StartNewFrame(net::Frame newFrame)
{
    if (currentFrame_ > newFrame)
        return;
    const auto delta = newFrame - currentFrame_;
    if (delta == 0)
    {
        return;
    }
    for (auto& inputs : inputs_)
    {
        for (auto i = inputs.size() - 1; i >= delta; i--)
        {
            inputs[i] = inputs[i - delta];
        }

        for (net::Frame i = 0; i < delta; i++)
        {
            inputs[i] = inputs[delta];
        }
    }
    currentFrame_ = newFrame;
}

void RollbackManager::ValidateFrame(net::Frame newValidateFrame)
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Validate Frame");
#endif
    const auto lastValidateFrame = gameManager_.GetLastValidateFrame();
    //Destroying all created Entities after the last validated frame
    for (const auto& createdEntity : createdEntities_)
    {
        if (createdEntity.createdFrame > lastValidateFrame)
        {
            entityManager_.DestroyEntity(createdEntity.entity);
        }
    }
    createdEntities_.clear();
    //Remove DESTROYED flag
    for (Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if (entityManager_.HasComponent(entity, EntityMask(ComponentType::DESTROYED)))
        {
            entityManager_.RemoveComponentType(entity, EntityMask(ComponentType::DESTROYED));
        }

    }
    createdEntities_.clear();
    //We check that we got all the inputs
    for (net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        if (GetLastReceivedFrame(playerNumber) < newValidateFrame)
        {
            neko_assert(false, "We should not validate a frame if we did not receive all inputs!!!");
            return;
        }
    }
    //We use the current game state as the temporary new validate game state
    currentBulletManager_ = lastValidateBulletManager_;
    currentPhysicsManager_ = lastValidatePhysicsManager_;
    currentPlayerManager_ = lastValidatePlayerManager_;

    //We simulate the frames until the new validated frame
    for (net::Frame frame = lastValidateFrame_ + 1; frame <= newValidateFrame; frame++)
    {
        testedFrame_ = frame;
        //Copy the players inputs into the player manager
        for (net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
        {
            const auto playerInput = GetInputAtFrame(playerNumber, frame);
            const auto playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
            auto playerCharacter = currentPlayerManager_.GetComponent(playerEntity);
            playerCharacter.input = playerInput;
            currentPlayerManager_.SetComponent(playerEntity, playerCharacter);
        }
        //We simulate one frame
        currentBulletManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
        currentPlayerManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
        currentPhysicsManager_.FixedUpdate(seconds(GameManager::FixedPeriod));
    }
    //Definitely remove DESTROY entities
    for (Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
    {
        if (entityManager_.HasComponent(entity, EntityMask(ComponentType::DESTROYED)))
        {
            entityManager_.DestroyEntity(entity);
        }
    }
    //Copy back the new validate game state to the last validated game state
    lastValidateBulletManager_ = currentBulletManager_;
    lastValidatePlayerManager_ = currentPlayerManager_;
    lastValidatePhysicsManager_ = currentPhysicsManager_;
    lastValidateFrame_ = newValidateFrame;
    createdEntities_.clear();
}
void RollbackManager::ConfirmFrame(net::Frame newValidateFrame, const std::array<PhysicsState, maxPlayerNmb>& serverPhysicsState)
{
    ValidateFrame(newValidateFrame);
    for (net::PlayerNumber playerNumber = 0; playerNumber < maxPlayerNmb; playerNumber++)
    {
        const PhysicsState lastPhysicsState = GetValidatePhysicsState(playerNumber);
        if (serverPhysicsState[playerNumber] != lastPhysicsState)
        {
            neko_assert(false, "Physics State are not equal");
        }
    }
}
PhysicsState RollbackManager::GetValidatePhysicsState(net::PlayerNumber playerNumber) const
{
    PhysicsState state = 0;
    const Entity playerEntity = gameManager_.GetEntityFromPlayerNumber(playerNumber);
    const auto& playerBody = lastValidatePhysicsManager_.GetBody(playerEntity);

    const auto pos = playerBody.position;
    const auto* posPtr = reinterpret_cast<const PhysicsState*>(&pos);
    //Adding position
    for (size_t i = 0; i < sizeof(Vec2f) / sizeof(PhysicsState); i++)
    {
        state += posPtr[i];
    }

    //Adding velocity
    const auto velocity = playerBody.velocity;
    const auto* velocityPtr = reinterpret_cast<const PhysicsState*>(&velocity);
    for (size_t i = 0; i < sizeof(Vec2f) / sizeof(PhysicsState); i++)
    {
        state += velocityPtr[i];
    }
    //Adding rotation
    const auto angle = playerBody.rotation.value();
    const auto* anglePtr = reinterpret_cast<const PhysicsState*>(&angle);
    for (size_t i = 0; i < sizeof(float) / sizeof(PhysicsState); i++)
    {
        state += anglePtr[i];
    }
    //Adding angular Velocity
    const auto angularVelocity = playerBody.angularVelocity.value();
    const auto* angularVelPtr = reinterpret_cast<const PhysicsState*>(&angularVelocity);
    for (size_t i = 0; i < sizeof(float) / sizeof(PhysicsState); i++)
    {
        state += angularVelPtr[i];
    }
    return state;
}

void RollbackManager::SpawnPlayer(net::PlayerNumber playerNumber, Entity entity, Vec2f position, degree_t rotation)
{
    Body playerBody;
    playerBody.position = position;
    playerBody.rotation = rotation;
    Box playerBox;
    playerBox.extends = Vec2f::one * 0.5f;

    PlayerCharacter playerCharacter;
    playerCharacter.playerNumber = playerNumber;

    currentPlayerManager_.AddComponent(entity);
    currentPlayerManager_.SetComponent(entity, playerCharacter);

    currentPhysicsManager_.AddBody(entity);
    currentPhysicsManager_.SetBody(entity, playerBody);
    currentPhysicsManager_.AddBox(entity);
    currentPhysicsManager_.SetBox(entity, playerBox);

    lastValidatePlayerManager_.AddComponent(entity);
    lastValidatePlayerManager_.SetComponent(entity, playerCharacter);

    lastValidatePhysicsManager_.AddBody(entity);
    lastValidatePhysicsManager_.SetBody(entity, playerBody);
    lastValidatePhysicsManager_.AddBox(entity);
    lastValidatePhysicsManager_.SetBox(entity, playerBox);

    currentTransformManager_.AddComponent(entity);
    currentTransformManager_.SetPosition(entity, position);
    currentTransformManager_.SetRotation(entity, rotation);
}

net::PlayerInput RollbackManager::GetInputAtFrame(net::PlayerNumber playerNumber, net::Frame frame)
{
    neko_assert(currentFrame_ - frame <= inputs_[playerNumber].size(),
        "Trying to get input too far in the past");
    return inputs_[playerNumber][currentFrame_ - frame];
}

void RollbackManager::OnCollision(Entity entity1, Entity entity2)
{
    std::function<void(const PlayerCharacter&, Entity, const Bullet&, Entity)> ManageCollision =
        [this](const auto& player, auto playerEntity, const auto& bullet, auto bulletEntity)
    {
        if (player.playerNumber != bullet.playerNumber)
        {
            gameManager_.DestroyBullet(bulletEntity);
            //lower health point
            auto playerCharacter = currentPlayerManager_.GetComponent(playerEntity);
            if (playerCharacter.invincibilityTime <= 0.0f)
            {
                playerCharacter.health--;
                playerCharacter.invincibilityTime = playerInvincibilityPeriod;
            }
            currentPlayerManager_.SetComponent(playerEntity, playerCharacter);
        }
    };
    if (entityManager_.HasComponent(entity1, EntityMask(ComponentType::PLAYER_CHARACTER)) &&
        entityManager_.HasComponent(entity2, EntityMask(ComponentType::BULLET)))
    {
        const auto& player = currentPlayerManager_.GetComponent(entity1);
        const auto& bullet = currentBulletManager_.GetComponent(entity2);
        ManageCollision(player, entity1, bullet, entity2);

    }
    if (entityManager_.HasComponent(entity2, EntityMask(ComponentType::PLAYER_CHARACTER)) &&
        entityManager_.HasComponent(entity1, EntityMask(ComponentType::BULLET)))
    {
        const auto& player = currentPlayerManager_.GetComponent(entity2);
        const auto& bullet = currentBulletManager_.GetComponent(entity1);
        ManageCollision(player, entity2, bullet, entity1);
    }
}

void RollbackManager::SpawnBullet(net::PlayerNumber playerNumber, Entity entity, Vec2f position, Vec2f velocity)
{
    createdEntities_.push_back({ entity, testedFrame_ });

    Body bulletBody;
    bulletBody.position = position;
    bulletBody.velocity = velocity;
    Box bulletBox;
    bulletBox.extends = Vec2f::one * bulletScale * 0.5f;

    currentBulletManager_.AddComponent(entity);
    currentBulletManager_.SetComponent(entity, { bulletPeriod, playerNumber });

    currentPhysicsManager_.AddBody(entity);
    currentPhysicsManager_.SetBody(entity, bulletBody);
    currentPhysicsManager_.AddBox(entity);
    currentPhysicsManager_.SetBox(entity, bulletBox);

    currentTransformManager_.AddComponent(entity);
    currentTransformManager_.SetPosition(entity, position);
    currentTransformManager_.SetScale(entity, Vec2f::one * bulletScale);
    currentTransformManager_.SetRotation(entity, degree_t(0.0f));
    currentTransformManager_.UpdateDirtyComponent(entity);
}

void RollbackManager::DestroyEntity(Entity entity)
{
    //we don't need to save a bullet that has been created in the time window
    if (std::find_if(createdEntities_.begin(), createdEntities_.end(), [entity](auto newEntity)
        {
            return newEntity.entity == entity;
        }) != createdEntities_.end())
    {
        entityManager_.DestroyEntity(entity);
        return;
    }
        entityManager_.AddComponentType(entity, EntityMask(ComponentType::DESTROYED));
}
}
