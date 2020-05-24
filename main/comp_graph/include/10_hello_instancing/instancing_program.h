#pragma once
#include "comp_graph/sample_program.h"
#include "gl/model.h"
#include "gl/shader.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloInstancingProgram : public SampleProgram
{
public:
	HelloInstancingProgram();
	enum class InstancingType
	{
		NO_INSTANCING,
		UNIFORM_INSTANCING,
		BUFFER_INSTANCING,
		LENGTH
	};
	
	void Init() override;
	void Update(seconds dt) override;
	void Destroy() override;
	void DrawImGui() override;
	void Render() override;
	void OnEvent(const SDL_Event& event) override;
private:
	void CalculateForce(size_t begin, size_t end);
	void CalculateVelocity(size_t begin, size_t end);
	
	sdl::Camera3D camera_;
	assimp::Model model_;

	const size_t maxAsteroidNmb_ = 100'000;
	size_t asteroidNmb = 1000;

	gl::Shader singleDrawShader_;
	gl::Shader uniformInstancingShader_;
	gl::Shader vertexInstancingDrawShader_;
	
	std::vector<Vec3f> planetPositions_;
	std::vector<Vec3f> planetVelocities_;
	std::vector<Vec3f> planetForces_;
	std::vector<Mat4f> planetModels_;
	std::array<Job, 4> planetMotionUpdate_;
	int instanceVBO_ = 0;

	const float gravityConst = 1000.0f;
	const float centerMass = 1000.0f;
	const float planetMass = 1.0f;
};

	
}
