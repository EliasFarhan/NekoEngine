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
#include "voxel/chunk_generator.h"
#include "voxel/voxel_render_program.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::voxel
{

class SingleRegionManager :
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
        camera3D_.position = Vec3f(0, 300, 0);
        camera3D_.WorldLookAt(camera3D_.position + Vec3f::forward + Vec3f::right);
        camera3D_.farPlane = 1000.0f;

        region_ = chunkGenerator_.GenerateRegion(0);

        renderInit.Join();
        RendererLocator::get().RegisterSyncBuffersFunction(&renderProgram_);
    }

    void Update(neko::seconds dt) override
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Update Region Manager");
#endif
        camera3D_.Update(dt);
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Push Chunks To Renderer");
#endif
        for(const auto& chunk : region_.GetChunks())
        {
            if(chunk.flag & Chunk::IS_VISIBLE)
                renderProgram_.AddChunk(chunk);

        }
#ifdef EASY_PROFILE_USE
        EASY_END_BLOCK;
#endif
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
    ChunkGenerator chunkGenerator_;
    VoxelRenderProgram renderProgram_;
    sdl::Camera3D camera3D_;
    Region region_;
};


}

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{
    neko::Filesystem filesystem;
    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::sdl::SdlEngine engine(filesystem);
    engine.SetWindowAndRenderer(&window, &renderer);

    neko::voxel::SingleRegionManager chunkManager;
    engine.RegisterSystem(chunkManager);
    engine.RegisterOnEvent(chunkManager);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;

}