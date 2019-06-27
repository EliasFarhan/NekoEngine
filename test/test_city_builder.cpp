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
		roadPositions_.reserve(roadNmb_);
		rects_.reserve(roadNmb_);
		sf::Vector2i direction = sf::Vector2i(1,0);
		sf::Vector2i currentPos = sf::Vector2i(0,0);
		const auto inverseVector2i = [](const sf::Vector2i dir, bool negative = false)->sf::Vector2i{
		    return (negative?-1:1)*sf::Vector2i(dir.y, dir.x);
		};
		//Adding initial pos
		{
			sf::RectangleShape rect;

			const auto rectSize = sf::Vector2f(float(config.screenSize.x / tileNmb.x), float(config.screenSize.y / tileNmb.y));
			rect.setFillColor(sf::Color::Blue);
			rect.setOutlineColor(sf::Color::Green);
			rect.setOutlineThickness(outlineThickness);
			rect.setOrigin(rectSize / 2.0f);
			rect.setPosition(sf::Vector2f(rectSize.x * currentPos.x, rectSize.y * currentPos.y) + rectSize / 2.0f);
			rect.setSize(rectSize - sf::Vector2f(outlineThickness, outlineThickness)*2.0f);
			roadPositions_.push_back(currentPos);
			rects_.push_back(rect);
			graph_.AddNode(currentPos);
		}
		for(Index i = 0u; i < roadNmb_;i++)
        {
		    const int randValue = rand()%20;
		    if(randValue == 0)
            {
		        direction = inverseVector2i(direction);
            }
            if(randValue == 1)
            {
                direction = inverseVector2i(direction, true);
            }

            sf::Vector2i newPos = currentPos+direction;
            if(newPos.x < 0 || newPos.x >= tileNmb.x)
            {
                direction.x = -direction.x;
                newPos = currentPos+direction;
            }

            if(newPos.y < 0 || newPos.y >= tileNmb.y)
            {
                direction.y = -direction.y;
                newPos = currentPos+direction;
            }
            currentPos = newPos;
            if(graph_.ContainNode(newPos))
            {
                i--;
                continue;
            }
            sf::RectangleShape rect;

            const auto rectSize = sf::Vector2f(float(config.screenSize.x / tileNmb.x), float(config.screenSize.y / tileNmb.y));
            rect.setFillColor(sf::Color::Blue);
            rect.setOutlineColor(sf::Color::Green);
            rect.setOutlineThickness(outlineThickness);
            rect.setOrigin(rectSize/2.0f);
            rect.setPosition(sf::Vector2f(rectSize.x * newPos.x, rectSize.y * newPos.y) + rectSize / 2.0f);
            rect.setSize(rectSize-sf::Vector2f(outlineThickness, outlineThickness)*2.0f);
            roadPositions_.push_back(newPos);
            rects_.push_back(rect);
            graph_.AddNode(newPos);



        }
	}
	void Update() override
	{
		BasicEngine::Update();
		const auto mousePos = mouseManager_.GetPosition();
		const auto rectSize = sf::Vector2f(float(config.screenSize.x / tileNmb.x), float(config.screenSize.y / tileNmb.y));
		const sf::Vector2i tilePos = sf::Vector2i(int(mousePos.x / rectSize.x), int(mousePos.y / rectSize.y));
		const auto path = graph_.CalculateShortestPath(sf::Vector2i(0, 0), tilePos);

		for (Index i = 0; i < roadPositions_.size();i++)
		{
            const sf::Vector2i pos = roadPositions_[i];
            if (std::find(path.begin(), path.end(), pos) != path.end())
            {
                rects_[i].setFillColor(sf::Color::Red);
            }
            else
            {
                rects_[i].setFillColor(sf::Color::Blue);
            }
		}
		{
			rmt_ScopedCPUSample(DrawRectangles, 0);
			for (const auto& rect : rects_)
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
	const sf::Vector2i tileNmb = sf::Vector2i(200, 200);
	const size_t roadNmb_ = 20'000;
	const float outlineThickness = 1.0f;
    std::vector<sf::Vector2i> roadPositions_;
};

TEST(CityBuilder, Pathfinding)
{
    PathfindingEngine engine;
    engine.Init();
    engine.EngineLoop();
}

