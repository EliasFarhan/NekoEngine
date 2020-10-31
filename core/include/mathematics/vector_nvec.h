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

#include <mathematics/vector.h>
#include "engine/intrinsincs.h"
namespace neko
{
//-----------------------------------------------------------------------------
// NVec2
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec2
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec2() noexcept
            : xs{}, ys{}
    {
    }

    explicit NVec2(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    explicit NVec2(const std::array<Vec2<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec2<T, N> operator+(const Vec2<T>& rhs) const
    {
        NVec2<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        return v;
    }

    NVec2<T, N>& operator+=(const Vec2<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        return *this;
    }

    NVec2<T, N> operator-(const Vec2<T>& rhs) const
    {
        NVec2<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        return v;
    }

    NVec2<T, N>& operator-=(const Vec2<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec2<T, N> v1, NVec2<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i];
        }
        return result;
    }

    std::array<T, N> SquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i];
        }
        return result;
    }

    std::array<T, N> Magnitude() const
    {
        std::array<T, N> result;
        std::array<T, N> sqrtMag = this->SquareMagnitude();
        for (int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(sqrtMag[i]);
        }
        return result;
    }

    NVec2<T, N> Normalized() const
    {
        NVec2<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
        }
        return result;
    }

    static NVec2<T, N> Lerp(const NVec2<T, N>& v1, const NVec2<T, N>& v2, T t)
    {
        NVec2<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
        }
        return result;
    }

    static NVec2<T, N> Reflect(const NVec2<T, N>& inVec, const NVec2<T, N>& normal)
    {
        NVec2<T, N> result;
        NVec2<T, N> normalized = normal.Normalized();
        std::array<T, N> dot = Dot(inVec, normal);
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = inVec.xs[i] - normalized.xs[i] * 2 * dot[i];
            result.ys[i] = inVec.ys[i] - normalized.ys[i] * 2 * dot[i];
        }
        return result;
    }

    NVec2<T, N> Rotate(std::array<T, N> angles) const
    {
        NVec2<T, N> result;
        for (int i = 0; i < N; i++)
        {
            angles[i] *= PI / 180.0f;
            result.xs[i] = xs[i] * cos(angles[i]) - xs[i] * sin(angles[i]);
            result.ys[i] = ys[i] * sin(angles[i]) - ys[i] * cos(angles[i]);
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec2<T, N> v1, NVec2<T, N> v2);
    static std::array<T, N> DotIntrinsics(NVec2<T, N> v1, Vec2<T> v2);
    std::array<T, N> SquareMagnitudeIntrinsics() const;
    std::array<T, N> MagnitudeIntrinsics() const;
    NVec2<T, N> NormalizedIntrinsics();
};

//-----------------------------------------------------------------------------
// NVec2 Aliases
//-----------------------------------------------------------------------------
using FourVec2f = NVec2<float, 4>;
using EightVec2f = NVec2<float, 8>;

//-----------------------------------------------------------------------------
// NVec2 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
inline std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, FourVec2f v2)
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
inline std::array<float, 4> FourVec2f::DotIntrinsics(FourVec2f v1, Vec2f v2)
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
inline std::array<float, 4> FourVec2f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 4> FourVec2f::MagnitudeIntrinsics() const
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
inline FourVec2f FourVec2f::NormalizedIntrinsics()
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
inline std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, EightVec2f v2)
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
inline std::array<float, 8> EightVec2f::DotIntrinsics(EightVec2f v1, Vec2f v2)
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
inline std::array<float, 8> EightVec2f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 8> EightVec2f::MagnitudeIntrinsics() const
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
inline EightVec2f EightVec2f::NormalizedIntrinsics()
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

