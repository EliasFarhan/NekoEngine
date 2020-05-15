#pragma once
#include "comp_graph/sample_program.h"
#include "graphics/texture.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloLightCastersProgram : public SampleProgram
{
public:
	enum class LightCasterType
	{
		DIRECTIONAL,
		POINT,
		FLASH,
		SPOT,
		LENGTH
	};

	
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;

private:
	LightCasterType casterType_ = LightCasterType::DIRECTIONAL;
	gl::Shader containerShaders_[(int)LightCasterType::LENGTH];
	gl::Shader lampShader_;
	TextureId containerDiffuse_;
	TextureId containerSpecular_;
	int specularPow_ = 32;

	gl::RenderCuboid cube_{ Vec3f::zero, Vec3f::one };

	sdl::Camera3D camera_;
	float time_ = 0.0f;
	Vec3f lightPointPos_;
	Vec3f lightDirection_ = Vec3f(-1.0f, -1.0f,0.0f).Normalized();
	const float lightDist_ = 3.0f;
	float ambientStrength_ = 0.1f;
	float diffuseStrength_ = 1.0f;
	float specularStrength_ = 0.5f;
	degree_t lightCutOffAngle_{ 30.0f };

	const static size_t cubeNumbers_ = 10;
	Vec3f cubePositions_[cubeNumbers_] =
	{
		Vec3f(0.0f, 0.0f, 0.0f),
		Vec3f(2.0f,  5.0f, -15.0f),
		Vec3f(-1.5f, -2.2f, -2.5f),
		Vec3f(-3.8f, -2.0f, -12.3f),
		Vec3f(2.4f, -0.4f, -3.5f),
		Vec3f(-1.7f, 3.0f, -7.5f),
		Vec3f(1.3f, -2.0f, -2.5f),
		Vec3f(1.5f, 2.0f, -2.5f),
		Vec3f(1.5f, 0.2f, -1.5f),
		Vec3f(-1.3f, 1.0f, -1.5f)
	};
	EulerAngles cubeAngles_[cubeNumbers_]{};
};
}
