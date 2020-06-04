#pragma once
#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloCullingProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	enum CullingFlags : std::uint8_t
	{
		NONE,
		CULLING = 1u << 0u,
		BACK_CULLING = 1u << 1u,
		CCW = 1u << 2u
	};
	
	sdl::Camera3D camera_;
	assimp::Model model_;
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };
	gl::Texture cubeTexture_;
	gl::Shader modelShader_;

	sdl::Camera3D externalCamera_;

	std::uint8_t flags_ = BACK_CULLING | CCW;
};

}
