#pragma once

#include <thread>
#include <engine/system.h>
#include <graphics/graphics.h>
#include <ctpl_stl.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include "Box2D/Common/b2Math.h"


struct Remotery;

namespace neko
{

struct Configuration
{
	sf::Vector2u screenSize = sf::Vector2u(1280, 720);
	b2Vec2 gravity = b2Vec2(0.0f, 9.81f);
	float physicsTimeStep = 1.0f/50.0f;
	int velocityIterations = 8;
	int positionIterations = 3;
};

class MainEngine : public sf::NonCopyable, public System
{
public:
	explicit MainEngine(Configuration* config = nullptr);
	virtual ~MainEngine();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Destroy() override;

	void EngineLoop();

	virtual void OnEvent(sf::Event& event);
	Configuration config;
	sf::RenderWindow* renderWindow = nullptr;
	static MainEngine *GetInstance();
	std::atomic<bool> isRunning = false;
	std::atomic<bool> isReady = false;
	sf::Vector2u renderTargetSize;

	//used to sync with the render thread
	std::condition_variable condSyncRender;
	std::mutex renderMutex;

	unsigned frameIndex = 0;
protected:

	Remotery* rmt = nullptr;
	ctpl::thread_pool workingThreadPool;
	std::thread renderThread;
	GraphicsManager* graphicsManager{};
	static MainEngine *instance;
};

}
