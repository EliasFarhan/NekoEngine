#pragma once

#include "mathematics/vec2.h"
#include "mathematics/vec3.h"

namespace neko
{

struct Ray2f
{
    Vec2f origin;
    Vec2f dir;
};

struct Ray3f
{
    Vec3f origin;
    Vec3f dir;
};

} // namespace neko
