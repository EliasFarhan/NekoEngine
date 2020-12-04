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

namespace neko::voxel
{

class SingleChunkManager :
        public SystemInterface,
        public RenderCommandInterface,
        public sdl::SdlEventSystemInterface
{
public:
    void Init() override
    {
        Job renderInit(
                [this]()
                {
                    const auto& config = BasicEngine::GetInstance()->GetConfig();
                    const auto& filesystem = BasicEngine::GetInstance()->GetFilesystem();
                    chunkShader_.LoadFromFile(config.dataRootPath+"shaders/voxel/chunk.vert",
                                              config.dataRootPath+"shaders/voxel/chunk.frag");
                    chunkCube_.Init();
                    chunkTexture_ = gl::CreateTextureFromKTX(config.dataRootPath+"sprites/tilesheet.png.ktx", filesystem);
                    glBindTexture(GL_TEXTURE_2D, chunkTexture_);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                });
        BasicEngine::GetInstance()->ScheduleJob(&renderInit, neko::JobThreadType::RENDER_THREAD);

        camera3D_.Init();

        for(size_t z = 0; z < chunkSize; z++)
        {
            for(size_t x = 0; x < chunkSize; x++)
            {
                for(size_t y = 0; y < chunkSize; y++)
                {
                    chunk_.chunkContents[z][x][y] = {std::uint8_t (Cube::CubeFlag::IS_VISIBLE),
                                                     static_cast<Cube::CubeType>(neko::RandomRange(
                            static_cast<std::uint8_t>(1u),
                            static_cast<std::uint8_t>(Cube::CubeType::LENGTH)))
                    };
                }
            }
        }

        renderInit.Join();
    }

    void Update(neko::seconds dt) override
    {
        camera3D_.Update(dt);
        RendererLocator::get().Render(this);
    }

    void Destroy() override
    {
        Job renderDestroy(
                [this]()
                {
                    chunkShader_.Destroy();
                    chunkCube_.Destroy();
                });
        BasicEngine::GetInstance()->ScheduleJob(&renderDestroy, JobThreadType::RENDER_THREAD);
        renderDestroy.Join();
    }

    void Render() override
    {

    }

    void OnEvent(const SDL_Event& event) override
    {
        camera3D_.OnEvent(event);
    }

private:
    sdl::Camera3D camera3D_;
    Chunk chunk_;
    gl::Shader chunkShader_;
    gl::RenderCuboid chunkCube_{neko::Vec3f::zero, neko::Vec3f::one};
    neko::TextureName chunkTexture_ = neko::INVALID_TEXTURE_NAME;
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

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;

}