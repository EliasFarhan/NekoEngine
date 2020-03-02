
#ifdef WIN32
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <random>
#include <gtest/gtest.h>
#include <sdl_engine/sdl_engine.h>

TEST(Engine, TestInput)
{
	auto engine = neko::sdl::SdlEngine(nullptr);
	
	neko::InputManager inputManager;
	SDL_Init(SDL_INIT_EVENTS);
	while (true)
	{
		inputManager.OnPreUserInputs();
		if (inputManager.IsKeyDown(neko::KeyCode::W))
		{
			std::cout << "Touche W appuyé" << '\n';
		}
	}
	
}