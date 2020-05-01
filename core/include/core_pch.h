#pragma once

#include <vector>
#include <map>
#include <array>
#include <thread>
#include <atomic>
#include <condition_variable>

#ifdef WIN32
#define _USE_MATH_DEFINES
#include <corecrt_math_defines.h>
#endif

#include <cmath>
#include <imgui.h>