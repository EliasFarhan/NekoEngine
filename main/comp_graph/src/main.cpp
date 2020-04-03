//
// Created by efarhan on 25.01.20.
//

#ifdef NEKO_GLES3
#include <gl/graphics.h>
#include "gl/gles3_window.h"
#endif

#ifdef NEKO_VULKAN
#include <vk/graphics.h>
#include <vk/vulkan_window.h>
#endif

#include "comp_graph/comp_graph_engine.h"


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
    neko::CompGraphEngine engine;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return 0;
}