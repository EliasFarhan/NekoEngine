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

#include "asteroid/player_character.h"

namespace neko::asteroid
{

PlayerCharacterManager::PlayerCharacterManager(EntityManager& entityManager, PhysicsManager& physicsManager) :
    physicsManager_(physicsManager),
    ComponentManager<PlayerCharacter, neko::ComponentType(ComponentType::PLAYER_CHARACTER)>(entityManager)
{

}

void PlayerCharacterManager::FixedUpdate(seconds dt)
{
    for(Entity playerEntity = 0; playerEntity < entityManager_.GetEntitiesSize(); playerEntity++)
    {
        if(!entityManager_.HasComponent(playerEntity, EntityMask(ComponentType::PLAYER_CHARACTER)))
            continue;
        auto playerBody = physicsManager_.GetBody(playerEntity);
        auto input = GetComponent(playerEntity).input;

        const bool right = input & PlayerInput::RIGHT;
        const bool left = input & PlayerInput::LEFT;
        const bool up = input & PlayerInput::UP;
        const bool down = input & PlayerInput::DOWN;

        auto angularVelocity = ((left ? 1.0f : 0.0f) + (right ? -1.0f : 0.0f)) * playerAngularSpeed;

        playerBody.angularVelocity = angularVelocity;

        auto dir = Vec2f::up;
        dir = dir.Rotate(-playerBody.rotation);

        auto acceleration = ((down ? -1.0f : 0.0f) + (up ? 1.0f : 0.0f)) * dir;

        auto& velocity = playerBody.velocity;
        velocity += acceleration * GameManager::FixedPeriod;

        physicsManager_.SetBody(playerEntity, playerBody);

    }
}

PlayerCharacterManager::PlayerCharacterManager(const PlayerCharacterManager& playerCharacterManager) :
    ComponentManager<PlayerCharacter, neko::ComponentType(ComponentType::PLAYER_CHARACTER)>(playerCharacterManager),
    physicsManager_(playerCharacterManager.physicsManager_)
{

}

PlayerCharacterManager& PlayerCharacterManager::operator=(const PlayerCharacterManager& playerCharacterManager)
{
    entityManager_ = playerCharacterManager.entityManager_;
    physicsManager_ = playerCharacterManager.physicsManager_;
    components_ = playerCharacterManager.components_;
    return *this;
}
}