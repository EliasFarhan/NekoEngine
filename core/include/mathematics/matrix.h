#pragma once

#include "mathematics/vector.h"
#include "mathematics/angle.h"
#include "mathematics/quaternion.h"
#include "mathematics/intrinsincs.h"
#include "trigo.h"

#include <cassert>

namespace neko
{

class Mat3f
{
public :
    Mat3f()
    {
        data = std::array<float, 9>();
    }

    Vec3f operator*(const Vec3f& rhs) const
    {
        Vec3f v;
        v.x = data[0] * rhs.x + data[1] * rhs.y + data[2] * rhs.z;
        v.y = data[3] * rhs.x + data[4] * rhs.y + data[5] * rhs.z;
        v.z = data[6] * rhs.x + data[7] * rhs.y + data[8] * rhs.z;
        return v;
    }
    std::array<float, 9> data; //row vector
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

    explicit Mat4(const std::array<Vec4 < T>,4>& v)
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

    inline Mat4<T> Transpose() const
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

    Mat4<T> operator*(const Mat4<T>& rhs) const noexcept
    {
#ifdef EMSCRIPTEN
        return MultiplyNaive(rhs);
#else
        return MultiplyIntrinsincs(rhs);
#endif
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
                    v[column][row] += columns_[i][row] * rhs[column][i];
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

    inline Mat4<T> MultiplyAoSoA(const Mat4<T>& rhs) const noexcept;

    inline Mat4<T> MultiplyIntrinsincs(const Mat4<T>& rhs) const noexcept;

    static T MatrixDifference(const Mat4<T>& rhs, const Mat4<T>& lhs)
    {
        T result = 0;
        for (int column = 0; column < 4; column++)
        {
            for (int row = 0; row < 4; row++)
            {
                result += std::abs(rhs[column][row] - lhs[column][row]);
            }
        }
        return result;
    };

    static Mat4<float> Translate(const Mat4<float>& transform, const Vec3f pos);

    static Mat4<float> Scale(const Mat4<float>& transform, const Vec3f scale);

    static Mat4<float> Rotate(const Mat4<float>& transform, const degree_t angle, const Vec3f axis);

    static Mat4<float> Rotate(const Mat4<float>& transform, const radian_t angle, const Vec3f axis);

    static Mat4<float> Rotate(const Mat4<float>& transform, const Quaternion& quaternion);

    static Mat4<float> Rotate(const Mat4<float>& transform, const EulerAngles eulerAngles);

    static Mat4<float> const TranslationMatrixFrom(const Vec3f translation);

    static Mat4<float> const ScalingMatrixFrom(const Vec3f scale);

    static Mat4<float> const RotationMatrixFrom(const degree_t angle, const Vec3f axis);

    static Mat4<float> const RotationMatrixFrom(const radian_t angle, const Vec3f axis);

    static Mat4<float> const RotationMatrixFrom(const EulerAngles cardinalRotation);

    static Mat4<float> const RotationMatrixFrom(const RadianAngles cardinalRotation);

    static Mat4<float> const RotationMatrixFrom(const Quaternion& quaternion);

    static EulerAngles const Rotation(const Mat4<float>& transform);

    static float Pitch(const Mat4<float>& transform);

    static float RotationOnX(const Mat4<float>& transform);

    static float Yaw(const Mat4<float>& transform);

    static float RotationOnY(const Mat4<float>& transform);

    static float Roll(const Mat4<float>& transform);

    static float RotationOnZ(const Mat4<float>& transform);

    static Mat4<T> Perspective(radian_t fovy, float aspect, float near, float far);

    friend std::ostream& operator<<(std::ostream& os, const Mat4<T>& m)
    {
        for(int row=0; row < 4;row++)
        {
            os << "(";
            for(int col=0; col < 4;col++)
            {
                 os << m[col][row]<<' ';
            }
            os << ")";
        }
        return os;
    }


    const static Mat4<T> Identity;
    const static Mat4<T> Zero;
private:
    std::array<Vec4 < T>, 4>
    columns_; //row vector
};

//using Mat3f = Mat3<float>;
using Mat4f = Mat4<float>;

#ifdef __SSE__
template<>
inline Mat4f Mat4f::Transpose() const
{
    std::array<Vec4f, 4> v;
    auto xmm3 = _mm_load_ps(&columns_[1][0]);
    auto xmm4 = _mm_load_ps(&columns_[3][0]);
    auto xmm0 = _mm_load_ps(&columns_[0][0]);
    auto xmm1 = _mm_load_ps(&columns_[2][0]);

    auto xmm2 = _mm_shuffle_ps(xmm0, xmm3, 136);
    xmm0 = _mm_shuffle_ps(xmm0, xmm3, 221);
    xmm3 = _mm_shuffle_ps(xmm1, xmm4, 136);
    xmm1 = _mm_shuffle_ps(xmm1, xmm4, 221);
    xmm4 = _mm_shuffle_ps(xmm2, xmm3, 136);
    xmm2 = _mm_shuffle_ps(xmm2, xmm3, 221);
    _mm_store_ps(&v[0][0], xmm4);
    xmm4 = _mm_shuffle_ps(xmm0, xmm1, 136);
    _mm_store_ps(&v[2][0], xmm2);
    xmm0 = _mm_shuffle_ps(xmm0, xmm1, 221);
    _mm_store_ps(&v[1][0], xmm4);
    _mm_store_ps(&v[3][0], xmm0);
    return Mat4f(v);
}
#endif

template<typename T>
inline Mat4<T> Mat4<T>::MultiplyAoSoA(const Mat4<T>& rhs) const noexcept
{
    const auto lhsT(Transpose());
    std::array<Vec4f, 4> v;
    for (int column = 0; column < 4; column++)
    {
        for(int row = 0; row < 4; row++)
        {
            const auto result = Vec4f::Dot(lhsT[row], rhs.columns_[column]);
            v[column][row] = result;
        }

    }
    return Mat4f(v);
}

template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept;

#if defined(__SSE__) && !defined(__ANDROID__) 
template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    __m128 c1 = _mm_load_ps(&this->columns_[0][0]);
    __m128 c2 = _mm_load_ps(&this->columns_[1][0]);
    __m128 c3 = _mm_load_ps(&this->columns_[2][0]);
    __m128 c4 = _mm_load_ps(&this->columns_[3][0]);
    for (int column = 0; column < 4; column++)
    {
        __m128 x = _mm_load1_ps(&rhs.columns_[column][0]);
        __m128 y = _mm_load1_ps(&rhs.columns_[column][1]);
        __m128 z = _mm_load1_ps(&rhs.columns_[column][2]);
        __m128 w = _mm_load1_ps(&rhs.columns_[column][3]);
        x = _mm_mul_ps(c1, x);
        y = _mm_mul_ps(c2, y);
        z = _mm_mul_ps(c3, z);
        w = _mm_mul_ps(c4, w);

        x = _mm_add_ps(x,y);
        z = _mm_add_ps(z,w);
        x = _mm_add_ps(x,z);
        _mm_store_ps(&v[column][0], x);
    }
    return Mat4f(v);
}
#endif

#if false
template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    float32x4_t c1 = vld1q_f32(&this->columns_[0][0]);
    float32x4_t c2 = vld1q_f32(&this->columns_[1][0]);
    float32x4_t c3 = vld1q_f32(&this->columns_[2][0]);
    float32x4_t c4 = vld1q_f32(&this->columns_[3][0]);
    for (int column = 0; column < 4; column++)
    {
        const float tmpX = rhs.columns_[column][0];
        float32x4_t x = vdupq_n_f32(tmpX);
        const float tmpY = rhs.columns_[column][1];
        float32x4_t y = vdupq_n_f32(tmpY);
        const float tmpZ = rhs.columns_[column][2];
        float32x4_t z = vdupq_n_f32(tmpZ);
        const float tmpW = rhs.columns_[column][3];
        float32x4_t w = vdupq_n_f32(tmpW);
        
        x = vmulq_f32(c1,x);
        y = vmulq_f32(c2,y);
        z = vmulq_f32(c3,z);
        w = vmulq_f32(c4,w);

        x = vaddq_f32(x,y);
        z = vaddq_f32(z,w);
        x = vaddq_f32(x,z);
        vst1q_f32(&v[column][0], x);
    }
    return Mat4f(v);
}
#endif

