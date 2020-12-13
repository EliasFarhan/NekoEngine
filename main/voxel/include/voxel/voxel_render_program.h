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

#include "region.h"
#include "engine/system.h"
#include "engine/component.h"
#include "graphics/graphics.h"
#include "voxel/chunk.h"

#include "gl/gles3_include.h"
#include "gl/shader.h"
#include "graphics/camera.h"
#include "voxel/frustum.h"

namespace neko::voxel
{

struct CubeRenderData
{
    RegionId regionId = 0;
    ChunkId chunkId = 0;
    CubeId cubeId = 0;
    CubeTextureId textureId = 0;
};

class VoxelRenderProgram :
        public SystemInterface,
        public RenderCommandInterface,
        public SyncBuffersInterface,
        public DrawImGuiInterface
{
public:
    /**
     * \brief Init the render data, must be called from the render thread
     */
    void Init() override;

    void Update(seconds dt) override;
    /**
     * \brief Destroy the render data, must be called from the render thread
     */
    void Destroy() override;

    void Render() override;

    void AddChunk(const Chunk& chunk, RegionId regionId = 0);

    void AddCube(const Cube& cube, ChunkId chunkId = 0, RegionId regionId = 0);

    void SyncBuffers() override;

    void DrawImGui() override;

    void SetCurrentCamera(const Camera3D& camera);

private:
    Camera3D currentCamera3D_;
    Camera3D camera3D_;
    Frustum frustum_;
    std::vector<CubeRenderData> currentRenderData_;
    std::vector<CubeRenderData> renderData_;
    gl::Shader cubeShader_;
    gl::Shader skyboxShader_;
    GLuint tilesheetTexture_ = 0;
    GLuint skyboxTexture_ = 0;
    GLuint cubeVao_ = 0;
    GLuint instanceVbo_ = 0;
    size_t instanceChunkSize_ = 32768;
    static constexpr size_t cubeVerticesNmb = 36;

};
}