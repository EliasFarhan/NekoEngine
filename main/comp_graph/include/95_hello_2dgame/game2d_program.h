#pragma once

#include "comp_graph/sample_program.h"
#include "gl/sprite.h"
#include "gl/texture.h"
#include "engine/transform.h"

namespace neko
{
class Hello2dGameProgram : public SampleProgram
{
public:
    Hello2dGameProgram();
    void Init() override;
    void Update(seconds dt) override;
    void Destroy() override;
    void DrawImGui() override;
    void Render() override;
    void OnEvent(const SDL_Event &event) override;
private:
    EntityManager entityManager_;
    Transform2dManager transform2dManager_;
    gl::SpriteManager spriteManager_;
    gl::TextureManager textureManager_;
};
}
