//
// Created by efarhan on 26.01.20.
//

#include <imgui.h>
#include "comp_graph/sample_browser.h"

#include "01_hello_triangle/triangle_program.h"
#include "02_hello_texture/texture_program.h"
#include "03_hello_transform/transform_program.h"
#include "04_hello_coords/coords_program.h"
#include "05_hello_camera/camera_program.h"
#include "06_hello_model/model_program.h"
#include "07_hello_light/light_program.h"
#include "08_hello_lightmaps/lightmaps_program.h"
#include "09_hello_lightcasters/lightcasters_program.h"
#include "10_hello_instancing/instancing_program.h"
#include "11_hello_framebuffer/framebuffer_program.h"
#include "12_hello_stencil/stencil_program.h"
#include "13_hello_depth/depth_program.h"
#include "14_hello_outline/outline_program.h"
#include "15_hello_cubemaps/cubemaps_program.h"
#include "16_hello_culling/culling_program.h"
#include "17_hello_frustum/frustum_program.h"
#include "18_hello_normal/normal_program.h"
#include "19_hello_hdr/hdr_program.h"
#include "20_hello_bloom/bloom_program.h"
#include "21_hello_shadow/shadow_program.h"

#include "98_hello_line/line_program.h"
#include "99_hello_scene/scene_program.h"

namespace neko
{

void SampleBrowser::Init()
{
    RegisterRenderProgram("01 Hello Triangle", std::make_unique<HelloTriangleProgram>());
    RegisterRenderProgram("02 Hello Texture", std::make_unique<HelloTextureProgram>());
    RegisterRenderProgram("03 Hello Transform", std::make_unique<HelloTransformProgram>());
    RegisterRenderProgram("04 Hello Coords", std::make_unique<HelloCoordsProgram>());
    RegisterRenderProgram("05 Hello Camera", std::make_unique<HelloCameraProgram>());
    RegisterRenderProgram("06 Hello Model", std::make_unique<HelloModelProgram>());
    RegisterRenderProgram("07 Hello Light", std::make_unique<HelloLightProgram>());
    RegisterRenderProgram("08 Hello Lightmaps", std::make_unique<HelloLightMapsProgram>());
    RegisterRenderProgram("09 Hello Light Casters", std::make_unique<HelloLightCastersProgram>());
    RegisterRenderProgram("10 Hello Instancing", std::make_unique<HelloInstancingProgram>());
	RegisterRenderProgram("11 Hello Framebuffer", std::make_unique<HelloFramebufferProgram>());
    RegisterRenderProgram("12 Hello Stencil", std::make_unique<HelloStencilProgam>());
    RegisterRenderProgram("13 Hello Depth", std::make_unique<HelloDepthProgram>());
    RegisterRenderProgram("14 Hello Outline", std::make_unique<HelloOutlineProgram>());
    RegisterRenderProgram("15 Hello Cubemaps", std::make_unique<HelloCubemapsProgram>());
    RegisterRenderProgram("16 Hello Culling", std::make_unique<HelloCullingProgram>());
    RegisterRenderProgram("17 Hello Frustum Culling", std::make_unique<HelloFrustumProgram>());
    RegisterRenderProgram("18 Hello Normal", std::make_unique<HelloNormalProgram>());
    RegisterRenderProgram("19 Hello Hdr", std::make_unique<HelloHdrProgram>());
    RegisterRenderProgram("20 Hello Bloom", std::make_unique<HelloBloomProgram>());
    RegisterRenderProgram("21 Hello Shadow", std::make_unique<HelloShadowProgram>());

    RegisterRenderProgram("98 Hello Line", std::make_unique<HelloLineProgram>());
    RegisterRenderProgram("99 Hello Scene", std::make_unique<HelloSceneProgram>());
	
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
    const auto previousIndex = currentProgramIndex_;
    currentProgramIndex_ = index;
    programs_[previousIndex]->Destroy();
    programs_[currentProgramIndex_]->Init();
}


void SampleBrowser::OnEvent(const SDL_Event& event)
{
    programs_[currentProgramIndex_]->OnEvent(event);
}


void SampleBrowser::DrawImGui()
{

    ImGui::SetNextWindowPos(ImVec2(0, 200), ImGuiCond_FirstUseEver);
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