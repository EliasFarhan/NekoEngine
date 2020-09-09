#pragma once

#include <comp_net/sample_program.h>

#include "02_rollback_simulation/rollback_server.h"
#include "02_rollback_simulation/rollback_client.h"

#include "gl/shader.h"
#include "gl/shape.h"

namespace neko::net
{
class HelloRollbackSimulation : public SampleProgram
{
public:
    HelloRollbackSimulation();
    void OnEvent(const SDL_Event& event) override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;
private:
    std::array<std::unique_ptr<SimulationClient>, 2> clients_;
    SimulationServer server_;
    gl::RenderQuad quad_{ Vec3f::zero, Vec2f(2.0f) };
    gl::Shader clientShader_;
};
}
