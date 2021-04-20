//
// Created by efarhan on 4/4/21.
//

#include <sample_program.h>
#include <hello_triangle.h>
#include <hello_input_buffer.h>

namespace neko::vk
{

SampleBrowser::SampleBrowser(VkWindow& window, VkRenderer& renderer)
{
    RegisterSample("Hello Triangle", 
        std::make_unique<HelloTriangle>(window, renderer));
    RegisterSample("Hello Input Buffer", 
        std::make_unique<HelloInputBuffer>(window, renderer));
    window.RegisterRecreateSwapchainInterface(this);
}

void SampleBrowser::Init()
{
    samplePrograms_[currentIndex_]->Init();
}

void SampleBrowser::Update(seconds dt)
{
    samplePrograms_[currentIndex_]->Update(dt);
}

void SampleBrowser::DrawImGui()
{
    ImGui::Begin("Sample Browser");
    if(ImGui::BeginCombo("Samples", sampleNames_[currentIndex_].data()))
    {
        for(std::size_t i = 0; i < sampleNames_.size(); i++)
        {
            const bool isSelected = i == currentIndex_;
            if(ImGui::Selectable(sampleNames_[i].data(), isSelected))
            {
                samplePrograms_[currentIndex_]->Destroy();
                samplePrograms_[i]->Init();
                currentIndex_ = i;
            }
            if(isSelected)
            {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();
    samplePrograms_[currentIndex_]->DrawImGui();
}

void SampleBrowser::Destroy()
{
    samplePrograms_[currentIndex_]->Destroy();
}

void SampleBrowser::CleanupSwapChain()
{
    samplePrograms_[currentIndex_]->CleanupSwapChain();
}

void SampleBrowser::CreateSwapChain()
{
    samplePrograms_[currentIndex_]->CreateSwapChain();
}

void SampleBrowser::RegisterSample(std::string_view sampleName, std::unique_ptr<SampleProgram> sample)
{
    sampleNames_.push_back(sampleName.data());
    samplePrograms_.push_back(std::move(sample));
}


SampleProgram::SampleProgram(VkWindow& window, VkRenderer& renderer) : window_(window), renderer_(renderer)
{
    
}
}