#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloDepthProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;

	
private:

	enum DepthFlag
	{
		NONE = 0,
		DEPTH_ONLY = 1,
		LINEAR_DEPTH = 1 << 1
	};

	void DrawScene();
	
	sdl::Camera3D camera_;
	
	gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
	gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*5.0f};
	gl::RenderQuad screenPlane_{Vec3f::zero, Vec2f::one*2.0f};

	gl::Shader depthOnlyShader_;
	gl::Shader screenShader_;
	gl::Shader sceneShader_;
	unsigned int fbo_ = 0;
	unsigned int screenTexture_ = 0;
	unsigned int depthTexture_ = 0;
	std::uint8_t flags_ = NONE;
};

}
