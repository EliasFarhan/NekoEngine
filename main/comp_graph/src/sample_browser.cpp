//
// Created by efarhan on 26.01.20.
//

#include <imgui.h>
#include "comp_graph/sample_browser.h"


#include "01_hello_triangle/triangle_program.h"
#include "02_hello_texture/texture_program.h"
#include "03_hello_transform/transform_program.h"
#include "04_hello_coords/coords_program.h"

namespace neko
{


void SampleBrowser::Init()
{
    RegisterRenderProgram("01 Hello Triangle", std::make_unique<HelloTriangleProgram>());
    RegisterRenderProgram("02 Hello Texture", std::make_unique<HelloTextureProgram>());
    RegisterRenderProgram("03 Hello Transform", std::make_unique<HelloTransformProgram>());
    RegisterRenderProgram("04 Hello Coords", std::make_unique<HelloCoordsProgram>());
    programs_[currentProgramIndex_]->Init();
}

void SampleBrowser::Update(seconds dt)
{
    programs_[currentProgramIndex_]->Update(dt);
    RendererLocator::get().Render(programs_[currentProgramIndex_].get());
    
}

void SampleBrowser::Destroy()
{
    programs_[currentProgramIndex_]->Destroy();
}

void SampleBrowser::SwitchToProgram(size_t index)
{
    const auto previousindex = currentProgramIndex_;
    currentProgramIndex_ = index;
    programs_[previousindex]->Destroy();
    programs_[currentProgramIndex_]->Init();
}


void SampleBrowser::OnEvent(const SDL_Event& event)
{
    programs_[currentProgramIndex_]->OnEvent(event);
}


void SampleBrowser::DrawImGui()
{
    ImGui::Begin("Sample Browser");
    if (ImGui::BeginCombo("Current Sample",
                          programsNames_[currentProgramIndex_].c_str())) // The second parameter is the label previewed before opening the combo.
    {
        for (int n = 0; n < programsNames_.size(); n++)
        {
	        const bool is_selected = (currentProgramIndex_ == n);
            if (ImGui::Selectable(programsNames_[n].c_str(), is_selected))
            {
                SwitchToProgram(n);
            }
            if (is_selected)
            {
                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();
    programs_[currentProgramIndex_]->DrawImGui();
}

size_t SampleBrowser::RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program)
{
    const auto index = programs_.size();
    programsNames_.emplace_back(name.data());
    programs_.push_back(std::move(program));
    return index;
}
}