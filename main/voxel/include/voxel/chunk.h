#pragma once

#include <array>

namespace neko::voxel
{
const size_t chunkSize = 64;

struct Cube
{
    enum CubeFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,

    };
    enum class CubeType : std::uint8_t
    {
        NONE,
        GRASS,
        DESERT,
        SNOW,
        ROCK,
        BRICK,
        WOOD,
        DIRT,
        LENGTH,
    };

    std::uint8_t flag = NONE;
    CubeType type = CubeType::NONE;
};

struct Chunk
{
    enum ChunkFlag : std::uint8_t
    {
        NONE = 0u,
        IS_VISIBLE = 1u,

    };
    std::array<std::array<std::array<Cube, chunkSize>, chunkSize>, chunkSize> chunkContents{};
    std::uint8_t flag = NONE;
};
}