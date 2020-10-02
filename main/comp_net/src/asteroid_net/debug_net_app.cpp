#include "asteroid_net/debug_net_app.h"
#include "imgui.h"

namespace neko::net
{
DebugApp::DebugApp()
{
}

void DebugApp::Init()
{
	const auto& config = BasicEngine::GetInstance()->config;
	windowSize_ = config.windowSize;
	for (auto& framebuffer : clientsFramebuffer_)
	{
		framebuffer.SetSize(windowSize_ / Vec2u(2, 1));
		framebuffer.Create();
	}
	quad_.Init();
	//client shader init
	clientShader_.LoadFromFile(config.dataRootPath + "shaders/comp_net/client.vert",
		config.dataRootPath + "shaders/comp_net/client.frag");
	for (auto& client : clients_)
	{
		client.Init();
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void DebugApp::Update(seconds dt)
{
	//Checking if keys are down
	const Uint8* keys = SDL_GetKeyboardState(nullptr);
	net::PlayerInput clientInput1 = 0;
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LEFT] ? asteroid::PlayerInput::LEFT : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_RIGHT] ? asteroid::PlayerInput::RIGHT : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_UP] ? asteroid::PlayerInput::UP : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_DOWN] ? asteroid::PlayerInput::DOWN : 0u);
	clientInput1 = clientInput1 | (keys[SDL_SCANCODE_LCTRL] ? asteroid::PlayerInput::SHOOT : 0u);

	clients_[0].SetPlayerInput(clientInput1);

	net::PlayerInput clientInput2 = 0;
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_A] ? asteroid::PlayerInput::LEFT : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_D] ? asteroid::PlayerInput::RIGHT : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_W] ? asteroid::PlayerInput::UP : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_S] ? asteroid::PlayerInput::DOWN : 0u);
	clientInput2 = clientInput2 | (keys[SDL_SCANCODE_SPACE] ? asteroid::PlayerInput::SHOOT : 0u);

	clients_[1].SetPlayerInput(clientInput2);
	for (auto& client : clients_)
	{
		client.Update(dt);
	}

	RendererLocator::get().Render(this);
}

void DebugApp::Destroy()
{
	for (auto& client : clients_)
	{
		client.Destroy();
	}

	glDisable(GL_BLEND);
}

void DebugApp::DrawImGui()
{
	ImGui::Begin("Rollback Simulation");
	ImGui::End();
	for (auto& client : clients_)
	{
		client.DrawImGui();
	}
}

void DebugApp::Render()
{
	const auto& config = BasicEngine::GetInstance()->config;
	if (config.windowSize != windowSize_)
	{
		for (auto& framebuffer : clientsFramebuffer_)
		{
			framebuffer.SetSize(windowSize_ / Vec2u(2, 1));
			framebuffer.Reload();
		}
	}
	for (PlayerNumber playerNumber = 0; playerNumber < asteroid::maxPlayerNmb; playerNumber++)
	{
		clientsFramebuffer_[playerNumber].Bind();
		clientsFramebuffer_[playerNumber].Clear(Vec3f(0.0f));
		clients_[playerNumber].Render();
		gl::Framebuffer::Unbind();
	}

	glViewport(0, 0, config.windowSize.x, config.windowSize.y);
	clientShader_.Bind();
	auto transform = Mat4f::Identity;
	transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
	transform = Transform3d::Translate(transform, Vec3f(-0.5f, 0.0f, 0.0f));
	clientShader_.SetMat4("transform", transform);
	clientShader_.SetTexture("texture", clientsFramebuffer_[0].GetColorTexture());
	quad_.Draw();

	transform = Mat4f::Identity;
	transform = Transform3d::Scale(transform, Vec3f(0.5f, 1.0f, 1.0f));
	transform = Transform3d::Translate(transform, Vec3f(0.5f, 0.0f, 0.0f));
	clientShader_.SetMat4("transform", transform);
	clientShader_.SetTexture("texture", clientsFramebuffer_[1].GetColorTexture());
	quad_.Draw();
}

void DebugApp::OnEvent(const SDL_Event& event)
{
}
}
