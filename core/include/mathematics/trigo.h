#pragma once

#include <cmath>
#include "mathematics/angle.h"

namespace neko
{
inline float Sin(radian_t angle)
{
    return std::sin(angle.value());
}
inline float Cos(radian_t angle)
{
    return std::cos(angle.value());
}
inline float Tan(radian_t angle)
{
    return std::tan(angle.value());
}
inline radian_t Asin(float ratio)
{
    return radian_t(std::asin(ratio));
}
inline radian_t Acos(float ratio)
{
    return radian_t(std::acos(ratio));
}
inline radian_t Atan(float ratio)
{
    return radian_t(std::atan(ratio));
}

inline radian_t Atan2(float a, float b)
{
    return radian_t(std::atan2(a,b));
}
}