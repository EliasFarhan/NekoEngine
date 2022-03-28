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

#include "mathematics/vec2.h"

namespace neko
{

//-----------------------------------------------------------------------------
// NVec2 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, FourVec2f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());

    auto x2 = _mm_load_ps(v2.xs.data());
    auto y2 = _mm_load_ps(v2.ys.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);

    x1 = _mm_add_ps(x1, y1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, Vec2f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());

    auto x2 = _mm_load_ps(&v2.x);
    auto y2 = _mm_load_ps(&v2.y);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);

    x1 = _mm_add_ps(x1, y1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 4> FourVec2f::SquareMagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);

    x = _mm_add_ps(x, y);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 4> FourVec2f::MagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);

    x = _mm_add_ps(x, y);
    x = _mm_sqrt_ps(x);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
FourVec2f FourVec2f::NormalizedIntrinsics()
{
    FourVec2f result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());

    auto mag = _mm_load_ps(MagnitudeIntrinsics().data());

    x = _mm_div_ps(x, mag);
    y = _mm_div_ps(y, mag);

    _mm_store_ps(result.xs.data(), x);
    _mm_store_ps(result.ys.data(), y);

    return result;
}
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, EightVec2f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());

    auto x2 = _mm_load_ps(v2.xs.data());
    auto y2 = _mm_load_ps(v2.ys.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);

    x1 = _mm_add_ps(x1, y1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, Vec2f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());

    auto x2 = _mm_load_ps(&v2.x);
    auto y2 = _mm_load_ps(&v2.y);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);

    x1 = _mm_add_ps(x1, y1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 8> EightVec2f::SquareMagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);

    x = _mm256_add_ps(x, y);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 8> EightVec2f::MagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);

    x = _mm256_add_ps(x, y);
    x = _mm256_sqrt_ps(x);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
EightVec2f EightVec2f::NormalizedIntrinsics()
{
    EightVec2f result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto mag = _mm_load_ps(MagnitudeIntrinsics().data());

    x = _mm_div_ps(x, mag);
    y = _mm_div_ps(y, mag);

    _mm_store_ps(result.xs.data(), x);
    _mm_store_ps(result.ys.data(), y);

    return result;
}
#endif
}