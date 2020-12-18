#pragma once
#include "engine/system.h"
#include "gl/shape.h"
#include "glad/glad.h"
#include "graphics/graphics.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/shader.h"

namespace neko
{

struct SampleFrameData
{
    long long renderTime = 0;
    size_t frame = 0;
};

class PerfApp : public SystemInterface, public RenderCommandInterface
{
public:
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void Render() override;
private:
    void InitGraphics();
    void ReloadSample(size_t instance, size_t segment);

    static constexpr size_t minInstanceNmb = 10;
    static constexpr size_t maxInstanceNmb = 100'000;
    static constexpr size_t instanceStep = 10;
    static constexpr size_t framePerSample = 100;
    static constexpr size_t minSegmentNmb = 4;
    static constexpr size_t maxSegmentNmb = 2048;
    static constexpr size_t segmentStep = 8;

    size_t instanceNmb_ = 10;
    size_t segments_ = 64;
    size_t currentFrame = 0;

    gl::RenderSphere sphere_{Vec3f::zero, 1.0f};
    gl::Shader instanceShader_;

    sdl::Camera3D camera3d_;

    Job reloadSphere{ []() {} };
    std::vector<SampleFrameData> frameDatas_;

    std::vector<Vec3f> positions_;
    GLuint instanceVbo_ = 0;
    GLuint texture_ = 0;
};
}
