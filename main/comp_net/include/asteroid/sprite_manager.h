#pragma once

#include "engine/transform.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "graphics/graphics.h"
namespace neko::asteroid
{
class ClientGameManager;
struct Sprite
{

};

class SpriteManager : public ComponentManager<Sprite, neko::ComponentType::SPRITE2D>, public RenderCommandInterface,
public SystemInterface
{
public:
	explicit SpriteManager(ClientGameManager& gameManager, EntityManager& entityManager, neko::Transform2dManager& transformManager);

	void Render() override;
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
private:
	neko::Transform2dManager& transformManager_;
	ClientGameManager& gameManager_;
	gl::Texture shipTexture_;
	gl::Shader spriteShader_;
	gl::RenderQuad spriteQuad_{ Vec3f::zero, Vec2f::one };
};
} // namespace neko::asteroid
