#include <SFML/Window/Event.hpp>
#include "sfml_engine/engine.h"
#include "imgui-SFML.h"

namespace neko::sfml
{

void SfmlBasicEngine::Init()
{
	BasicEngine::Init();
	window_ = std::make_unique<sf::RenderWindow>(
		sf::VideoMode(config.realWindowSize.first, config.realWindowSize.second),
		config.windowName, config.fullscreen ? sf::Style::Fullscreen : sf::Style::Default);

	if (config.vSync)
	{
		window_->setVerticalSyncEnabled(config.vSync);
	}
	else
	{
		window_->setFramerateLimit(config.framerateLimit);
	}
	ImGui::SFML::Init(*window_);
	mouseManager_.SetWindow(window_.get());
}

void SfmlBasicEngine::Update(float dt)
{
	keyboardManager_.ClearKeys();
	mouseManager_.ClearFrameData();
	sf::Event event{};
	while (window_->pollEvent(event))
	{
		OnEvent(event);
		ImGui::SFML::ProcessEvent(event);
	}

	window_->clear(sf::Color::Black);
	updateDelegate_.Execute(dt);
	RenderTarget renderTarget{ window_.get() };
	graphicsManager_.RenderAll(&renderTarget);
	ImGui::SFML::Update(*window_, sf::seconds(dt));
	//update Ui
	drawUiDelegate_.Execute(dt);
	ImGui::SFML::Render(*window_);
	window_->display();
}



void SfmlBasicEngine::Destroy()
{
	BasicEngine::Destroy();
	window_->close();
	ImGui::SFML::Shutdown();
	window_ = nullptr;
}

void SfmlBasicEngine::OnEvent(sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:
	{
		isRunning_ = false;
		break;
	}
	case sf::Event::KeyPressed:
	{

		keyboardManager_.AddPressKey(event.key.code);
		break;
	}
	case sf::Event::KeyReleased:
	{
		if (event.key.code == sf::Keyboard::Escape)
		{
			isRunning_ = false;
		}
		keyboardManager_.AddReleaseKey(event.key.code);
		break;
	}
	case sf::Event::MouseWheelScrolled:
	{
		mouseManager_.OnWheelScrolled(event);
		break;
	}
	case sf::Event::Resized:
	{
		config.realWindowSize = std::pair<unsigned, unsigned>(event.size.width, event.size.height);
		break;
	}
	default:
	{
		break;
	}
	}
}

SfmlFullEngine::SfmlFullEngine(Configuration* config) :
	SfmlBasicEngine(config),
	position2dManager_(entityManager_),
	scale2dManager_(entityManager_),
	rotation2dManager_(entityManager_),
	transform2dManager_(position2dManager_, scale2dManager_, rotation2dManager_),
	physics2dManager_(contactListener_),
	body2dManager_(entityManager_, physics2dManager_),
	spriteManager_(entityManager_, textureManager_),
	contactListener_(*this),
	spineManager_(entityManager_),
	spineBoneFollowerManager_(entityManager_)
{
	physics2dManager_.SetConfiguration(config);
}

void SfmlFullEngine::OnBeginContact(const box2d::Collider* collider1, const box2d::Collider* collider2)
{
	contactBeginDelegate_.Execute(collider1, collider2);
}

void SfmlFullEngine::OnEndContact(const box2d::Collider* collider1, const box2d::Collider* collider2)
{
	contactEndDelegate_.Execute(collider1, collider2);
}
}
