#include "Network/pred_sim.h"
#include <random>
#include <cmath>
#ifdef WIN32
#include <corecrt_math_defines.h>
#endif
namespace net
{
	const size_t actorNmb = 10;
	const float speed = 1.0f;
	ServerSimSystem::ServerSimSystem(PredSimEngine& engine) : engine_(engine)
	{
	}

	void ServerSimSystem::Init()
	{
		
	}

	void ServerSimSystem::Update([[maybe_unused]]float dt)
	{
		tick_++;
	}

	void ServerSimSystem::Destroy()
	{
	}

	ClientSimSystem::ClientSimSystem(PredSimEngine& engine) : engine_(engine)
	{
		
	}

	void ClientSimSystem::Init()
	{

		auto& entityManager = engine_.entityManager_;
		auto& transformManager = engine_.transformManager_;
		auto& positionManager = transformManager.GetPositionManager();
		auto& rotationManager = transformManager.GetRotationManager();
		auto& velocityManager = engine_.velocitiesManager_;
		auto& shapeManager = engine_.shapeManager_;
		
		const auto screenSize = engine_.config.gameWindowSize;
		std::random_device rd; // obtain a random number from hardware
		std::mt19937 eng(rd()); // seed the generator
		const std::uniform_real_distribution<float> distrX(0.0f, float(screenSize.x)); // define the range
		const std::uniform_real_distribution<float> distrY(0.0f, float(screenSize.y)); // define the range
		const std::uniform_real_distribution<float> distrAngle(0.0f, 2.0f*float(M_PI)); // define the range

		entities_.reserve(actorNmb);
		for(size_t i = 0; i < actorNmb;i++)
		{
			const auto entity = entityManager.CreateEntity();
			entities_.push_back(entity);

			positionManager.AddComponent(entityManager, entity);
			const auto position = neko::pixel2unit(neko::Vec2f(distrX(eng), distrY(eng)));
			positionManager.SetComponent(entity, position);

			rotationManager.AddComponent(entityManager, entity);
			const float angle = distrAngle(eng);
			rotationManager.SetComponent(entity, angle/float(M_PI)*180.0f);

			velocityManager.AddComponent(entityManager, entity);
			velocityManager.SetComponent(entity, neko::Vec2f(sin(angle), -cos(angle)));

			neko::ShapeDef clientShape;
			clientShape.fillColor = sf::Color::Red;
			neko::Vec2f points[] = {
				sf::Vector2f(0,-0.5f),
				sf::Vector2f(0.5f,0.5f),
				sf::Vector2f(-0.5f, 0.5f)
			};
			shapeManager.AddPolygon(entity, position, points, 3, clientShape);

		}
	}

	void ClientSimSystem::Update(float dt)
	{
		auto& transformManager = engine_.transformManager_;
		auto& positionManager = transformManager.GetPositionManager();
		auto& rotationManager = transformManager.GetRotationManager();
		auto& velocityManager = engine_.velocitiesManager_;
		const auto screenSize = engine_.config.gameWindowSize;

		for(auto& entity : entities_)
		{
			const auto pos = positionManager.GetConstComponent(entity);
			auto vel = velocityManager.GetConstComponent(entity);
			const auto newPos = pos + vel * dt * speed;
			positionManager.SetComponent(entity, newPos);

			if((newPos.x < 0.0f and vel.x < 0.0f) or (newPos.x > neko::pixel2unit(float(screenSize.x)) and vel.x > 0.0f))
			{
				vel.x *= -1.0f;
			}
			if ((newPos.y < 0.0f and vel.y < 0.0f) or (newPos.y > neko::pixel2unit(float(screenSize.y)) and vel.y > 0.0f))
			{
				vel.y *= -1.0f;
			}

			const float newAngle = neko::Vec2f::AngleBetween(vel, neko::Vec2f(0.0f, -1.0f));
			rotationManager.SetComponent(entity, newAngle);
			velocityManager.SetComponent(entity, vel);
		}
		tick_++;
	}

	void ClientSimSystem::Destroy()
	{
	}

	PredSimEngine::PredSimEngine() : server_(*this), client_(*this)
	{
		
	}

	void PredSimEngine::Init()
	{
		BasicEngine::Init();

		client_.Init();
		server_.Init();
	}

	void PredSimEngine::Update(float dt)
	{
		BasicEngine::Update(dt);
		client_.Update(dt);
		server_.Update(dt);

		shapeManager_.CopyTransformPosition(transformManager_.GetPositionManager(), 0, actorNmb * 2);
		shapeManager_.CopyTransformRotation(transformManager_.GetRotationManager(), 0, actorNmb * 2);
		shapeManager_.PushCommands(&graphicsManager_, 0, actorNmb * 2);

		graphicsManager_.Render(*renderWindow);
	}

	void PredSimEngine::Destroy()
	{
		BasicEngine::Destroy();

		client_.Destroy();
		server_.Destroy();
	}
}
