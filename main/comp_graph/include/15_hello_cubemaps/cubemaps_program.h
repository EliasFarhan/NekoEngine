#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{

class HelloCubemapsProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one*2.0f };
	gl::Shader skyboxShader_;
	gl::Shader modelShader_;
	TextureId skyboxTexture_ = 0;
	assimp::Model model_;
	sdl::Camera3D camera_;




	
	
};

}
