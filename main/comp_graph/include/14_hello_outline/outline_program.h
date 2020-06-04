#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "gl/shader.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloOutlineProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum OutlineFlags : std::uint8_t
	{
		NONE = 0u,
		DRAW_OUTLINE = 1u << 0u,
		USE_STENCIL = 1u << 1u
	};
	gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
	gl::Shader modelShader_;
	gl::Shader outlineShader_;

	gl::Texture cubeTexture_;

	sdl::Camera3D camera_;

	Vec3f outlineColor_ = Vec3f::one;
	float outlineScale_ = 1.1f;
	std::uint8_t flags_ = NONE;
};

}
