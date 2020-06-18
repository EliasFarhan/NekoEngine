#pragma once
#include "comp_graph/sample_program.h"
#include "gl/texture.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloCascadedShadowProgram : public SampleProgram
{
public:
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	struct DirectionalLight
	{
		Vec3f direction = -Vec3f::one;
	};

	void RenderScene(const gl::Shader& shader);

	sdl::Camera3D camera_;
	Camera2D lightCamera_;

	gl::Shader simpleDepthShader_;
	gl::Shader shadowShader_;

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned fbo_ = 0;
	std::array<unsigned, 3> shadowMaps_{};
	float cascadedNearRatio_ = 0.1f;
	float cascadedMiddleRatio_ = 0.6f;
	DirectionalLight light_;

	assimp::Model dragonModel_;
	TextureId whiteTexture_;
};
}
