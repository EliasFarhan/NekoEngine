#pragma once

#include "comp_graph/sample_program.h"
#include "gl/shader.h"
#include "gl/shape.h"
#include "gl/model.h"
#include "sdl_engine/sdl_camera.h"


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
private:
    struct DirectionalLight
    {
        Vec3f lightPos_ = Vec3f(1,0,1)*10.0f;
        Vec3f lightDir_ = Vec3f(-1,0,-1).Normalized();
    };
    void RenderScene(const gl::Shader& shader);
	
    gl::RenderQuad floor_{Vec3f::zero, Vec2f::one*5.0f};
    gl::Texture floorTexture_;

    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
	
    assimp::Model model_;

    gl::Shader simpleDepthShader_;
    gl::Shader modelShader_;

    sdl::Camera3D camera_;
    Camera2D depthCamera_;

    unsigned int depthMapFbo_ = 0;
    unsigned int depthMap_ = 0;
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    DirectionalLight light_;
};
}