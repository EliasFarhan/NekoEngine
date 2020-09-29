#include "asteroid/sprite_manager.h"
#include "asteroid/game_manager.h"
#include "engine/engine.h"

namespace neko::asteroid
{
SpriteManager::SpriteManager(ClientGameManager& gameManager, EntityManager& entityManager, neko::Transform2dManager& transformManager) :
	ComponentManager<Sprite, neko::ComponentType::SPRITE2D>(entityManager),
	transformManager_(transformManager),
	gameManager_(gameManager)
{
}


void SpriteManager::Render()
{
	spriteShader_.Bind();
	const auto& camera = gameManager_.GetCamera();
	spriteShader_.SetMat4("view", camera.GenerateViewMatrix());
	spriteShader_.SetMat4("projection", camera.GenerateProjectionMatrix());
	for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
	{
		if(entityManager_.HasComponent(entity, static_cast<EntityMask>(neko::ComponentType::SPRITE2D)))
		{
			const auto position = Vec3f(transformManager_.GetPosition(entity));
			const auto rotation = transformManager_.GetRotation(entity);
			auto model = Mat4f::Identity;
			model = Transform3d::Rotate(model, rotation, Vec3f::forward);
			model = Transform3d::Translate(model, position);
			spriteShader_.SetMat4("model", model);
			spriteShader_.SetTexture("texture", shipTextureName_);
			spriteQuad_.Draw();
		}
	}
}

void SpriteManager::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	auto& textureManager = TextureManagerLocator::get();
	shipTextureId_ = textureManager.LoadTexture(
		config.dataRootPath + "sprites/asteroid/ship.png", 
		static_cast<Texture::TextureFlags>(Texture::TextureFlags::FLIP_Y | Texture::TextureFlags::CLAMP_WRAP));
	spriteQuad_.Init();
	spriteShader_.LoadFromFile(
		config.dataRootPath + "shaders/comp_net/sprite.vert",
		config.dataRootPath + "shaders/comp_net/sprite.frag"
	);
}

void SpriteManager::Update(seconds dt)
{
	if (shipTextureName_ == INVALID_TEXTURE_NAME)
	{
		const auto& textureManager = TextureManagerLocator::get();
		shipTextureName_ = textureManager.GetTexture(shipTextureId_).name;
	}
}

void SpriteManager::Destroy()
{
	spriteQuad_.Destroy();
	spriteShader_.Destroy();
}
}
