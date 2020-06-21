#pragma once

#include <cmath>

namespace neko
{
inline float Abs(float v)
{
	return std::abs(v);
}
inline bool Equal(float a, float b, float epsilon = 0.0000001f)
{
	return Abs(a - b) < epsilon;
}
	
inline float Lerp(float start, float end, float t)
{
	return start + (end - start) * t;
}

float RandomRange(float start, float end);
}
