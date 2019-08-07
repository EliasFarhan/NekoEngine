
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
#include <tools/scene_viewer.h>
#include <imgui.h>
#include <imgui-SFML.h>
namespace editor
{

void SceneViewer::Update(sf::RenderTexture& sceneTexture)
{
    if (ImGui::BeginTabItem("Scene Viewer"))
    {
        auto tintColor = sf::Color::White;
        auto borderColor = sf::Color::White;
        const auto imWindowSize = ImGui::GetWindowSize();
        auto windowSize = sf::Vector2f(imWindowSize.x, imWindowSize.y);
        const auto ratio = sf::Vector2f(windowSize.x / sceneTexture.getSize().x,
                                        windowSize.y / sceneTexture.getSize().y);
        windowSize = sf::Vector2f(sceneTexture.getSize()) * (ratio.x < ratio.y ? ratio.x : ratio.y);
        ImGui::Image(sceneTexture.getTexture(), sf::Vector2f(windowSize.x - 25.f, windowSize.y - 40.f), tintColor,
                     borderColor);
        ImGui::EndTabItem();
    }
}
}