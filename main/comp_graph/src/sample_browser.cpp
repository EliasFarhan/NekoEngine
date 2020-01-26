//
// Created by efarhan on 26.01.20.
//

#include "comp_graph/sample_browser.h"

#include "01_hello_triangle/triangle_program.h"

namespace neko
{


void SampleBrowser::Init()
{
RegisterRenderProgram("01 Hello Triangle", std::make_unique<HelloTriangleProgram>());

programs_[currentProgramIndex_]->Init();
}

void SampleBrowser::Update(seconds dt)
{
programs_[currentProgramIndex_]->Update(dt);
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

void SampleBrowser::Render()
{
    programs_[currentProgramIndex_]->Render();
}

void SampleBrowser::DrawGui(seconds dt)
{
    programs_[currentProgramIndex_]->DrawUi(dt);
}

size_t SampleBrowser::RegisterRenderProgram(const std::string_view name, std::unique_ptr<SampleProgram> program)
{
    const auto index = programs_.size();
    programsNames_.emplace_back(name.data());
    programs_.push_back(std::move(program));
    return index;
}
}