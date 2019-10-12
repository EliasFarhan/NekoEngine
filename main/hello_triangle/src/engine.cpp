#include "hello_triangle/engine.h"

namespace neko
{
HelloTriangleEngine::HelloTriangleEngine(Configuration* config) : SdlGlEngine(config)
{
	initDelegate_.RegisterCallback([&helloTriangleCommand = helloTriangleCommand_](void)
		{
			helloTriangleCommand.Init();
		});
	updateDelegate_.RegisterCallback([&helloTriangleCommand = helloTriangleCommand_](float dt)
		{
			helloTriangleCommand.Update(dt);
		});
	destroyDelegate_.RegisterCallback([&helloTriangleCommand = helloTriangleCommand_](void)
		{
			helloTriangleCommand.Destroy();
		});
	drawDelegate_.RegisterCallback([this]()
		{
			graphicsManager_.Draw(&helloTriangleCommand_);
		});
}
}
