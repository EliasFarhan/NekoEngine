//
// Created by efarhan on 12/4/20.
//

#include "engine/system.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "sdl_engine/sdl_engine.h"
#include "gl/gles3_window.h"
#include "gl/graphics.h"
#include "sdl_engine/sdl_camera.h"

#include "voxel/chunk.h"
#include "voxel/voxel_render_program.h"

namespace neko::voxel
{

class SingleChunkManager :
        public SystemInterface,
        public sdl::SdlEventSystemInterface
{
public:
    void Init() override
    {
        Job renderInit(
                [this]()
                {
                    renderProgram_.Init();
                });
        BasicEngine::GetInstance()->ScheduleJob(&renderInit, neko::JobThreadType::RENDER_THREAD);

        camera3D_.Init();

        for(size_t z = 0; z < chunkSize; z++)
        {
            for(size_t x = 0; x < chunkSize; x++)
            {
                for(size_t y = 0; y < chunkSize; y++)
                {

                    chunk_.contents[x][z][y] = {std::uint8_t (Cube::CubeFlag::IS_VISIBLE),
                                                     static_cast<CubeType>(neko::RandomRange(
                            static_cast<std::uint16_t>(CubeType::GRASS),
                            static_cast<std::uint16_t>(CubeType::DIRT))),
                            CubeId(x*chunkSize*chunkSize+z*chunkSize+y)
                    };
                }
            }
        }

        renderInit.Join();
        RendererLocator::get().RegisterSyncBuffersFunction(&renderProgram_);
    }

    void Update(neko::seconds dt) override
    {
        camera3D_.Update(dt);
        for(const auto& zArray : chunk_.contents)
        {
            for(const auto& yArray : zArray)
            {
                for(const auto& cube : yArray)
                {
                    renderProgram_.AddCube(cube, 0,0);
                }
            }
        }
        renderProgram_.SetCurrentCamera(camera3D_);
        RendererLocator::get().Render(&renderProgram_);
    }

    void Destroy() override
    {
        Job renderDestroy(
                [this]()
                {
                    renderProgram_.Destroy();
                });
        BasicEngine::GetInstance()->ScheduleJob(&renderDestroy, JobThreadType::RENDER_THREAD);
        renderDestroy.Join();
    }

    void OnEvent(const SDL_Event& event) override
    {
        camera3D_.OnEvent(event);
    }

private:
    VoxelRenderProgram renderProgram_;
    sdl::Camera3D camera3D_;
    Chunk chunk_;
};


}

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    neko::Filesystem filesystem;
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::sdl::SdlEngine engine(filesystem);
    engine.SetWindowAndRenderer(&window, &renderer);

    neko::voxel::SingleChunkManager chunkManager;
    engine.RegisterSystem(chunkManager);
    engine.RegisterOnEvent(chunkManager);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;

}