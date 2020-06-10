#pragma once
#include "comp_graph/sample_program.h"
#include "gl/model.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloBlinnProgram : public SampleProgram
{
public:
    virtual ~HelloBlinnProgram()= default;
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	gl::Shader modelShader_;
	gl::Shader blinnShader_;

	assimp::Model model_;
	gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*10.0f};
	gl::Texture floorTexture_;
	float floorResolution_ = 6.0f;
	int specularPow_ = 32;
	enum BlinnFlags : std::uint8_t
	{
		NONE = 0u,
		ENABLE_BLINN = 1u << 0u,
		ONLY_SPECULAR = 1u << 1u
	};
	sdl::Camera3D camera_;
	Vec3f lightPos_;
	float dt_ = 0.0f;
	std::uint8_t flags_ = NONE;
};
}
