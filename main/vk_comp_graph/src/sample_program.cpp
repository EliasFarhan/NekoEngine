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
    samplePrograms_[0]->Init();
}

void SampleBrowser::Update(seconds dt)
{
    samplePrograms_[0]->Update(dt);
}

void SampleBrowser::Destroy()
{
    samplePrograms_[0]->Destroy();
}


SampleProgram::SampleProgram(VkWindow& window, VkRenderer& renderer) : window_(window), renderer_(renderer)
{

}
}