#pragma once

#include <thread>
#include <engine/system.h>
#include <graphics/graphics.h>
#include <ctpl_stl.h>
#include <SFML/Graphics/RenderWindow.hpp>

namespace neko
{

class Engine : public sf::NonCopyable, public System
{
public:
    Engine();
    virtual ~Engine();
    void Init() override;
	void Update() override;
	void Destroy() override;
    
	virtual void EngineLoop();

	virtual void OnEvent(sf::Event& event);

    std::atomic<bool> isRunning = false;
    sf::RenderTarget* renderTarget = nullptr;
	sf::Vector2u renderTargetSize;

	std::mutex renderMutex;
protected:

    std::thread renderThread;
    GraphicsManager* graphicsManager;
    std::condition_variable condSyncRender;

};


class MainEngine : public Engine
{
public:
	MainEngine();
	virtual ~MainEngine();

	void Init() override;
	void Update() override;
	void Destroy() override;


	void OnEvent(sf::Event& event) override;

	sf::RenderWindow* renderWindow = nullptr;
	
	static MainEngine *GetInstance();
private:
	ctpl::thread_pool workingThreadPool;

	static MainEngine *instance;
};

}
