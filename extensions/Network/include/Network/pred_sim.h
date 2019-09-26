#pragma once


#include <engine/engine.h>
#include <engine/transform.h>
#include <graphics/shape.h>
namespace net
{
	struct Actor
	{
		neko::Vec2f position{};
		neko::Vec2f velocity{};
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
		std::vector<Actor> serverActors_;
	};

	class ClientSimSystem : public  neko::System
	{
	public:
		explicit ClientSimSystem(PredSimEngine& engine);
		void Init() override;
		void Update(float dt) override;
		void Destroy() override;
	private:
		PredSimEngine& engine_;
		std::vector<Actor> clientActors_;
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
	};

}