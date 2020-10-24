#pragma once

#include <comp_graph/sample_program.h>
#include <gl/texture.h>
#include <gl/material.h>
#include <gl/shape.h>

namespace neko
{
class HelloMaterialProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;
private:
    gl::RenderQuad quad_{Vec3f::zero, Vec2f::one};
    gl::TextureManager textureManager_;
    gl::Material material_;
};
}