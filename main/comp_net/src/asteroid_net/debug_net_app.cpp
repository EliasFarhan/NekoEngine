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

#include "asteroid_net/debug_net_app.h"
#include "imgui.h"

namespace neko::net
{
NetworkDebugApp::NetworkDebugApp()
{
}

void NetworkDebugApp::Init()
{
    Job initJob = Job([this]()
        {
            const auto& config = BasicEngine::GetInstance()->GetConfig();
            windowSize_ = config.windowSize;
            for (auto& framebuffer : clientsFramebuffer_)
            {
                framebuffer.SetSize(windowSize_ / Vec2u(2, 1));
                framebuffer.Create();
            }
            for (auto& client : clients_)
            {
                client.SetWindowSize(windowSize_ / Vec2u(2, 1));
            }
            quad_.Init();
            //client shader init
            clientShader_.LoadFromFile(config.dataRootPath + "shaders/comp_net/client.vert",
                config.dataRootPath + "shaders/comp_net/client.frag");
            for (auto& client : clients_)
            {
                client.Init();
            }
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        });
    BasicEngine::GetInstance()->ScheduleJob(&initJob, JobThreadType::RENDER_THREAD);
    initJob.Join();
}

void NetworkDebugApp::Update(seconds dt)
{
    //Checking if keys are down
    const Uint8* keys = SDL_GetKeyboardState(nullptr);
    net::PlayerInput clientInput1 = 0;
    clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LEFT] ? asteroid::PlayerInput::LEFT : 0u);
    clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RIGHT] ? asteroid::PlayerInput::RIGHT : 0u);
    clientInput1 = clientInput1 | (keys[SDL_SCANCODE_UP] ? asteroid::PlayerInput::UP : 0u);
    clientInput1 = clientInput1 | (keys[SDL_SCANCODE_DOWN] ? asteroid::PlayerInput::DOWN : 0u);
    clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RCTRL] ? asteroid::PlayerInput::SHOOT : 0u);

    clients_[0].SetPlayerInput(clientInput1);

    net::PlayerInput clientInput2 = 0;
    clientInput2 = clientInput2 | (keys[SDL_SCANCODE_A] ? asteroid::PlayerInput::LEFT : 0u);
    clientInput2 = clientInput2 | (keys[SDL_SCANCODE_D] ? asteroid::PlayerInput::RIGHT : 0u);
    clientInput2 = clientInput2 | (keys[SDL_SCANCODE_W] ? asteroid::PlayerInput::UP : 0u);
    clientInput2 = clientInput2 | (keys[SDL_SCANCODE_S] ? asteroid::PlayerInput::DOWN : 0u);
    clientInput2 = clientInput2 | (keys[SDL_SCANCODE_SPACE] ? asteroid::PlayerInput::SHOOT : 0u);

    clients_[1].SetPlayerInput(clientInput2);
    for (auto& client : clients_)
    {
        client.Update(dt);
    }

    RendererLocator::get().Render(this);
}

void NetworkDebugApp::Destroy()
{
    for (auto& client : clients_)
    {
        client.Destroy();
    }

    glDisable(GL_BLEND);
}

void NetworkDebugApp::DrawImGui()
{
    for (auto& client : clients_)
    {
        client.DrawImGui();
    }
}

void NetworkDebugApp::Render()
{
    const auto& config = BasicEngine::GetInstance()->GetConfig();
    if (config.windowSize != windowSize_)
    {
        logDebug(fmt::format(
            "Resize Net Debug App Window framebuffers! with new resolution {}", config.windowSize.ToString()));
        for (auto& framebuffer : clientsFramebuffer_)
        {
            framebuffer.SetSize(config.windowSize / Vec2u(2, 1));
            framebuffer.Reload();
        }

        for (auto& client : clients_)
        {
            client.SetWindowSize(config.windowSize / Vec2u(2, 1));
        }
        windowSize_ = config.windowSize;
    }
    for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb; playerNumber++)
    {
        clientsFramebuffer_[playerNumber].Bind();
        clientsFramebuffer_[playerNumber].Clear(Color::black);
        clients_[playerNumber].Render();
    }
    gl::Framebuffer::Unbind();
    glViewport(0, 0, windowSize_.x, windowSize_.y);
    clientShader_.Bind();
    auto transform = Mat4f::Identity;
    transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
    transform = Transform3d::Translate(transform, Vec3f(-0.5f, 0.0f, 0.0f));
    clientShader_.SetMat4("transform", transform);
    clientShader_.SetTexture("texture", clientsFramebuffer_[0].GetColorTexture());
    quad_.Draw();

    transform = Mat4f::Identity;
    transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
    transform = Transform3d::Translate(transform, Vec3f(0.5f, 0.0f, 0.0f));
    clientShader_.SetMat4("transform", transform);
    clientShader_.SetTexture("texture", clientsFramebuffer_[1].GetColorTexture());
    quad_.Draw();
}

void NetworkDebugApp::OnEvent(const SDL_Event& event)
{
}
}
