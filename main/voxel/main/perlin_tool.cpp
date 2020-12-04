#include <array>



#include <gl/gles3_window.h>
#include <sdl_engine/sdl_engine.h>
#include <gl/graphics.h>

#include <tools/perlin_viewer.h>

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    neko::Filesystem filesystem;
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::sdl::SdlEngine engine(filesystem);
    engine.SetWindowAndRenderer(&window, &renderer);

    neko::voxel::PerlinViewer perlinViewer;
    engine.RegisterSystem(perlinViewer);
    engine.RegisterOnDrawUi(perlinViewer);
    engine.Init();

    engine.EngineLoop();
    return EXIT_SUCCESS;
}