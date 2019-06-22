/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <gtest/gtest.h>
#include <engine/engine.h>
#include <city_graph.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <imgui.h>
#include <Remotery.h>

class PathfindingEngine : public neko::BasicEngine
{
public:
	void Init() override
	{
		
		config.screenSize = sf::Vector2u(800, 800);
		
		BasicEngine::Init();
		for (int dx = 0; dx < tileNmb.x; dx++)
		{
			for (int dy = 0; dy < tileNmb.y; dy++)
			{
				const sf::Vector2i pos = sf::Vector2i(dx, dy);

				bool inactive = (rand() % 10 == 1) && pos != sf::Vector2i(0,0);
				if(!inactive)
				{
					graph_.AddNode(pos);
				}

				sf::RectangleShape rect;
				auto rectSize = sf::Vector2f(config.screenSize.x / tileNmb.x, config.screenSize.y / tileNmb.y);
				rect.setFillColor(inactive?sf::Color::Black:sf::Color::Blue);
				rect.setOutlineColor(sf::Color::Green);
				rect.setOutlineThickness(2.0f);
				rect.setOrigin(rectSize/2.0f);
				rect.setPosition(sf::Vector2f(rectSize.x*dx, rectSize.y*dy)+ rectSize / 2.0f);
				rect.setSize(rectSize);
				rects_.push_back(rect);
			}
		}
	}
	void Update() override
	{
		BasicEngine::Update();
		auto mousePos = mouseManager_.GetPosition();
		auto rectSize = sf::Vector2f(config.screenSize.x / tileNmb.x, config.screenSize.y / tileNmb.y);
		sf::Vector2i tilePos = sf::Vector2i(mousePos.x / rectSize.x, mousePos.y / rectSize.y);
		const auto path = graph_.CalculateShortestPath(sf::Vector2i(0, 0), tilePos);
		int i = 0;
		for (int dx = 0; dx < tileNmb.x; dx++)
		{
			for (int dy = 0; dy < tileNmb.y; dy++)
			{
				const sf::Vector2i pos = sf::Vector2i(dx, dy);
				const Index rectIndex = i;
				if (std::find(path.begin(), path.end(), pos) != path.end())
				{
					rects_[rectIndex].setFillColor(sf::Color::Red);
				}
				else
				{
					bool inactive = rects_[rectIndex].getFillColor() == sf::Color::Black;
					rects_[rectIndex].setFillColor(inactive?sf::Color::Black:sf::Color::Blue);
				}
				i++;
			}
		}
		{
			rmt_ScopedCPUSample(DrawRectangles, 0);
			for (auto& rect : rects_)
			{
				renderWindow->draw(rect);
			}
		}

		ImGui::Begin("Inspector");
		ImGui::LabelText("FPS", "%f", 1.0f/dt.asSeconds());
		ImGui::LabelText("Path Length", "%d", path.size());
		ImGui::End();
	}
	void Destroy() override
	{
		BasicEngine::Destroy();
	}
private:
	std::vector<sf::RectangleShape> rects_;
    neko::TileMapGraph graph_;
	const sf::Vector2u tileNmb = sf::Vector2u(200, 200);

};

TEST(CityBuilder, Pathfinding)
{
    PathfindingEngine engine;
    engine.Init();
    engine.EngineLoop();
}