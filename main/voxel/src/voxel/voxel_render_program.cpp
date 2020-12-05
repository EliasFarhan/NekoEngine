//
// Created by efarhan on 12/5/20.
//

#include <gl/texture.h>
#include "voxel/voxel_render_program.h"
#include "mathematics/vector.h"
#include "engine/engine.h"

#ifdef EASY_PROFILE_USE
#include <easy/profiler.h>
#endif

namespace neko::voxel
{

void VoxelRenderProgram::Init()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Voxel Init");
#endif
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const auto& config = BasicEngine::GetInstance()->GetConfig();
    cubeShader_.LoadFromFile(config.dataRootPath+"shaders/voxel/cube.vert",
                             config.dataRootPath+"shaders/voxel/cube.frag");
    const auto& filesystem = BasicEngine::GetInstance()->GetFilesystem();
    tilesheetTexture_ = gl::CreateTextureFromKTX(config.dataRootPath+"sprites/tilesheet.png.ktx", filesystem);
    glBindTexture(GL_TEXTURE_2D, tilesheetTexture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    struct Vertex
    {
        enum FaceType : std::uint8_t
        {
            NONE = 0,
            TOP = 1,
            BOTTOM = 2,
            SIDE = 3
        };
        Vec3f position;
        Vec2f texCoords;
        std::uint8_t faceType = NONE;
    };
    Vertex vertices[cubeVerticesNmb] =
            {
                    //Right face
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, -0.5f),   Vec2f(0.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(0.5f, -0.5f, 0.5f),   Vec2f(1.0f, 0.0f),Vertex::SIDE},
                    //Left face                 
                    {Vec3f(-0.5f, 0.5f, 0.5f),   Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, 0.5f, -0.5f),  Vec2f(0.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, -0.5f, 0.5f),  Vec2f(1.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, 0.5f, 0.5f),   Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    //Top face                  
                    {Vec3f(-0.5f, 0.5f, -0.5f),  Vec2f(0.0f, 1.0f),Vertex::TOP},
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 0.0f),Vertex::TOP},
                    {Vec3f(0.5f, 0.5f, -0.5f),   Vec2f(1.0f, 1.0f),Vertex::TOP},
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 0.0f),Vertex::TOP},
                    {Vec3f(-0.5f, 0.5f, -0.5f),  Vec2f(0.0f, 1.0f),Vertex::TOP},
                    {Vec3f(-0.5f, 0.5f, 0.5f),   Vec2f(0.0f, 0.0f),Vertex::TOP},
                    //Bottom fa                 
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 1.0f),Vertex::BOTTOM},
                    {Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(1.0f, 1.0f),Vertex::BOTTOM},
                    {Vec3f(0.5f, -0.5f, 0.5f),   Vec2f(1.0f, 0.0f),Vertex::BOTTOM},
                    {Vec3f(0.5f, -0.5f, 0.5f),   Vec2f(1.0f, 0.0f),Vertex::BOTTOM},
                    {Vec3f(-0.5f, -0.5f, 0.5f),  Vec2f(0.0f, 0.0f),Vertex::BOTTOM},
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 1.0f),Vertex::BOTTOM},
                    //Front fac                 
                    {Vec3f(-0.5f, -0.5f, 0.5f),  Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, -0.5f, 0.5f),   Vec2f(1.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, 0.5f),    Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, 0.5f, 0.5f),   Vec2f(0.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, -0.5f, 0.5f),  Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    //Back face
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, -0.5f),   Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(0.5f, -0.5f, -0.5f),  Vec2f(1.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(0.5f, 0.5f, -0.5f),   Vec2f(1.0f, 1.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, -0.5f, -0.5f), Vec2f(0.0f, 0.0f),Vertex::SIDE},
                    {Vec3f(-0.5f, 0.5f, -0.5f),  Vec2f(0.0f, 1.0f),Vertex::SIDE},
            };
    GLuint cubeVbo = 0;

    glGenVertexArrays(1, &cubeVao_);
    glGenBuffers(1, &cubeVbo);
    glBindVertexArray(cubeVao_);

    //Setup vertices
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    //texCoord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(1);
    //face type
    glVertexAttribIPointer(2, 1, GL_UNSIGNED_BYTE, sizeof(Vertex), (void*)offsetof(Vertex, faceType));
    glEnableVertexAttribArray(2);

    //Initialize the instanced vbo
    glGenBuffers(1, &instanceVbo_);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);
    // region id attribute
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 1, GL_UNSIGNED_INT, sizeof(CubeRenderData), nullptr);
    glVertexAttribDivisor(3, 1);
    // chunk id attribute
    glEnableVertexAttribArray(4);
    glVertexAttribIPointer(4, 1, GL_UNSIGNED_SHORT, sizeof(CubeRenderData),
                          (void*)offsetof(CubeRenderData, chunkId));
    glVertexAttribDivisor(4, 1);
    // cube id attribute
    glEnableVertexAttribArray(5);
    glVertexAttribIPointer(5, 1, GL_UNSIGNED_SHORT, sizeof(CubeRenderData),
                          (void*)offsetof(CubeRenderData, cubeId));
    glVertexAttribDivisor(5, 1);
    // texture id attribute
    glEnableVertexAttribArray(6);
    glVertexAttribIPointer(6, 1, GL_UNSIGNED_INT, sizeof(CubeRenderData),
                          (void*)offsetof(CubeRenderData, textureId));
    glVertexAttribDivisor(6, 1);
}

