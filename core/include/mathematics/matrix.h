#pragma once

#include "mathematics/vector.h"

#include <cassert>

namespace neko
{
template<typename T>
class Mat3
{
private:
    std::array<Vec4 < T>, 4>
    columns_; //row vector
};

template<typename T>
class alignas(4 * sizeof(T)) Mat4
{
public:
    Mat4()
    {
        columns_ = Identity.columns_;
    }

    Mat4& operator=(Mat4 m)
    {
        columns_ = m.columns_;
        return *this;
    }

    Mat4(const Mat4& m) noexcept
    {
        columns_ = m.columns_;
    }

    explicit Mat4(const std::array<Vec4 < T>,

    4>& v)
    {
        columns_ = v;
    }

    const T& operator()(size_t row, size_t column) const
    {
        return columns_[column][row];
    }

    T& operator()(size_t row, size_t column)
    {
        return columns_[column][row];
    }

    const Vec4 <T>& operator[](size_t column) const
    {
        return columns_[column];
    }

    Vec4 <T>& operator[](size_t column)
    {
        return columns_[column];
    }

    Mat4<T> Transpose() const
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[row][column] = columns_[column][row];
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator+(const Mat4<T>& rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] + rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator-(const Mat4<T>& rhs) const
    {
        std::array<Vec4<T>, 4> v;
        for (int i = 0; i < 4; i++)
        {
            v[i] = columns_[i] - rhs.columns_[i];
        }
        return Mat4<T>(v);
    }

    Vec4 <T> operator*(const Vec4 <T>& rhs) const
    {
        Vec4<T> v;
        for (int column = 0; column < 4; column++)
        {
            v[column] = 0;
            for (int row = 0; row < 4; row++)
            {
                v[column] += columns_[column][row] * rhs[row];
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> operator*(const Mat4<T>& rhs) const
    {
        return MultiplyIntrinsincs(rhs);
    }

    inline Mat4<T> MultiplyNaive(const Mat4<T>& rhs) const noexcept
    {
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = 0;
                for (int i = 0; i < 4; i++)
                {
                    v[column][row] += columns_[i][column] * rhs[row][i];
                }
            }
        }
        return Mat4<T>(v);
    }

    inline Mat4<T> MultiplyTranpose(const Mat4<T>& rhs) const noexcept
    {
        const auto lhsT = this->Transpose();
        std::array<Vec4<T>, 4> v;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                v[column][row] = Vec4<T>::Dot(lhsT[row], Vec4<T>(rhs.columns_[column]));
            }
        }
        return Mat4<T>(v);
    }

    Mat4<T> MultiplyAoSoA(const Mat4<T>& rhs) const noexcept;

    Mat4<T> MultiplyIntrinsincs(const Mat4<T>& rhs) const noexcept;

    static T MatrixDifference(const Mat4<T>& rhs, const Mat4<T>& lhs)
    {
        T result = 0;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                result += rhs[column][row] - lhs[column][row];
            }
        }
        return result;
    };

    static Mat4<T> Translate(const Mat4<T>& transform, Vec3 <T> pos);

    static Mat4<T> Scale(const Mat4<T>& transform, Vec3 <T> scale);

    static Mat4<T> Rotate(const Mat4<T>& transform, T angle, Vec3 <T> axis);

    static Mat4<T> Rotate(const Mat4<T>& transform, Vec4 <T> quaternion);

    static Mat4<T> FromQuaternion(Vec4<T> quaternion);

    const static Mat4<T> Identity;
private:
    std::array<Vec4 < T>, 4>
    columns_; //row vector
};

template<typename T, int N>
struct alignas(N * sizeof(T)) NVec4
{
    std::array<T, N> xs;
    std::array<T, N> ys;
    std::array<T, N> zs;
    std::array<T, N> ws;

    NVec4()
            : xs{}, ys{}, zs{}, ws{}
    {
    };

    explicit NVec4(const Mat4<T>& m)
    {
        for (int row = 0; row < N; row++)
        {
            xs[row] = m[0][row];
            ys[row] = m[1][row];
            zs[row] = m[2][row];
            ws[row] = m[3][row];
        }
    }

