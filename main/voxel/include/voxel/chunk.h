#pragma once

#include <array>
#include <memory>
#include <vector>
#include <memory>

namespace neko::voxel
{
const size_t chunkSize = 32;


using CubeId = std::uint16_t;
enum class CubeType : std::uint8_t
{
    NONE = 0u,
    GRASS,
    DESERT,
    SNOW,
    ROCK,
    BRICK,
    WOOD,
    DIRT,
    LENGTH,
};
using CubeTextureId = std::uint32_t;

CubeTextureId GenerateTextureId(CubeType cubeType);
struct Cube
{
    enum CubeFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,

    };

    /**
     * \brief Used to identify a cube position inside a chunk
     */
    CubeId cubeId = 0;
    std::uint8_t flag = NONE;
    CubeTextureId cubeTextureId = 0u;

};
using ChunkContent = std::array<std::array<std::array<Cube, chunkSize>, chunkSize>, chunkSize>;
using ChunkVisibleArray = std::vector<Cube*>;
using ChunkId = std::uint16_t;
struct Chunk
{
    enum ChunkFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,
    };
    std::unique_ptr<ChunkContent> contents = nullptr;
    ChunkVisibleArray visibleCubes;
    ChunkId chunkId = 0;
    std::uint8_t flag = NONE;
};


}
