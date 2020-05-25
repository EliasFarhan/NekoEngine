#include "10_hello_instancing/instancing_program.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{


void HelloInstancingProgram::Init()
{
	asteroidPositions_.resize(maxAsteroidNmb_);
	asteroidModels_.resize(maxAsteroidNmb_);
	asteroidForces_.resize(maxAsteroidNmb_);
	asteroidVelocities_.resize(maxAsteroidNmb_);
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Calculate Positions");
#endif
	//Calculate init pos and velocities
	for (size_t i = 0; i < maxAsteroidNmb_; i++)
	{
		const float radius = RandomRange(100.0f, 300.0f);
		const degree_t angle = degree_t(RandomRange(0.0f, 360.0f));
		Vec3f position = Vec3f::forward;
		position = Vec3f(Transform3d::RotationMatrixFrom(angle, Vec3f::up) * Vec4f(position));
		position *= radius;
		asteroidPositions_[i] = position;

		Mat4f model = Mat4f::Identity;
		model = Transform3d::Translate(model, position);
		asteroidModels_[i] = model;
	}
#ifdef EASY_PROFILE_USE
	EASY_END_BLOCK;
#endif
	const auto& config = BasicEngine::GetInstance()->config;
	model_.LoadModel(config.dataRootPath + "model/rock/rock.obj");

	singleDrawShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_single.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
	uniformInstancingShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_uniform_instancing.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
	vertexInstancingDrawShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_vertex_instancing.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
	
	camera_.position = Vec3f(0.0f, 500.0f, -500.0f);
	camera_.farPlane = 1'000.0f;
	camera_.LookAt(asteroidPositions_.front());
}

void HelloInstancingProgram::Update(seconds dt)
{
	if (!model_.IsLoaded())
	{
		return;
	}
	std::lock_guard<std::mutex> lock(updateMutex_);
	dt_ = dt.count();
	auto* engine = BasicEngine::GetInstance();
	//Kicking the velocity calculus for force and velocities
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Calculate Positions");
#endif
	CalculateForce(0, asteroidNmb_);
	CalculateVelocity(0, asteroidNmb_);
	CalculateTransforms(0, asteroidNmb_);
#ifdef EASY_PROFILE_USE
	EASY_END_BLOCK;
#endif

	const auto& config = BasicEngine::GetInstance()->config;
	camera_.SetAspect(config.windowSize.x, config.windowSize.y);
	camera_.Update(dt);
}

void HelloInstancingProgram::Destroy()
{
	model_.Destroy();
	singleDrawShader_.Destroy();
	vertexInstancingDrawShader_.Destroy();
	uniformInstancingShader_.Destroy();
}

void HelloInstancingProgram::DrawImGui()
{
	ImGui::Begin("Instancing example");
	ImGui::SliderScalar("Asteroid Nmb", ImGuiDataType_U64, &asteroidNmb_, &minAsteroidNmb_, &maxAsteroidNmb_);
	ImGui::End();
}

void HelloInstancingProgram::Render()
{
	if (!model_.IsLoaded())
	{
		return;
	}

	std::lock_guard<std::mutex> lock(updateMutex_);


	switch (instancingType_)
	{
	case InstancingType::NO_INSTANCING:
	{
#ifdef EASY_PROFILE_USE
		EASY_BLOCK("Draw No Instance");
#endif
		singleDrawShader_.Bind();
		singleDrawShader_.SetMat4("view", camera_.GenerateViewMatrix());
		singleDrawShader_.SetMat4("projection", camera_.GenerateProjectionMatrix());
		for(size_t i = 0; i < asteroidNmb_;i++)
		{
			singleDrawShader_.SetMat4("model", asteroidModels_[i]);
			model_.Draw(singleDrawShader_);
		}
		break;
	}
	case InstancingType::UNIFORM_INSTANCING: break;
	case InstancingType::BUFFER_INSTANCING: break;
	default: break;
	}

}

void HelloInstancingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

void HelloInstancingProgram::CalculateForce(size_t begin, size_t end)
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Calculate Forces");
#endif
	const size_t endCount = std::min(end, asteroidNmb_);
	for (auto i = begin; i < endCount; i++)
	{
		const auto deltaToCenter = Vec3f::zero - asteroidPositions_[i];
		const auto r = deltaToCenter.Magnitude();
		const auto force = gravityConst * centerMass * asteroidMass / (r * r);
		asteroidForces_[i] = deltaToCenter / r * force;
	}
}

void HelloInstancingProgram::CalculateVelocity(size_t begin, size_t end)
{
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Calculate Velocities");
#endif
	const size_t endCount = std::min(end, asteroidNmb_);
	for (auto i = begin; i < endCount; i++)
	{
		const auto deltaToCenter = Vec3f::zero - asteroidPositions_[i];
		auto velDir = Vec3f(-deltaToCenter.z, 0.0f, deltaToCenter.x);
		velDir = velDir.Normalized();

		const auto speed = std::sqrt(asteroidForces_[i].Magnitude() / asteroidMass * deltaToCenter.Magnitude());
		asteroidVelocities_[i] = velDir * speed;
	}
}

void HelloInstancingProgram::CalculateTransforms(size_t begin, size_t end)
{
	const size_t endCount = std::min(end, asteroidNmb_);
	for (auto i = begin; i < endCount; i++)
	{
		asteroidPositions_[i] += asteroidVelocities_[i] * dt_;
		const Mat4f model = Transform3d::Translate(Mat4f::Identity, asteroidPositions_[i]);
		asteroidModels_[i] = model;
	}
}
}
