#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"

namespace neko
{

class HelloTextureProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void Render() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;

private:
    gl::RenderQuad quad_{Vec3f::zero, Vec2f::one};
    gl::Shader shader_;

    gl::TextureId textureId_ = 0u;
};
}