/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */

#include <argh.h>
#include <City/city_engine.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    const argh::parser cmdl(argv);
    neko::Configuration config{};

	bool vsync = true;
	if (cmdl({"--vsync"}) >> vsync)
	{
		config.vSync = vsync;
	}
	int framerateLimit = 0;
	if (cmdl({ "--fpslimit" }) >> framerateLimit)
	{
		config.framerateLimit = framerateLimit;
	}
	neko::CityBuilderEngine engine(&config);
	if(cmdl[{"--infinite-money"}])
	{
		auto cheatData = engine.GetCheatData();
		cheatData |= neko::CheatModeData::INFINITE_MONEY;
		engine.SetCheatData(cheatData);
	}

	if(cmdl[{"--quick-spawn"}])
	{
		auto cheatData = engine.GetCheatData();
		cheatData |= neko::CheatModeData::QUICK_SPAWN;
		engine.SetCheatData(cheatData);
	}
	if(cmdl[{"--init-spawn"}])
	{
		auto cheatData = engine.GetCheatData();
		cheatData |= neko::CheatModeData::CITY_SPAWN_INIT;
		engine.SetCheatData(cheatData);
	}

	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}
