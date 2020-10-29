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
#include "game.h"
#include "engine/transform.h"
#include "asteroid/packet_type.h"
#include "asteroid/physics_manager.h"
#include "player_character.h"
#include "bullet_manager.h"

namespace neko::asteroid
{
class GameManager;

struct CreatedEntity
{
    Entity entity = INVALID_ENTITY;
    net::Frame createdFrame = 0;
};

struct DestroyedBullet
{
    Bullet bullet;
    Body body;
    net::Frame destroyedFrame = 0;
};

class RollbackManager : public OnCollisionInterface
{
public:
    explicit RollbackManager(GameManager& gameManager, EntityManager& entityManager);
    /**
     * \brief Simulate all players with new inputs, method call only by the clients
     */
    void SimulateToCurrentFrame();
    void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, net::Frame inputFrame);
    void StartNewFrame(net::Frame newFrame);
    /**
     * \brief Validate all the frame from lastValidateFrame_ to newValidateFrame
     */
    void ValidateFrame(net::Frame newValidateFrame);
    /**
     * \brief Confirm Frame and Check with Physics State checksum, called by the clients when receiving Confirm Frame packet
     */
    void ConfirmFrame(net::Frame newValidatedFrame, const std::array<PhysicsState, maxPlayerNmb>& serverPhysicsState);
    [[nodiscard]] PhysicsState GetValidatePhysicsState(net::PlayerNumber playerNumber) const;
    [[nodiscard]] net::Frame GetLastValidateFrame() const { return lastValidateFrame_; }
    [[nodiscard]] net::Frame GetLastReceivedFrame(net::PlayerNumber playerNumber) const { return lastReceivedFrame_[playerNumber]; }
    [[nodiscard]] net::Frame GetCurrentFrame() const { return currentFrame_; }
    [[nodiscard]] const Transform2dManager& GetTransformManager() const { return currentTransformManager_; }
    [[nodiscard]] const PlayerCharacterManager& GetPlayerCharacterManager() const { return currentPlayerManager_; }
    void SpawnPlayer(net::PlayerNumber playerNumber, Entity entity, Vec2f position, degree_t rotation);
    void SpawnBullet(net::PlayerNumber playerNumber, Entity entity, Vec2f position, Vec2f velocity);
    /**
     * \brief This function does not destroy the entity definitely, but puts the DESTROY flag
     */
    void DestroyEntity(Entity entity);

    void OnCollision(Entity entity1, Entity entity2) override;
private:
    net::PlayerInput GetInputAtFrame(net::PlayerNumber playerNumber, net::Frame frame);
    GameManager& gameManager_;
    EntityManager& entityManager_;
    /**
     * \brief Used for rendering
     */
    Transform2dManager currentTransformManager_;
    PhysicsManager currentPhysicsManager_;
    PlayerCharacterManager currentPlayerManager_;
    BulletManager currentBulletManager_;
    PhysicsManager lastValidatePhysicsManager_;
    PlayerCharacterManager lastValidatePlayerManager_;
    BulletManager lastValidateBulletManager_;


    net::Frame lastValidateFrame_ = 0;
    net::Frame currentFrame_ = 0;
    net::Frame testedFrame_ = 0;

    static const size_t windowBufferSize = 5 * 50;
    std::array<std::uint32_t, maxPlayerNmb> lastReceivedFrame_{};
    std::array<std::array<net::PlayerInput, windowBufferSize>, maxPlayerNmb> inputs_{};
    std::vector<CreatedEntity> createdEntities_;
public:
    [[nodiscard]] const std::array<net::PlayerInput, windowBufferSize>& GetInputs(net::PlayerNumber playerNumber) const
    {
        return inputs_[playerNumber];
    }

};
}
