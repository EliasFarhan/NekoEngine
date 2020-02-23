#pragma once

#include "mathematics/angle.h"

namespace neko
{
float Sin(radian_t angle)
{
    return std::sin(angle.value());
}
float Cos(radian_t angle)
{
    return std::cos(angle.value());
}
float Tan(radian_t angle)
{
    return std::tan(angle.value());
}
radian_t Asin(float ratio)
{
    return radian_t(std::asin(ratio));
}
radian_t Acos(float ratio)
{
    return radian_t(std::acos(ratio));
}
radian_t Atan(float ratio)
{
    return radian_t(std::atan(ratio));
}

radian_t Atan2(float a, float b)
{
    return radian_t(std::atan2(a,b));
}
}