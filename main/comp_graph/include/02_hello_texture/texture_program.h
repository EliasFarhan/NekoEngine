#pragma once

#include <comp_graph/sample_program.h>
#include "gl/shape.h"
#include "gl/shader.h"

namespace neko
{

class HelloTextureProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void Render() override;

    void DrawUi(seconds dt) override;

    void OnEvent(const SDL_Event& event) override;

private:
    neko::gl::Quad quad_{Vec2f::Zero, Vec2f::One};
    gl::Shader shader_;

    unsigned int textureId_;
};
}