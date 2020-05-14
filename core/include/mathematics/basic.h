#pragma once

#include <cmath>

namespace neko
{
inline float Abs(float v)
{
	return std::abs(v);
}
inline bool Equal(float a, float b, float epsilon = 0.00001f)
{
	return Abs(a - b) < epsilon;
}

float RandomRange(float start, float end);
}
