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
	enum class ModelRenderMode
	{
		NONE,
		REFLECTION,
		REFRACTION,
		LENGTH
	};
	gl::RenderCuboid skyboxCube_{ Vec3f::zero, Vec3f::one*2.0f };
	gl::Shader skyboxShader_;
	
	TextureId skyboxTexture_ = 0;

	gl::Shader modelShader_;
	gl::Shader modelReflectionShader_;
	gl::Shader modelRefractionShader_;
	assimp::Model model_;
	gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
	gl::Texture cubeTexture_;
	sdl::Camera3D camera_;
	
	ModelRenderMode currentRenderMode_ = ModelRenderMode::NONE;
	float reflectionValue_ = 1.0f;
	float refractionValue_ = 1.0f;
	float refractiveIndex_ = 1.52f;

	
	
};

}
