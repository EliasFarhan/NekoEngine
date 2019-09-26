#include "Network/pred_sim.h"
namespace net
{
	ServerSimSystem::ServerSimSystem(PredSimEngine& engine) : engine_(engine)
	{
		neko::BasicEngine();
	}

	void ServerSimSystem::Init()
	{
	}

	void ServerSimSystem::Update(float dt)
	{
	}

	void ServerSimSystem::Destroy()
	{
	}

	ClientSimSystem::ClientSimSystem(PredSimEngine& engine) : engine_(engine)
	{
		neko::BasicEngine();
	}

	void ClientSimSystem::Init()
	{
	}

	void ClientSimSystem::Update(float dt)
	{
	}

	void ClientSimSystem::Destroy()
	{
	}

	PredSimEngine::PredSimEngine() : client_(*this), server_(*this)
	{
	}

	void PredSimEngine::Init()
	{
		BasicEngine::Init();
	}

	void PredSimEngine::Update(float dt)
	{
		BasicEngine::Update(dt);
	}

	void PredSimEngine::Destroy()
	{
		BasicEngine::Destroy();
	}
}