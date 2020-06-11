#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/texture.h"
#include "sdl_engine/sdl_camera.h"

namespace neko
{
class HelloHdrProgram : public SampleProgram
{
    struct Light
    {
        Vec3f lightPos_;
        Color3 lightColor_;
    };
public:


    void Init() override;

    void Update(seconds dt) override;

    void Destroy() override;

    void DrawImGui() override;

    void Render() override;
    void OnEvent(const SDL_Event& event) override;
private:
    enum HdrFlags : std::uint8_t
    {
        NONE = 0u,
        ENABLE_HDR = 1u<<0u,
    	RESIZE_FRAMEBUFFER = 1u << 1u
    };

    void CreateFramebuffer();
	
    unsigned int hdrFbo_ = 0;
    unsigned int hdrColorBuffer_ = 0;
    unsigned int hdrRbo_ = 0;
	//Shader applying tone mapping
    gl::Shader hdrShader_;
    gl::RenderQuad hdrPlane_{Vec3f::zero, Vec2f::one*2.0f};

    std::array<Light, 4> lights_=std::array<Light,4>{{
            {Vec3f( 0.0f,  0.0f, 27.5f),Color3(200.0f, 200.0f, 200.0f)},
            {Vec3f(-1.4f, -1.9f, 9.0f), Color3(0.1f, 0.0f, 0.0f)},
            {Vec3f( 0.0f, -1.8f, 4.0f), Color3(0.0f, 0.0f, 0.2f)},
            {Vec3f( 0.8f, -1.7f, 6.0f), Color3(0.0f, 0.1f, 0.0f)}
    }};

    gl::RenderCuboid cube_{Vec3f::forward*10.0f, Vec3f(5.0f,5.0f,50.0f)};
    gl::Texture cubeTexture_;
    gl::Shader cubeShader_;

    sdl::Camera3D camera_;
    float exposure_ = 1.0f;
    std::uint8_t flags_ = NONE;

};
}