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

#include "mathematics/vec3.h"

namespace neko
{
//-----------------------------------------------------------------------------
// NVec3 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, FourVec3f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());

    auto x2 = _mm_load_ps(v2.xs.data());
    auto y2 = _mm_load_ps(v2.ys.data());
    auto z2 = _mm_load_ps(v2.zs.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, Vec3f v2)
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());

    auto x2 = _mm_load_ps(&v2.x);
    auto y2 = _mm_load_ps(&v2.y);
    auto z2 = _mm_load_ps(&v2.z);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 4> FourVec3f::SquareMagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);

    x = _mm_add_ps(x, y);
    x = _mm_add_ps(x, z);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 4> FourVec3f::MagnitudeIntrinsics() const
{
    alignas(4 * sizeof(float))
    std::array<float, 4> result;
    auto x = _mm_load_ps(xs.data());
    auto y = _mm_load_ps(ys.data());
    auto z = _mm_load_ps(zs.data());

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);

    x = _mm_add_ps(x, y);
    x = _mm_add_ps(x, z);
    x = _mm_sqrt_ps(x);
    _mm_store_ps(result.data(), x);
    return result;
}
template<>
FourVec3f FourVec3f::NormalizedIntrinsics()
{
    FourVec3f result;
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
template<>
FourVec3f FourVec3f::ReflectIntrinsics(FourVec3f inVec, FourVec3f normal)
{
    FourVec3f result;
    normal = normal.NormalizedIntrinsics();

    auto xi = _mm_load_ps(inVec.xs.data());
    auto yi = _mm_load_ps(inVec.ys.data());
    auto zi = _mm_load_ps(inVec.zs.data());

    auto dot = _mm_load_ps(DotIntrinsics(inVec, normal).data());

    auto xn = _mm_load_ps(normal.xs.data());
    auto yn = _mm_load_ps(normal.ys.data());
    auto zn = _mm_load_ps(normal.zs.data());
    const float multiplier = 2.0f;
    const auto twos = _mm_load1_ps(&multiplier);

    xn = _mm_mul_ps(xn, twos);
    yn = _mm_mul_ps(yn, twos);
    zn = _mm_mul_ps(zn, twos);

    xn = _mm_mul_ps(xn, dot);
    yn = _mm_mul_ps(yn, dot);
    zn = _mm_mul_ps(zn, dot);

    xi = _mm_sub_ps(xi, xn);
    yi = _mm_sub_ps(yi, yn);
    zi = _mm_sub_ps(zi, zn);

    _mm_store_ps(result.xs.data(), xi);
    _mm_store_ps(result.ys.data(), yi);
    _mm_store_ps(result.zs.data(), zi);

    return result;
}
#endif

#ifdef __AVX2__
template <>
std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, EightVec3f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());

    auto x2 = _mm_load_ps(v2.xs.data());
    auto y2 = _mm_load_ps(v2.ys.data());
    auto z2 = _mm_load_ps(v2.zs.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template <>
std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, Vec3f v2)
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x1 = _mm_load_ps(v1.xs.data());
    auto y1 = _mm_load_ps(v1.ys.data());
    auto z1 = _mm_load_ps(v1.zs.data());

    auto x2 = _mm_load_ps(&v2.x);
    auto y2 = _mm_load_ps(&v2.y);
    auto z2 = _mm_load_ps(&v2.z);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    _mm_store_ps(result.data(), x1);
    return result;
}
template<>
std::array<float, 8> EightVec3f::SquareMagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());
    auto z = _mm256_load_ps(zs.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);

    x = _mm256_add_ps(x, y);
    x = _mm256_add_ps(x, z);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
std::array<float, 8> EightVec3f::MagnitudeIntrinsics() const
{
    alignas(8 * sizeof(float))
    std::array<float, 8> result;
    auto x = _mm256_load_ps(xs.data());
    auto y = _mm256_load_ps(ys.data());
    auto z = _mm256_load_ps(zs.data());

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);

    x = _mm256_add_ps(x, y);
    x = _mm256_add_ps(x, z);
    x = _mm256_sqrt_ps(x);
    _mm256_store_ps(result.data(), x);
    return result;
}
template<>
EightVec3f EightVec3f::NormalizedIntrinsics()
{
    EightVec3f result;
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
template<>
EightVec3f EightVec3f::ReflectIntrinsics(EightVec3f inVec, EightVec3f normal)
{
    EightVec3f result;
    normal = normal.NormalizedIntrinsics();

    auto xi = _mm_load_ps(inVec.xs.data());
    auto yi = _mm_load_ps(inVec.ys.data());
    auto zi = _mm_load_ps(inVec.zs.data());

    auto dot = _mm_load_ps(DotIntrinsics(inVec, normal).data());

    auto xn = _mm_load_ps(normal.xs.data());
    auto yn = _mm_load_ps(normal.ys.data());
    auto zn = _mm_load_ps(normal.zs.data());
    const float multiplier = 2.0f;
    const auto twos = _mm_load1_ps(&multiplier);
    xn = _mm_mul_ps(xn, twos);
    yn = _mm_mul_ps(yn, twos);
    zn = _mm_mul_ps(zn, twos);
    xn = _mm_mul_ps(xn, dot);
    yn = _mm_mul_ps(yn, dot);
    zn = _mm_mul_ps(zn, dot);

    xi = _mm_sub_ps(xi, xn);
    yi = _mm_sub_ps(yi, yn);
    zi = _mm_sub_ps(zi, zn);

    _mm_store_ps(result.xs.data(), xi);
    _mm_store_ps(result.ys.data(), yi);
    _mm_store_ps(result.zs.data(), zi);

    return result;
}
#endif
}