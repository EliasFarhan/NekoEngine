/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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
        camera3D_.position = Vec3f(0, 100, 0);
        camera3D_.WorldLookAt(camera3D_.position + Vec3f::forward + Vec3f::right);
        camera3D_.farPlane = 5000.0f;
        camera3D_.cameraSpeed_ = 10.0f;
        camera3D_.cameraFast_ = 100.0f;
        camera3D_.fovY = degree_t(45.0f / 2.0f);

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