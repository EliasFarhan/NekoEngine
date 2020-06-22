#pragma once
#include "comp_graph/sample_program.h"
#include "gl/texture.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloBlendingProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum BlendingFlags
	{
		NONE = 0u,
		ENABLE_BLENDING = 1u,
		ENABLE_SORTING = 1u << 1u
	};
	gl::RenderQuad plane_{ Vec3f::zero, Vec2f::one };
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
	gl::Shader cutoffShader_;

	gl::Texture windowTexture_;
	gl::Texture cubeTexture_;

	sdl::Camera3D camera_;

	TextureId whiteTexture_ = 0;
	std::uint8_t flags_ = NONE;
};
}
