#pragma once

#include <comp_net/sample_program.h>

#include "gl/shader.h"
#include "gl/shape.h"
#include "asteroid_simulation/simulation_client.h"
#include "asteroid_simulation/simulation_server.h"

namespace neko::net
{
class SimulationDebugApp : public SampleProgram
{
public:
    SimulationDebugApp();

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;
private:
    std::array<std::unique_ptr<SimulationClient>, asteroid::maxPlayerNmb> clients_;
    std::array<gl::Framebuffer, asteroid::maxPlayerNmb> clientsFramebuffer_;
    SimulationServer server_;
    gl::RenderQuad quad_{ Vec3f::zero, Vec2f(2.0f) };
    gl::Shader clientShader_;
    Vec2u windowSize_;
};



}

