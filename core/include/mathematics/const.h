#pragma once

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif

#include <cmath>
namespace neko
{
const float PI = static_cast<float>(M_PI);


}