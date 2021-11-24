/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <engine/editor.h>

#include <imgui.h>
#include <imgui-SFML.h>
#include <engine/engine.h>
#include <graphics/graphics.h>

#include <utility>
#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{
void Editor::Init()
{

}

void Editor::Update(float dt)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const auto* engine = MainEngine::GetInstance();
	const Index frameIndex = (engine->frameIndex - 1) % 2; //Render frame is always the previous one
	ImGui::SFML::Update(*renderWindow_, engine->clockDeltatime);

	ImGui::Begin("Inspector");
	//Draw inspector data
	for (auto& inspectorData : inspectorValues_[frameIndex])
	{
		ImGui::LabelText(inspectorData.first.c_str(), "%s", inspectorData.second.c_str());
	}
	ImGui::End();
	inspectorValues_[frameIndex].clear();

	ImGui::SFML::Render(*renderWindow_);
}

void Editor::Destroy()
{

}

void Editor::AddInspectorInfo(const std::string_view name, const std::string_view value)
{
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
	inspectorValues_[frameIndex].push_back(std::pair<std::string, std::string>(name, value));
}
}