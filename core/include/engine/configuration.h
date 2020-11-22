#pragma once

#include <mathematics/vector.h>


namespace neko
{
/**
 * \brief store various Engine constant or global values
 */
struct Configuration
{
    enum Flags : std::uint8_t
    {
        FULLSCREEN = 1u << 0u,
        VSYNC = 1u << 1u
    };
    std::string windowName = "NekoEngine 0.1";
    Vec2u windowSize = Vec2u(1024, 1024);
    std::uint8_t flags = VSYNC;
    unsigned int framerateLimit = 0u;
    std::string dataRootPath = "data/";

};

}