//-----------------------------------------------------------------------------
// NVec3
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec3
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec3() noexcept
            : xs{}, ys{}, zs{}
    {
    }

    explicit NVec3(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = 0.0f;
        }
    }

    explicit NVec3(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    explicit NVec3(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    explicit NVec3(const std::array<Vec3<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec3<T, N> operator+(const Vec3<T>& rhs) const
    {
        NVec3<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z + rhs.z; });
        return v;
    }

    NVec3<T, N>& operator+=(const Vec3<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z + rhs.z; });
        return *this;
    }

    NVec3<T, N> operator-(const Vec3<T>& rhs) const
    {
        NVec3<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z - rhs.z; });
        return v;
    }

    NVec3<T, N>& operator-=(const Vec3<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z - rhs.z; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec3<T, N> v1, NVec3<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i] +
                        v1.zs[i] * v2.zs[i];
        }
        return result;
    }

    std::array<T, N> SquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i];
        }
        return result;
    }

    std::array<T, N> Magnitude() const
    {
        std::array<T, N> result;
        std::array<T, N> sqrtMag = this->SquareMagnitude();
        for (int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(sqrtMag[i]);
        }
        return result;
    }

    NVec3<T, N> Normalized() const
    {
        NVec3<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
            result.zs[i] = zs[i] / mag[i];
        }
        return result;
    }

    static NVec3<T, N> Lerp(const NVec3<T, N>& v1, const NVec3<T, N>& v2, T t)
    {
        NVec3<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
            result.zs[i] = v1.zs[i] + (v2.zs[i] - v1.zs[i]) * t;
        }
        return result;
    }

    static NVec3<T, N> Reflect(const NVec3<T, N>& inVec, const NVec3<T, N>& normal)
    {
        NVec3<T, N> result;
        NVec3<T, N> normalized = normal.Normalized();
        std::array<T, N> dot = Dot(inVec, normal);
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = inVec.xs[i] - normalized.xs[i] * 2 * dot[i];
            result.ys[i] = inVec.ys[i] - normalized.ys[i] * 2 * dot[i];
            result.zs[i] = inVec.zs[i] - normalized.zs[i] * 2 * dot[i];
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec3<T, N> v1, NVec3<T, N> v2);
    static std::array<T, N> DotIntrinsics(NVec3<T, N> v1, Vec3<T> v2);
    std::array<T, N> SquareMagnitudeIntrinsics() const;
    std::array<T, N> MagnitudeIntrinsics() const;
    NVec3<T, N> NormalizedIntrinsics();
    static NVec3<T, N> ReflectIntrinsics(NVec3<T, N> inVec, NVec3<T, N> normal);
};

//-----------------------------------------------------------------------------
// NVec3 Aliases
//-----------------------------------------------------------------------------
using FourVec3f = NVec3<float, 4>;
using EightVec3f = NVec3<float, 8>;

//-----------------------------------------------------------------------------
// NVec3 Implementations
//-----------------------------------------------------------------------------
#ifdef __SSE__
template <>
inline std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, FourVec3f v2)
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
inline std::array<float, 4> FourVec3f::DotIntrinsics(FourVec3f v1, Vec3f v2)
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
inline std::array<float, 4> FourVec3f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 4> FourVec3f::MagnitudeIntrinsics() const
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
inline FourVec3f FourVec3f::NormalizedIntrinsics()
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
inline FourVec3f FourVec3f::ReflectIntrinsics(FourVec3f inVec, FourVec3f normal)
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
inline std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, EightVec3f v2)
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
inline std::array<float, 8> EightVec3f::DotIntrinsics(EightVec3f v1, Vec3f v2)
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
inline std::array<float, 8> EightVec3f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 8> EightVec3f::MagnitudeIntrinsics() const
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
inline EightVec3f EightVec3f::NormalizedIntrinsics()
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
inline EightVec3f EightVec3f::ReflectIntrinsics(EightVec3f inVec, EightVec3f normal)
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

//-----------------------------------------------------------------------------
// NVec4
//-----------------------------------------------------------------------------
template<typename T, int N>
class alignas(N * sizeof(T)) NVec4
{
public:
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;
    std::array<T, N> ws;

    //-----------------------------------------------------------------------------
    // Constructors
    //-----------------------------------------------------------------------------
    NVec4() noexcept
            : xs{}, ys{}, zs{}, ws{}
    {
    }

