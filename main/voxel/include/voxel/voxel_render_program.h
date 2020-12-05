#pragma once

#include "engine/system.h"
#include "engine/component.h"
#include "graphics/graphics.h"
#include "voxel/chunk.h"

#include "gl/gles3_include.h"
#include "gl/shader.h"
#include "graphics/camera.h"

namespace neko::voxel
{
using CubeTextureId = std::uint32_t;

CubeTextureId GenerateTextureId(CubeType cubeType);
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

    void AddCube(const Cube& cube, ChunkId chunkId = 0, RegionId regionId = 0);

    void SyncBuffers() override;

    void DrawImGui() override;

    void SetCurrentCamera(const Camera3D& camera);

private:
    gl::Shader cubeShader_;
    GLuint tilesheetTexture_ = 0;
    GLuint cubeVao_ = 0;
    GLuint instanceVbo_ = 0;
    size_t instanceChunkSize_ = 32768;
    static constexpr size_t cubeVerticesNmb = 36;
    std::vector<CubeRenderData> currentRenderData_;
    std::vector<CubeRenderData> renderData_;
    Camera3D currentCamera3D_;
    Camera3D camera3D_;
};
}