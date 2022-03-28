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

#include "mathematics/vec4.h"

namespace neko
{
//-----------------------------------------------------------------------------
// NVec4 Implementations
//-----------------------------------------------------------------------------
#if defined(__arm__) || defined(__aarch64__) || defined(EMSCRIPTEN)
template<>
std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    v4f x = *(v4f*)(xs.data());
    v4f y = *(v4f*)(ys.data());
    v4f z = *(v4f*)(zs.data());
    v4f w = *(v4f*)(ws.data());

    x = x*x;
    y = y*y;
    z = z*z;
    w = w*w;

    x = x+y;
    z = z+w;
    x = x+z;
    for(int i = 0; i < 4; i++)
    {
        x[i] = std::sqrt(x[i]);
    }
    *(v4f*)(result.data()) = x;
    return result;
}
#endif
#ifdef __SSE__
template <>
std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, FourVec4f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());
    auto w1 = _mm_load_ps(v1.ws.data());

    auto x2 = _mm_load_ps(v2.xs.data());
    auto y2 = _mm_load_ps(v2.ys.data());
    auto z2 = _mm_load_ps(v2.zs.data());
    auto w2 = _mm_load_ps(v2.ws.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);
    w1 = _mm_mul_ps(w1, w2);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, Vec4f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());
    auto w1 = _mm_load_ps(v1.ws.data());

    auto x2 = _mm_load1_ps(&v2.x);
    auto y2 = _mm_load1_ps(&v2.y);
    auto z2 = _mm_load1_ps(&v2.z);
    auto w2 = _mm_load1_ps(&v2.w);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);
    w1 = _mm_mul_ps(w1, w2);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 4> FourVec4f::SquareMagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());
    auto w = _mm_load_ps(ws.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);
    w = _mm_mul_ps(w, w);

    x = _mm_add_ps(x, y);
    z = _mm_add_ps(z, w);
    x = _mm_add_ps(x, z);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());
    auto w = _mm_load_ps(ws.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);
    w = _mm_mul_ps(w, w);

    x = _mm_add_ps(x, y);
    z = _mm_add_ps(z, w);
    x = _mm_add_ps(x, z);
    x = _mm_sqrt_ps(x);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
FourVec4f FourVec4f::NormalizedIntrinsics()
{
    FourVec4f result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());

    auto mag = _mm_load_ps(MagnitudeIntrinsics().data());

    x = _mm_div_ps(x, mag);
    y = _mm_div_ps(y, mag);
    z = _mm_div_ps(z, mag);

    _mm_store_ps(result.xs.data(), x);
    _mm_store_ps(result.ys.data(), y);
    _mm_store_ps(result.zs.data(), z);

    return result;
}
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, EightVec4f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm256_load_ps(v1.xs.data());
    auto y1 = _mm256_load_ps(v1.ys.data());
    auto z1 = _mm256_load_ps(v1.zs.data());
    auto w1 = _mm256_load_ps(v1.ws.data());

    auto x2 = _mm256_load_ps(v2.xs.data());
    auto y2 = _mm256_load_ps(v2.ys.data());
    auto z2 = _mm256_load_ps(v2.zs.data());
    auto w2 = _mm256_load_ps(v2.ws.data());

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);
    w1 = _mm256_mul_ps(w1, w2);

    x1 = _mm256_add_ps(x1, y1);
    z1 = _mm256_add_ps(z1, w1);
    x1 = _mm256_add_ps(x1, z1);
    _mm256_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, Vec4f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm256_load_ps(v1.xs.data());
    auto y1 = _mm256_load_ps(v1.ys.data());
    auto z1 = _mm256_load_ps(v1.zs.data());
    auto w1 = _mm256_load_ps(v1.ws.data());

    auto x2 = _mm256_broadcast_ss(&v2.x);
    auto y2 = _mm256_broadcast_ss(&v2.y);
    auto z2 = _mm256_broadcast_ss(&v2.z);
    auto w2 = _mm256_broadcast_ss(&v2.w);

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);
    w1 = _mm256_mul_ps(w1, w2);

    x1 = _mm256_add_ps(x1, y1);
    z1 = _mm256_add_ps(z1, w1);
    x1 = _mm256_add_ps(x1, z1);
    _mm256_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 8> EightVec4f::SquareMagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());
    auto z = _mm256_load_ps(zs.data());
    auto w = _mm256_load_ps(ws.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);
    w = _mm256_mul_ps(w, w);

    x = _mm256_add_ps(x, y);
    z = _mm256_add_ps(z, w);
    x = _mm256_add_ps(x, z);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 8> EightVec4f::MagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());
    auto z = _mm256_load_ps(zs.data());
    auto w = _mm256_load_ps(ws.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);
    w = _mm256_mul_ps(w, w);

    x = _mm256_add_ps(x, y);
    z = _mm256_add_ps(z, w);
    x = _mm256_add_ps(x, z);
    x = _mm256_sqrt_ps(x);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
EightVec4f EightVec4f::NormalizedIntrinsics()
{
    EightVec4f result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());

    auto mag = _mm_load_ps(MagnitudeIntrinsics().data());

    x = _mm_div_ps(x, mag);
    y = _mm_div_ps(y, mag);
    z = _mm_div_ps(z, mag);

    _mm_store_ps(result.xs.data(), x);
    _mm_store_ps(result.ys.data(), y);
    _mm_store_ps(result.zs.data(), z);

    return result;
}
#endif
}