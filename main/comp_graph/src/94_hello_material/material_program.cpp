//
// Created by efarhan on 10/24/20.
//

#include "94_hello_material/material_program.h"

namespace neko
{


void HelloMaterialProgram::Init()
{
    const auto& config = BasicEngine::GetInstance()->config;
    quad_.Init();
    material_.LoadFromFile(config.dataRootPath+"material/test.mat");
}

void HelloMaterialProgram::Update(seconds dt)
{

}

void HelloMaterialProgram::Destroy()
{

}

void HelloMaterialProgram::DrawImGui()
{

}

void HelloMaterialProgram::Render()
{

}

void HelloMaterialProgram::OnEvent(const SDL_Event& event)
{

}

}