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

#include <gl/sprite.h>
#include "asteroid/physics_manager.h"
#include "engine/component.h"
#include "engine/transform.h"
#include "graphics/camera.h"
#include "gl/texture.h"
#include "comp_net/type.h"
#include "asteroid/rollback_manager.h"
#include "asteroid/game.h"

namespace neko::asteroid
{
class GameManager : public SystemInterface
{
public:
	GameManager();
	
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	virtual void SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation);
	virtual Entity SpawnBullet(net::PlayerNumber, Vec2f position, Vec2f velocity);
	[[nodiscard]] Entity GetEntityFromPlayerNumber(net::PlayerNumber playerNumber) const;
	[[nodiscard]] net::Frame GetCurrentFrame() const { return currentFrame_; }
	[[nodiscard]] net::Frame GetLastValidateFrame() const { return rollbackManager_.GetLastValidateFrame(); }
	[[nodiscard]] const Transform2dManager& GetTransformManager() const { return transformManager_; }
    [[nodiscard]] const RollbackManager& GetRollbackManager() const { return rollbackManager_; }
	virtual void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame);
	/*
	 * \brief Called by the server to validate a frame
	 */
    void Validate(net::Frame newValidateFrame);
    static constexpr float PixelPerUnit = 100.0f;
	static constexpr float FixedPeriod = 0.02f; //50fps
protected:
	EntityManager entityManager_;
	Transform2dManager transformManager_;
	RollbackManager rollbackManager_;
	std::array<Entity, maxPlayerNmb> entityMap_{};
	net::Frame currentFrame_ = 0;
};

class ClientGameManager : public GameManager, public RenderCommandInterface, public DrawImGuiInterface
{
public:
    enum State : std::uint32_t
    {
        STARTED = 1u << 0u,
        FINISHED = 1u << 1u,
    };
	explicit ClientGameManager(PacketSenderInterface& packetSenderInterface);
	void StartGame(unsigned long long int startingTime);
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void SetWindowSize(Vec2u windowsSize);
	[[nodiscard]] Vec2u GetWindowSize() const { return windowSize_; }
	void Render() override;
	void SetClientPlayer(net::PlayerNumber clientPlayer) { clientPlayer_ = clientPlayer; }
	[[nodiscard]] const Camera2D& GetCamera() const { return camera_; }
	void SpawnPlayer(net::PlayerNumber playerNumber, Vec2f position, degree_t rotation) override;
	Entity SpawnBullet(net::PlayerNumber playerNumber, Vec2f position, Vec2f velocity) override;
	void FixedUpdate();
	void SetPlayerInput(net::PlayerNumber playerNumber, net::PlayerInput playerInput, std::uint32_t inputFrame) override;
    void DrawImGui() override;
    void ConfirmValidateFrame(net::Frame newValidateFrame, const std::array<PhysicsState, maxPlayerNmb>& physicsStates);
	[[nodiscard]] net::PlayerNumber GetPlayerNumber() const { return clientPlayer_; }

    [[nodiscard]] std::uint32_t GetState() const {return state_;}
protected:
    PacketSenderInterface& packetSenderInterface_;
	Vec2u windowSize_;
	Camera2D camera_;
	net::PlayerNumber clientPlayer_ = net::INVALID_PLAYER;
	gl::TextureManager textureManager_;
	gl::SpriteManager spriteManager_;
	float fixedTimer_ = 0.0f;
    unsigned long long startingTime_ = 0;
	std::uint32_t state_ = 0;

    TextureId shipTextureId_ = INVALID_TEXTURE_ID;
    TextureId bulletTextureId_ = INVALID_TEXTURE_ID;
    std::mutex renderMutex_;
};

}
