#include "10_hello_instancing/instancing_program.h"
#include "imgui.h"
#ifdef EASY_PROFILE_USE
#include "easy/profiler.h"
#endif
namespace neko
{
HelloInstancingProgram::HelloInstancingProgram() :
	planetMotionUpdate_({
	Job([] {}),
	Job([] {}),
	Job([] {}),
	Job([] {})
		})
{
	const size_t jobCount = planetMotionUpdate_.size();
	for (size_t i = 0; i < jobCount; i++)
	{
		planetMotionUpdate_[i].SetTask(
			[this, &i, &jobCount]
			{
				const auto begin = i * maxAsteroidNmb_ / jobCount;
				const auto end = (i + 1) * maxAsteroidNmb_ / jobCount;
				CalculateForce(begin, end);
				CalculateVelocity(begin, end);
			});
	}

}

void HelloInstancingProgram::Init()
{
	planetPositions_.resize(maxAsteroidNmb_);
	planetModels_.resize(maxAsteroidNmb_);
	planetForces_.resize(maxAsteroidNmb_);
#ifdef EASY_PROFILE_USE
	EASY_BLOCK("Calculate Positions");
#endif
	//Calculate init pos and velocities
	for (size_t i = 0; i < maxAsteroidNmb_; i++)
	{
		const float radius = RandomRange(100.0f, 300.0f);
		const degree_t angle = degree_t(RandomRange(0.0f, 359.0f));
		Vec3f position = Vec3f::forward;
		position = Vec3f(Transform3d::RotationMatrixFrom(angle, Vec3f::up) * Vec4f(position));
		position *= radius;
		planetPositions_[i] = position;

		Mat4f model = Mat4f::Identity;
		model = Transform3d::Translate(model, position);
		planetModels_[i] = model;
	}

	const auto& config = BasicEngine::GetInstance()->config;
	model_.LoadModel(config.dataRootPath + "model/rock/rock.obj");

	singleDrawShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_single.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
	uniformInstancingShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_uniform_instancing.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
	vertexInstancingDrawShader_.LoadFromFile(config.dataRootPath + "shaders/10_hello_instancing/asteroid_vertex_instancing.vert",
		config.dataRootPath + "shaders/10_hello_instancing/asteroid.frag");
}

void HelloInstancingProgram::Update(seconds dt)
{
	std::lock_guard<std::mutex> lock(updateMutex_);

	auto* engine = BasicEngine::GetInstance();
	//Kicking the velocity calculus for force and velocities
	for (size_t i = 0; i < planetMotionUpdate_.size(); i++)
	{
		engine->ScheduleJob(&planetMotionUpdate_[i], JobThreadType::OTHER_THREAD);
	}
}

void HelloInstancingProgram::Destroy()
{
}

void HelloInstancingProgram::DrawImGui()
{
	ImGui::Begin("Instancing example");
	ImGui::End();
}

void HelloInstancingProgram::Render()
{
	std::lock_guard<std::mutex> lock(updateMutex_);
	for (auto& job : planetMotionUpdate_)
	{
		job.Join();
	}
	for (auto& job : planetMotionUpdate_)
	{
		job.Reset();
	}

}

void HelloInstancingProgram::OnEvent(const SDL_Event& event)
{
	camera_.OnEvent(event);
}

void HelloInstancingProgram::CalculateForce(size_t begin, size_t end)
{
	const size_t endCount = std::min(endCount, asteroidNmb);
	for (auto i = begin; i < endCount; i++)
	{
		const auto deltaToCenter = Vec3f::zero - planetPositions_[i];
		const auto r = deltaToCenter.Magnitude();
		const auto force = gravityConst * centerMass * planetMass / (r * r);
		planetForces_[i] = deltaToCenter / r * force;
	}
}

void HelloInstancingProgram::CalculateVelocity(size_t begin, size_t end)
{

	const size_t endCount = std::min(endCount, asteroidNmb);
	for (auto i = begin; i < endCount; i++)
	{
		const auto deltaToCenter = Vec3f::zero - planetPositions_[i];
		auto velDir = Vec3f(-deltaToCenter.z, 0.0f, deltaToCenter.x);
		velDir = velDir.Normalized();

		const auto speed = std::sqrt(planetForces_[i].Magnitude() / planetMass * deltaToCenter.Magnitude());
		planetVelocities_[i] = velDir * speed;
	}
}
}
