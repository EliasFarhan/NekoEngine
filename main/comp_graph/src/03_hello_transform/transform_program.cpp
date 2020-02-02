#include <gl/texture.h>
#include <engine/transform.h>
#include <engine/engine.h>
#include "03_hello_transform/transform_program.h"


#define ROTATE_OVER_TIME

namespace neko
{
void HelloTransformProgram::Init()
{

    const auto& config = BasicEngine::GetInstance()->config;
    shaderProgram.LoadFromFile(
            config.dataRootPath+"data/shaders/03_hello_coords/transform.vert",
            config.dataRootPath+"data/shaders/03_hello_coords/transform.frag"
    );
    const auto texturePath = config.dataRootPath+"data/sprites/wall.jpg";
    textureWall = gl::stbCreateTexture(texturePath.c_str());
    quad.Init();
}

void HelloTransformProgram::Render()
{


    shaderProgram.Bind();

    unsigned int transformLoc = glGetUniformLocation(shaderProgram.GetProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureWall);
    quad.Draw();

}

void HelloTransformProgram::Destroy()
{
    quad.Destroy();
    gl::DestroyTexture(textureWall);
}

void HelloTransformProgram::Update(seconds dt)
{
    transform = Mat4f::Identity;
    transform = Mat4f::Translate(transform, Vec4f(0.5f, -0.5f, 0.0f, 0.0f));
    transform = Mat4f::Rotate(transform, dt.count(), Vec3f(0.0f, 0.0f, 1.0f));

}

void HelloTransformProgram::DrawUi(seconds dt)
{

}

void HelloTransformProgram::OnEvent(const SDL_Event& event)
{

}

}