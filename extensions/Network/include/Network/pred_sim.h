#pragma once


#include <engine/engine.h>
#include <engine/transform.h>
#include <graphics/shape.h>
namespace net
{
	struct ActorData
	{
		neko::Index tickIndex = neko::INVALID_INDEX;
		neko::Vec2f position{};
		neko::Vec2f velocity{};
	};
	const size_t serverActorDataBufferSize = 3;
	const neko::EntityMask velocityMask = 1u << 15u;
	class VelocityManager : public neko::ComponentManager<neko::Vec2f, velocityMask>
	{
		
	};

	class PredSimEngine;
	class ServerSimSystem : public neko::System
	{
	public:
		explicit ServerSimSystem(PredSimEngine& engine);
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		PredSimEngine& engine_;
		std::vector<std::array<ActorData, serverActorDataBufferSize>> serverActorsDataBuffer_;
		neko::Index tick_ = 0;
	};

	class ClientSimSystem : public  neko::System
	{
	public:
		explicit ClientSimSystem(PredSimEngine& engine);
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		std::vector<neko::Entity> entities_;
		PredSimEngine& engine_;
		neko::Index tick_ = 0;
	};

	enum class ServerPredictionType
	{
		None,
		Interpolation,
		Extrapolation
	};
	class PredSimEngine : public neko::BasicEngine
	{
	public:
		PredSimEngine();
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		friend class ServerSimSystem;
		friend class ClientSimSystem;
		ServerSimSystem server_;
		ClientSimSystem client_;

		neko::EntityManager entityManager_;
		neko::GraphicsManager graphicsManager_;
		neko::Transform2dManager transformManager_;
		neko::ConvexShapeManager shapeManager_;
		VelocityManager velocitiesManager_;
	};

}