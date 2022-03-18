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
#include "fs/physfs.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "sdl_engine/sdl_engine.h"
#include "gl/sdl_window.h"
#include "gl/graphics.h"
#include "sdl_engine/sdl_camera.h"

#include "voxel/chunk.h"
#include "voxel/chunk_generator.h"
#include "voxel/voxel_manager.h"
#include "voxel/voxel_render_program.h"

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyC.h>
#endif

namespace neko::voxel
{

class VoxelGame :
        public SystemInterface,
        public sdl::SdlEventSystemInterface,
        public DrawImGuiInterface
{
public:
    void Init() override
    {
        const auto renderInit = std::make_shared<Task>(
                [this]()
                {
                    renderProgram_.Init();
                });
        BasicEngine::GetInstance()->ScheduleTask(renderInit, WorkerQueue::RENDER_QUEUE_NAME);
        voxelManager_.Init();

        const auto* engine = BasicEngine::GetInstance();
        const auto& config = engine->GetConfig();
        const auto windowSize = engine->GetWindowSize();
        camera3D_.position = Vec3f(0, voxelManager_.GetInitialHeight() + 2.0f, 0);
        camera3D_.WorldLookAt(camera3D_.position + Vec3f::forward() + Vec3f::right());
        camera3D_.farPlane = 5000.0f;
        camera3D_.cameraSpeed_ = 10.0f;
        camera3D_.cameraFast_ = 100.0f;
        camera3D_.fovY = Degree(45.0f / 2.0f);
        camera3D_.SetAspect(windowSize.x, windowSize.y);

        renderInit->Join();
        RendererLocator::get().RegisterSyncBuffersFunction(&renderProgram_);
    }

    void Update(neko::seconds dt) override
    {
#ifdef TRACY_ENABLE
        ZoneScoped;
#endif
        voxelManager_.Update(dt);
        camera3D_.Update(dt);
        renderProgram_.SetCurrentCamera(camera3D_);
#ifdef TRACY_ENABLE
        TracyCZoneCtx pushChunks{};
        TracyCZoneName(pushChunks, "Push Chunks To Renderer", 1);
#endif
        for(const auto* chunk : voxelManager_.GetChunks())
        {
            if(chunk->flag & Chunk::IS_VISIBLE)
                renderProgram_.AddChunk(*chunk);

        }
#ifdef TRACY_ENABLE
        TracyCZoneEnd(pushChunks);
#endif
        RendererLocator::get().Render(&renderProgram_);
    }

    void Destroy() override
    {
        const auto renderDestroy = std::make_shared<Task>(
                [this]()
                {
                    renderProgram_.Destroy();
                });
        BasicEngine::GetInstance()->ScheduleTask(renderDestroy, WorkerQueue::RENDER_QUEUE_NAME);
        renderDestroy->Join();
    }

    void OnEvent(const SDL_Event& event) override
    {
        camera3D_.OnEvent(event);
    }

    void DrawImGui() override
    {
        renderProgram_.DrawImGui();
    }
private:
    VoxelRenderProgram renderProgram_;
    sdl::Camera3D camera3D_;
    VoxelManager voxelManager_;
};


}

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv)
{

    neko::physfs::PhysFsFilesystem filesystem (argv[0]);
    filesystem.Init();
    
    neko::gl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;
    neko::sdl::SdlEngine engine(filesystem);
    auto& config = engine.MutableConfig();
    config.set_window_size_x(1280);
    config.set_window_size_y(720);
    config.set_vertical_sync(false);
    filesystem.AddMount(config.data_root() + "voxel.pkg", config.data_root(), 1);

    engine.SetWindowAndRenderer(&window, &renderer);

    neko::voxel::VoxelGame chunkManager;
    engine.RegisterSystem(chunkManager);
    engine.RegisterOnEvent(chunkManager);
    engine.RegisterOnDrawUi(chunkManager);

    engine.Init();
    engine.EngineLoop();
    filesystem.Destroy();
    return EXIT_SUCCESS;

}