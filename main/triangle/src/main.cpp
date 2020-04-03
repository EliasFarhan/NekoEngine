#include <vk/vulkan_window.h>
#include <vk/graphics.h>

#include <sdl_engine/sdl_engine.h>


int main(int argc, char** argv)
{
#ifdef NEKO_GLES3
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
#endif
#ifdef NEKO_VULKAN
    neko::sdl::VulkanWindow window;
    neko::vk::VulkanRenderer renderer;
#endif
    neko::sdl::SdlEngine engine;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return 0;
}
