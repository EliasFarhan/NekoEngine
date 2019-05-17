#include <engine/engine.h>
namespace neko
{

Engine* Engine::instance = nullptr;

Engine::Engine()
{
	instance = this;
}

void Engine::Init()
{
	workingThreadPool.resize(std::thread::hardware_concurrency() - 2);//removing main and render thread

}

void Engine::Update()
{
	
}

Engine* Engine::GetInstance()
{
	return instance;
}
}