#if defined(EMSCRIPTEN) || defined(__arm__) || defined(__ANDROID__)
template<>
inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
    std::array<Vec4f, 4> v;
    v4f c1 = *(v4f*)(&this->columns_[0][0]);
    v4f c2 = *(v4f*)(&this->columns_[1][0]);
    v4f c3 = *(v4f*)(&this->columns_[2][0]);
    v4f c4 = *(v4f*)(&this->columns_[3][0]);
    for (int column = 0; column < 4; column++)
    {
        float tmp = rhs.columns_[column][0];
        v4f rhsX = {tmp, tmp, tmp, tmp};
        tmp = rhs.columns_[column][1];
        v4f rhsY = {tmp, tmp, tmp, tmp};
        tmp = rhs.columns_[column][2];
        v4f rhsZ = {tmp, tmp, tmp, tmp};
        tmp = rhs.columns_[column][3];
        v4f rhsW = {tmp, tmp, tmp, tmp};
        v4f x = (c1*rhsX);
        v4f y = (c2*rhsY);
        v4f z = (c3*rhsZ);
        v4f w = (c4*rhsW);

        x = (x+y);
        z = (z+w);
        x = (x+z);
        *(v4f*)(&v[column][0])= x;
    }
    return Mat4f(v);
}
#endif

template<>
const inline Mat4f Mat4f::Identity = Mat4f(
        std::array<Vec4f, 4>
        {
                Vec4f(1, 0, 0, 0),
                Vec4f(0, 1, 0, 0),
                Vec4f(0, 0, 1, 0),
                Vec4f(0, 0, 0, 1)});
template <>
const inline Mat4f Mat4f::Zero = Mat4f(
        std::array<Vec4f, 4>
        {
            Vec4f::zero,
            Vec4f::zero,
            Vec4f::zero,
            Vec4f::zero
        });

template <>
inline Mat4f Mat4f::Perspective(radian_t fovy, float aspect, float near, float far)
{
    assert(fabsf(aspect - std::numeric_limits<float>::epsilon()) > 0.0f);

    const float tanHalfFovy = Tan(fovy / 2.0f);
    Mat4f perspective{Mat4f::Zero};

    perspective[0][0] = 1.0f/ (aspect * tanHalfFovy);
    perspective[1][1] = 1.0f / (tanHalfFovy);
    perspective[2][2] = - (far + near) / (far - near);
    perspective[2][3] = - 1.0f;
    perspective[3][2] = - (2.0f * far * near) / (far - near);
    return perspective;
}
}
