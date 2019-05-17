#pragma once

#include <thread>
#include <engine/system.h>
#include <ctpl_stl.h>

namespace neko
{

class Engine : public System
{
	Engine();
	void Init() override;
	void Update() override;

	static Engine* GetInstance();
private:
	ctpl::thread_pool workingThreadPool;
	std::thread renderThread;
	static Engine* instance;
};
}
