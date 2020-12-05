#pragma once

#include <array>

namespace neko::voxel
{
const size_t chunkSize = 32;
const size_t regionHeight = 10;
const size_t regionSize = 64;

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
struct Cube
{
    enum CubeFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,

    };


    std::uint8_t flag = NONE;
    CubeType type = CubeType::NONE;
    /**
     * \brief Used to identify a cube position inside a chunk
     */
    CubeId cubeId = 0;
};

using ChunkId = std::uint16_t;
struct Chunk
{
    enum ChunkFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,
    };
    std::array<std::array<std::array<Cube, chunkSize>, chunkSize>, chunkSize> contents{};
    ChunkId chunkId = 0;
    std::uint8_t flag = NONE;
};

using RegionId = std::uint32_t;
struct Region
{
    std::array<std::array<std::array<Chunk, regionHeight>, regionSize>, regionSize> contents{};
    RegionId regionId = 0;
};
}