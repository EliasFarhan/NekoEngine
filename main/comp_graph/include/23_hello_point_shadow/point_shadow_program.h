#pragma once

#include "comp_graph/sample_program.h"
#include "sdl_engine/sdl_camera.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"

namespace neko
{
class HelloPointShadowProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;

    void OnEvent(const SDL_Event& event) override;

private:
    struct Transform
    {
        Vec3f position = Vec3f::zero;
        Vec3f scale = Vec3f::one;
        Vec3f axis = Vec3f::up;
        radian_t angle = radian_t(0.0f);
    };

    void RenderScene(const gl::Shader& shader);

    sdl::Camera3D camera3D_;
    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};

    gl::Shader lightCubeShader_;
    Camera3D lightCamera_;
    Vec3f lightPos_ = Vec3f::zero;
    gl::Shader simpleDepthShader_;

    gl::Shader cubeShader_;
    gl::Texture cubeTexture_;

    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFbo_;
    unsigned int depthCubemap_;
    float dt_ = 0.0f;

    std::array<Transform, 5> transforms_ =
            {
                    {
                            {Vec3f(4.0f, -3.5f, 0.0f), Vec3f(0.5f)},
                            {Vec3f(2.0f, 3.0f, 1.0f), Vec3f(0.75f)},
                            {Vec3f(-3.0f, -1.0f, 0.0f), Vec3f(0.5f)},
                            {Vec3f(-1.5f, 1.0f, 1.5f), Vec3f(0.5f)},
                            {Vec3f(-1.5f, 2.0f, -3.0f), Vec3f(0.75f), Vec3f(1, 0, 1).Normalized(), degree_t(60.0f)}
                    }
            };
};
}