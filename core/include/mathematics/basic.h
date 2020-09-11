#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <cmath>
#include <random>

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
template<typename T>
float Clamp(T value, T lower, T upper)
{
    return value < lower ? lower : (value > upper ? upper : value);
}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type RandomRange(T start, T end)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<T> dis(start, end);
    return dis(gen);
}

template<typename T>
typename std::enable_if<std::is_floating_point<T>::value, T>::type RandomRange(T start, T end)
{
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<T> dis(start, end);
    return dis(gen);
}
}
