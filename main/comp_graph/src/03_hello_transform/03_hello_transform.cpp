#include <gl/texture.h>
#include <engine/transform.h>
#include "03_hello_transform/transform_program.h"


#define ROTATE_OVER_TIME

namespace neko
{
void HelloTransformDrawingProgram::Init()
{


    shaderProgram.LoadFromFile(
            "shaders/03_hello_transform/transform.vert",
            "shaders/03_hello_transform/transform.frag"
    );

    textureWall = gl::stbCreateTexture("data/sprites/wall.jpg");
    quad.Init();
}

void HelloTransformDrawingProgram::Render()
{


    shaderProgram.Bind();

    unsigned int transformLoc = glGetUniformLocation(shaderProgram.GetProgram(), "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

    glBindTexture(GL_TEXTURE_2D, textureWall);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void HelloTransformDrawingProgram::Destroy()
{
quad.Destroy();
}

void HelloTransformDrawingProgram::Update(seconds dt)
{
    transform = Mat4f::Identity;
    transform = Mat4f::Translate(transform, Vec4f(0.5f, -0.5f, 0.0f, 0.0f));
    transform = Mat4f::Rotate(transform, dt.count(), Vec3f(0.0f, 0.0f, 1.0f));

}

}