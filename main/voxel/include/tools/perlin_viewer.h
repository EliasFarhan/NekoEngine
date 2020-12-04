#pragma once

#include <gl/shape.h>
#include <gl/font.h>
#include <gl/shader.h>
#include "engine/system.h"
#include "graphics/graphics.h"
#include "gl/gles3_include.h"
#include "graphics/texture.h"

namespace neko::voxel
{

class PerlinViewer : public SystemInterface, public DrawImGuiInterface, public RenderCommandInterface
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

private:
    void ReloadTexture();

    std::int32_t currentOctave_ = 1;
    float currentScale_ = 1.0f;
    inline static const size_t textureSize_ = 1024;
    std::vector<std::array<float, textureSize_>> perlinValues_;
    neko::TextureName perlinTexture_ = neko::INVALID_TEXTURE_NAME;

    neko::gl::RenderQuad renderQuad_{Vec3f::zero, Vec2f::one};
    neko::gl::Shader viewerShader_;

};
}