    //Transpose the matrix
    explicit NVec4(const std::array<Vec4 < T>, N

    >& soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i][0];
            ys[i] = soaV[i][1];
            zs[i] = soaV[i][2];
            ws[i] = soaV[i][3];
        }
    }

    explicit NVec4(const Vec3 <T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i].x;
            ys[i] = soaV[i].y;
            zs[i] = soaV[i].z;
            ws[i] = 0.0f;
        }
    }

    explicit NVec4(const Vec4 <T>* soaV)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = soaV[i][0];
            ys[i] = soaV[i][1];
            zs[i] = soaV[i][2];
            ws[i] = soaV[i][3];
        }
    }

    explicit NVec4(const Vec4 <T>& v)
    {
        for (int i = 0; i < N; i++)
        {
            xs[i] = v.x;
            ys[i] = v.y;
            zs[i] = v.z;
            ws[i] = v.w;
        }
    }

    static std::array<T, N> Dot(const NVec4<T, N>& v1, const Vec4 <T>& v)
    {
        std::array<T, N> result{};
        for (int i = 0; i < N; i++)
        {
            result[0] += v1.xs[i] * v[i];
            result[1] += v1.ys[i] * v[i];
            result[2] += v1.zs[i] * v[i];
            result[3] += v1.ws[i] * v[i];
        }
        return result;
    }

    static std::array<T, N> Dot(const NVec4<T, N>& v1, const NVec4<T, N>& v2)
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = v1.xs[i] * v2.xs[i] + v1.ys[i] * v2.ys[i] + v1.zs[i] * v2.zs[i] + v1.ws[i] * v2.ws[i];
        }
        return result;
    }

    static std::array<T, N> DotIntrinsics(const NVec4<T, N>& v1, const NVec4<T, N>& v2);

    static std::array<T, N> DotIntrinsics(const NVec4<T, N>& v1, const Vec4 <T>& v);


    std::array<T, N> GetSquareMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
        }
        return result;
    }

    std::array<T, N> GetMagnitude() const
    {
        std::array<T, N> result;
        for (int i = 0; i < N; i++)
        {
            result[i] = xs[i] * xs[i] + ys[i] * ys[i] + zs[i] * zs[i] + ws[i] * ws[i];
            result[i] = std::sqrt(result[i]);
        }
        return result;
    }

    std::array<T, N> GetMagnitudeIntrinsincs() const;


};


using FourVec4f = NVec4<float, 4>;
using EightVec4f = NVec4<float, 8>;

#ifdef __SSE__

template<>
inline std::array<float, 4> FourVec4f::DotIntrinsics(const FourVec4f& v1, const FourVec4f& v2)
{
    __m128 x1 = _mm_load_ps(&v1.xs[0]);
    __m128 y1 = _mm_load_ps(&v1.ys[0]);
    __m128 z1 = _mm_load_ps(&v1.zs[0]);
    __m128 w1 = _mm_load_ps(&v1.ws[0]);

    __m128 x2 = _mm_load_ps(&v2.xs[0]);
    __m128 y2 = _mm_load_ps(&v2.ys[0]);
    __m128 z2 = _mm_load_ps(&v2.zs[0]);
    __m128 w2 = _mm_load_ps(&v2.ws[0]);

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);
    w1 = _mm_mul_ps(w1, w2);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);

    x1 = _mm_add_ps(x1, z1);

    std::array<float, 4> result;
    _mm_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 4> FourVec4f::DotIntrinsics(const FourVec4f& v1, const Vec4f& v2)
{
    __m128 x1 = _mm_load_ps(&v1.xs[0]);
    __m128 y1 = _mm_load_ps(&v1.ys[0]);
    __m128 z1 = _mm_load_ps(&v1.zs[0]);
    __m128 w1 = _mm_load_ps(&v1.ws[0]);

    __m128 v = _mm_load_ps(&v2[0]);

    x1 = _mm_mul_ps(x1, v);
    y1 = _mm_mul_ps(y1, v);
    z1 = _mm_mul_ps(z1, v);
    w1 = _mm_mul_ps(w1, v);

    x1 = _mm_add_ps(x1, y1);
    z1 = _mm_add_ps(z1, w1);

    x1 = _mm_add_ps(x1, z1);

    std::array<float, 4> result;
    _mm_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 4> FourVec4f::GetMagnitudeIntrinsincs() const
{
    __m128 x = _mm_load_ps(&xs[0]);
    __m128 y = _mm_load_ps(&ys[0]);
    __m128 z = _mm_load_ps(&zs[0]);
    __m128 w = _mm_load_ps(&ws[0]);

    x = _mm_mul_ps(x, x);
    y = _mm_mul_ps(y, y);
    z = _mm_mul_ps(z, z);
    w = _mm_mul_ps(w, w);

    x = _mm_add_ps(x, y);
    z = _mm_add_ps(z, w);

    x = _mm_add_ps(x, z);
    x = _mm_sqrt_ps(x);
    std::array<float, 4> result;
    _mm_store_ps(&result[0], x);
    return result;
}

#endif
#ifdef __AVX2__

template<>
inline std::array<float, 8> EightVec4f::DotIntrinsics(const EightVec4f& v1, const EightVec4f& v2)
{
    auto x1 = _mm256_load_ps(&v1.xs[0]);
    auto y1 = _mm256_load_ps(&v1.ys[0]);
    auto z1 = _mm256_load_ps(&v1.zs[0]);
    auto w1 = _mm256_load_ps(&v1.ws[0]);

    const auto x2 = _mm256_load_ps(&v2.xs[0]);
    const auto y2 = _mm256_load_ps(&v2.ys[0]);
    const auto z2 = _mm256_load_ps(&v2.zs[0]);
    const auto w2 = _mm256_load_ps(&v2.ws[0]);

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);
    w1 = _mm256_mul_ps(w1, w2);

    x1 = _mm256_add_ps(x1, y1);
    z1 = _mm256_add_ps(z1, w1);
    x1 = _mm256_add_ps(x1, z1);

    std::array<float, 8> result;
    _mm256_store_ps(&result[0], x1);
    return result;
}

