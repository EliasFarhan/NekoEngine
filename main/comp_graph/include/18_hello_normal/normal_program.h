#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloNormalProgram : public SampleProgram
{
public:


    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;
    void OnEvent(const SDL_Event& event) override;

private:
    gl::Shader normalShader_;
    gl::Texture diffuseTex_;
    gl::Texture normalTex_;
    gl::RenderQuad plane_{Vec3f::zero, Vec2f::one};

    assimp::Model model_;

    sdl::Camera3D camera_;
};
}