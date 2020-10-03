#pragma once

#include <comp_net/sample_program.h>


#include "network_client.h"
#include "gl/shader.h"
#include "gl/shape.h"

namespace neko::net
{
class DebugApp : public SampleProgram
{
public:
    DebugApp();

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;
private:
    std::array<ClientNetworkManager, asteroid::maxPlayerNmb> clients_;
    std::array<gl::Framebuffer, asteroid::maxPlayerNmb> clientsFramebuffer_;
    gl::RenderQuad quad_{ Vec3f::zero, Vec2f(2.0f) };
    gl::Shader clientShader_;
    Vec2u windowSize_;
};



}