    explicit NVec4(const Vec2<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = 0.0f;
            ws[i] = 0.0f;
        }
    }

    explicit NVec4(const Vec3<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = 0.0f;
        }
    }

    explicit NVec4(const Vec4<T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }

    explicit NVec4(const std::array<Vec4<T>, N> soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = soaV[i].w;
        }
    }

    //-----------------------------------------------------------------------------
    // Operators
    //-----------------------------------------------------------------------------
    NVec4<T, N> operator+(const Vec4<T>& rhs) const
    {
        NVec4<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z + rhs.z; });
        std::for_each(v.ws.begin(), v.ws.end(), [rhs](T w) { return w + rhs.w; });
        return v;
    }

    NVec4<T, N>& operator+=(const Vec4<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x + rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y + rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z + rhs.z; });
        std::for_each(ws.begin(), ws.end(), [rhs](T w) { return w + rhs.w; });
        return *this;
    }

    NVec4<T, N> operator-(const Vec4<T>& rhs) const
    {
        NVec4<T, N> v = this;
        std::for_each(v.xs.begin(), v.xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(v.ys.begin(), v.ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(v.zs.begin(), v.zs.end(), [rhs](T z) { return z - rhs.z; });
        std::for_each(v.ws.begin(), v.ws.end(), [rhs](T w) { return w - rhs.w; });
        return v;
    }

    NVec4<T, N>& operator-=(const Vec4<T>& rhs)
    {
        std::for_each(xs.begin(), xs.end(), [rhs](T x) { return x - rhs.x; });
        std::for_each(ys.begin(), ys.end(), [rhs](T y) { return y - rhs.y; });
        std::for_each(zs.begin(), zs.end(), [rhs](T z) { return z - rhs.z; });
        std::for_each(ws.begin(), ws.end(), [rhs](T w) { return w - rhs.w; });
        return *this;
    }

    //-----------------------------------------------------------------------------
    // Formulas
    //-----------------------------------------------------------------------------
    static std::array<T, N> Dot(NVec4<T, N> v1, NVec4<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i] +
                        v1.zs[i] * v2.zs[i] +
                        v1.ws[i] * v2.ws[i];
        }
        return result;
    }

    static std::array<T, N> Dot3(NVec4<T, N> v1, NVec4<T, N> v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] +
                        v1.ys[i] * v2.ys[i] +
                        v1.zs[i] * v2.zs[i];
        }
        return result;
    }

    std::array<T, N> SquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
        }
        return result;
    }

    std::array<T, N> Magnitude() const
    {
        std::array<T, N> result;
        std::array<T, N> sqrtMag = this->SquareMagnitude();
        for (int i = 0; i < N; i++)
        {
            result[i] = std::sqrt(sqrtMag[i]);
        }
        return result;
    }

    NVec4<T, N> Normalized() const
    {
        NVec4<T, N> result;
        std::array<T, N> mag = this->Magnitude();
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = xs[i] / mag[i];
            result.ys[i] = ys[i] / mag[i];
            result.zs[i] = zs[i] / mag[i];
            result.ws[i] = ws[i] / mag[i];
        }
        return result;
    }

    static NVec4<T, N> Lerp(NVec4<T, N> v1, NVec4<T, N> v2, T t)
    {
        NVec4<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result.xs[i] = v1.xs[i] + (v2.xs[i] - v1.xs[i]) * t;
            result.ys[i] = v1.ys[i] + (v2.ys[i] - v1.ys[i]) * t;
            result.zs[i] = v1.zs[i] + (v2.zs[i] - v1.zs[i]) * t;
            result.ws[i] = v1.ws[i] + (v2.ws[i] - v1.ws[i]) * t;
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(NVec4<T, N> v1, NVec4<T, N> v2);
    static std::array<T, N> DotIntrinsics(NVec4<T, N> v1, Vec4<T> v2);
    std::array<T, N> SquareMagnitudeIntrinsics() const;
    std::array<T, N> MagnitudeIntrinsics() const;
    NVec4<T, N> NormalizedIntrinsics();
};

//-----------------------------------------------------------------------------
// NVec4 Aliases
//-----------------------------------------------------------------------------
using FourVec4f = NVec4<float, 4>;
using EightVec4f = NVec4<float, 8>;

//-----------------------------------------------------------------------------
// NVec4 Implementations
//-----------------------------------------------------------------------------
#if defined(__arm__) || defined(__aarch64__) || defined(EMSCRIPTEN)
template<>
inline std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const
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
inline std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, FourVec4f v2)
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
inline std::array<float, 4> FourVec4f::DotIntrinsics(FourVec4f v1, Vec4f v2)
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
inline std::array<float, 4> FourVec4f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 4> FourVec4f::MagnitudeIntrinsics() const
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
inline FourVec4f FourVec4f::NormalizedIntrinsics()
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
inline std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, EightVec4f v2)
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
inline std::array<float, 8> EightVec4f::DotIntrinsics(EightVec4f v1, Vec4f v2)
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
inline std::array<float, 8> EightVec4f::SquareMagnitudeIntrinsics() const
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
inline std::array<float, 8> EightVec4f::MagnitudeIntrinsics() const
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
inline EightVec4f EightVec4f::NormalizedIntrinsics()
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
