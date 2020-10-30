#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include "comp_graph/sample_program.h"
#include "gl/shape.h"
#include "gl/shader.h"
#include "gl/texture.h"
#include "mathematics/vector.h"

namespace neko
{
class HelloCoordsProgram : public SampleProgram
{
public:
    void Init() override;

    void Update(seconds) override;

    void Render()override;

    void Destroy() override;

    void DrawImGui() override;

    void OnEvent(const SDL_Event& event) override;

private:
    gl::RenderCuboid cube_{Vec3f::zero, Vec3f::one};
    const static size_t cubeNumbers_ = 10;
    Vec3f cubePositions[cubeNumbers_] =
    {
        Vec3f(0.0f, 0.0f, 0.0f),
        Vec3f(2.0f, 5.0f, -15.0f),
        Vec3f(-1.5f, -2.2f, -2.5f),
        Vec3f(-3.8f, -2.0f, -12.3f),
        Vec3f(2.4f, -0.4f, -3.5f),
        Vec3f(-1.7f, 3.0f, -7.5f),
        Vec3f(1.3f, -2.0f, -2.5f),
        Vec3f(1.5f, 2.0f, -2.5f),
        Vec3f(1.5f, 0.2f, -1.5f),
        Vec3f(-1.3f, 1.0f, -1.5f)
    };
    gl::Shader shader_;
    gl::TextureManager textureManager_;
    TextureName  textureWall_ = INVALID_TEXTURE_NAME;
    TextureId  textureWallId_;

    Mat4f view{Mat4f::Identity};
    Mat4f projection{Mat4f::Identity};

    seconds timeSinceInit_{0};
};
}