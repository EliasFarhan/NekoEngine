#pragma once

#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "gl/shape.h"

namespace neko
{

class HelloFramebufferProgram : public SampleProgram
{
public:
    enum class PostProcessingType
    {
        NO_POSTPROCESS,
        INVERSE,
        GRAYSCALE,
        BLUR,
        EDGE_DETECTION,
        LENGTH
    };

    HelloFramebufferProgram();

    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;
private:
    sdl::Camera3D camera_;

    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
    gl::Shader modelShader_;
    gl::Texture containerTexture_;

    gl::RenderQuad screenFrame_;
    gl::Shader screenShader_;
    gl::Shader screenInverseShader_;
    gl::Shader screenGrayscaleShader_;
    gl::Shader screenBlurShader_;
    gl::Shader screenEdgeDetectionShader_;

    PostProcessingType postProcessingType_ = PostProcessingType::NO_POSTPROCESS;

    unsigned int fbo_;
    unsigned int rbo_;
    unsigned int fboColorBufferTexture_;
    bool hasScreenResize_ = false;
};


}