template<>
inline std::array<float, 8> EightVec4f::GetMagnitudeIntrinsincs() const
{
    __m256 x = _mm256_load_ps(&xs[0]);
    __m256 y = _mm256_load_ps(&ys[0]);
    __m256 z = _mm256_load_ps(&zs[0]);
    __m256 w = _mm256_load_ps(&ws[0]);

    x = _mm256_mul_ps(x, x);
    y = _mm256_mul_ps(y, y);
    z = _mm256_mul_ps(z, z);
    w = _mm256_mul_ps(w, w);

    x = _mm256_add_ps(x, y);
    z = _mm256_add_ps(z, w);

    x = _mm256_add_ps(x, z);
    x = _mm256_sqrt_ps(x);
    std::array<float, 8> result;
    _mm256_storeu_ps(&result[0], x);
    return result;
}

#endif
using Mat3f = Mat3<float>;
using Mat4f = Mat4<float>;
using Transform3d = Mat4f;


template<>
Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept;


template<typename T>
inline Mat4<T> Mat4<T>::MultiplyAoSoA(const Mat4<T>& rhs) const noexcept
{
    const NVec4<T, 4> lhsM(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        v[column] = Vec4<T>(NVec4<T, 4>::Dot(lhsM, rhs.columns_[column]));
    }
    return Mat4<T>(v);
}

template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    const FourVec4f lhsM(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        v[column] = Vec4f(FourVec4f::DotIntrinsics(lhsM, rhs.columns_[column]));
    }
    return Mat4f(v);
}

//TODO Implement Matrix Translation
template<>
inline Transform3d Transform3d::Translate(const Transform3d& transform, Vec3f pos)
{
    (void) pos;
    return transform;
}

//TODO Implement Matrix Scale
template<>
inline Transform3d Transform3d::Scale(const Transform3d& transform, Vec3f scale)
{
    (void) scale;
    return transform;
}

//TODO Implement Matrix Rotation with angle and axis
template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, float angle, Vec3f axis)
{
    (void) angle;
    (void) axis;
    return transform;
}

//TODO Implement Matrix Rotation with Quaternion
template<>
inline Transform3d Transform3d::Rotate(const Transform3d& transform, Quaternion quaternion)
{
    (void) quaternion;
    return transform;
}

template<>
const inline Mat4f Mat4f::Identity = Mat4f(
        std::array<Vec4f, 4>{
                Vec4f(1, 0, 0, 0),
                Vec4f(0, 1, 0, 0),
                Vec4f(0, 0, 1, 0),
                Vec4f(0, 0, 0, 1)});

template<>
inline Transform3d Transform3d::FromQuaternion(Quaternion quaternion)
{
    (void) quaternion;
    return Transform3d::Identity;
}

}