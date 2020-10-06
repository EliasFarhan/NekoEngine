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

#pragma once
#include "engine/component.h"
#include "asteroid/game.h"
#include "asteroid/physics_manager.h"

namespace neko::asteroid
{

struct PlayerCharacter
{
    float shootingTime = 0.0f;
    net::PlayerInput input = 0;
    net::PlayerNumber playerNumber = net::INVALID_PLAYER;

};
class GameManager;
class PlayerCharacterManager : public ComponentManager<PlayerCharacter, EntityMask(ComponentType::PLAYER_CHARACTER)>
{
public:
    explicit PlayerCharacterManager(EntityManager& entityManager, PhysicsManager& physicsManager, GameManager& gameManager);
    PlayerCharacterManager& operator= ( const PlayerCharacterManager & );
    void FixedUpdate(seconds dt);

private:
    std::reference_wrapper<PhysicsManager> physicsManager_;
    std::reference_wrapper<GameManager> gameManager_;
};

}