void VoxelRenderProgram::Update(seconds dt)
{

}

void VoxelRenderProgram::Destroy()
{
    glDeleteVertexArrays(1, &cubeVao_);
    cubeShader_.Destroy();
    glDeleteTextures(1, &tilesheetTexture_);

}

void VoxelRenderProgram::Render()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Voxel Render");
#endif
    cubeShader_.Bind();
    cubeShader_.SetTexture("tilesheet", tilesheetTexture_);
    // set view and proj matrix
    cubeShader_.SetMat4("view", camera3D_.GenerateViewMatrix());
    cubeShader_.SetMat4("proj", camera3D_.GenerateProjectionMatrix());
    for (size_t chunk = 0; chunk < renderData_.size() / instanceChunkSize_ + 1; chunk++)
    {
#ifdef EASY_PROFILE_USE
        EASY_BLOCK("Chunk Render");
#endif
        const size_t chunkBeginIndex = chunk * instanceChunkSize_ ;
        const size_t chunkEndIndex = std::min(renderData_.size() , (chunk + 1) * instanceChunkSize_ );
        if (chunkEndIndex > chunkBeginIndex)
        {
            const size_t tmpChunkSize = chunkEndIndex-chunkBeginIndex;
            glBindBuffer(GL_ARRAY_BUFFER, instanceVbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(CubeRenderData) * tmpChunkSize, &renderData_[chunkBeginIndex], GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glBindVertexArray(cubeVao_);
            glDrawArraysInstanced(GL_TRIANGLES, 0, cubeVerticesNmb, tmpChunkSize);
        }
    }
}

void VoxelRenderProgram::AddChunk(const Chunk& chunk, RegionId regionId)
{
    if(!(chunk.flag & Chunk::IS_VISIBLE))
    {
        return;   
    }
    if (chunk.contents == nullptr)
    {
        return;
    }
    //const auto& chunkContents = *chunk.contents;
    for(const auto* cube : chunk.visibleCubes)
    {
        currentRenderData_.push_back({ regionId, chunk.chunkId, cube->cubeId, cube->cubeTextureId });
    }
}

void VoxelRenderProgram::AddCube(const Cube& cube, ChunkId chunkId, RegionId regionId)
{
    currentRenderData_.push_back({regionId, chunkId, cube.cubeId, cube.cubeTextureId});
}

void VoxelRenderProgram::SyncBuffers()
{
#ifdef EASY_PROFILE_USE
    EASY_BLOCK("Voxel Sync Buffer");
#endif
    std::swap(renderData_, currentRenderData_);
    std::swap(camera3D_, currentCamera3D_);
    currentRenderData_.clear();
}

void VoxelRenderProgram::DrawImGui()
{

}

void VoxelRenderProgram::SetCurrentCamera(const Camera3D& camera)
{
    currentCamera3D_ = camera;
}


}