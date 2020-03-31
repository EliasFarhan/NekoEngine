#include <editor_engine.h>
#include "gl/graphics.h"
#include "gl/gles3_window.h"

#include <custom_allocator_viewer.h>

int main(int argc, char** argv)
{
	neko::sdl::Gles3Window window;
	neko::gl::Gles3Renderer renderer;
	neko::editor::EditorEngine engine;

	neko::editor::CustomAllocatorViewer customAllocatorViewer;
	neko::editor::CustomAllocatorTester customAllocatorTester(customAllocatorViewer);

	engine.RegisterOnDrawUi(customAllocatorViewer);
	engine.RegisterOnDrawUi(customAllocatorTester);

	engine.SetWindowAndRenderer(&window, &renderer);
	engine.Init();
	engine.EngineLoop();
	return 0;
}
