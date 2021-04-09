//
// Created by efarhan on 4/4/21.
//

#include <sample_program.h>
#include <hello_triangle.h>

namespace neko::vk
{

SampleBrowser::SampleBrowser(VkWindow& window, VkRenderer& renderer)
{
    samplePrograms_.push_back(std::make_unique<HelloTriangle>(window, renderer));
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


SampleProgram::SampleProgram(VkWindow& window, VkRenderer& renderer) : window_(window), renderer_(renderer)
{
    auto* engine = BasicEngine::GetInstance();
    window_.RegisterRecreateSwapchainInterface(this);
    
}
}