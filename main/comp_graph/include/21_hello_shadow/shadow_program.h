#pragma once

#include "comp_graph/sample_program.h"
namespace neko
{
class HelloShadowProgram : public SampleProgram
{
public:
    void OnEvent(const SDL_Event& event) override;

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

};
}