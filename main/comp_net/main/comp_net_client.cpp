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


#include <sdl_engine/sdl_engine.h>
#include <gl/gles3_window.h>
#include <gl/graphics.h>
#include "asteroid_net/network_client.h"

namespace neko::asteroid
{

class ClientApp : public SystemInterface, public DrawImGuiInterface
{
public:
    void Init() override
    {
        Job initJob = Job([this]()
            {
                const auto& config = BasicEngine::GetInstance()->GetConfig();
                windowSize_ = config.windowSize;
                client_.SetWindowSize(windowSize_);
                client_.Init();
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            });
        BasicEngine::GetInstance()->ScheduleJob(&initJob, JobThreadType::RENDER_THREAD);
        initJob.Join();

    }

    void Update(seconds dt) override
    {
        const auto& config = BasicEngine::GetInstance()->GetConfig();
        if (windowSize_ != config.windowSize)
        {
            client_.SetWindowSize(config.windowSize);
            windowSize_ = config.windowSize;
        }
        //Checking if keys are down
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        net::PlayerInput clientInput1 = 0;
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LEFT] ? asteroid::PlayerInput::LEFT : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RIGHT] ? asteroid::PlayerInput::RIGHT : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_UP] ? asteroid::PlayerInput::UP : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_DOWN] ? asteroid::PlayerInput::DOWN : 0u);
        clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RCTRL] ? asteroid::PlayerInput::SHOOT : 0u);

        client_.SetPlayerInput(clientInput1);
        client_.Update(dt);

        RendererLocator::get().Render(&client_);
    }

    void Destroy() override
    {
        client_.Destroy();
    }

    void DrawImGui() override
    {
        client_.DrawImGui();
    }

private:
    Vec2u windowSize_;
    net::ClientNetworkManager client_;
};

class ClientEngine : public sdl::SdlEngine
{
public:
    explicit ClientEngine(FilesystemInterface& filesystem) : sdl::SdlEngine(filesystem)
    {
        RegisterOnDrawUi(app_);
        RegisterSystem(app_);
    }
private:
    ClientApp app_;
};
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    neko::Filesystem filesystem;
    neko::asteroid::ClientEngine engine(filesystem);

    neko::sdl::Gles3Window window;
    neko::gl::Gles3Renderer renderer;

    engine.SetWindowAndRenderer(&window, &renderer);

    engine.Init();
    engine.EngineLoop();
    return EXIT_SUCCESS;
}