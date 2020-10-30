#include "model_viewer.h"

#include <editor_engine.h>
#ifdef NEKO_GLES3
#include "gl/graphics.h"
#include "gl/gles3_window.h"
#endif
#include <custom_allocator_viewer.h>

int main(int argc, char** argv)
{
#ifdef NEKO_GLES3
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
#endif
    neko::editor::EditorEngine engine;

    neko::editor::CustomAllocatorViewer customAllocatorViewer;
    neko::editor::CustomAllocatorTester customAllocatorTester(customAllocatorViewer);

    neko::editor::ModelViewer modelViewer;


    /*engine.RegisterOnDrawUi(customAllocatorViewer);
    engine.RegisterOnDrawUi(customAllocatorTester);*/
    engine.RegisterSystem(modelViewer);
    engine.RegisterOnDrawUi(modelViewer);

    engine.SetWindowAndRenderer(&window, &renderer);
    engine.Init();
    engine.EngineLoop();
    return 0;
}
