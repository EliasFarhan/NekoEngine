#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "gl/model.h"
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
	enum NormalFlags : std::uint8_t
	{
		NONE = 0u,
		ENABLE_NORMAL_MAP = 1u<<0u,
		ENABLE_MODEL = 1u<<1u
	};
    gl::Shader normalShader_;
    gl::Shader diffuseShader_;
    gl::Texture diffuseTex_;
    gl::Texture normalTex_;
    gl::RenderQuad plane_{Vec3f::zero, Vec2f::one};

    assimp::Model model_;

    sdl::Camera3D camera_;
    Vec3f lightPos_ = Vec3f::one * 3.0f;
    float dt_ = 0.0f;
    std::uint8_t flags_ = NONE;
};
}