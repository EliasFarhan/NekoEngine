#pragma once

#include "mathematics/vector.h"

#include <cassert>

namespace neko
{
    template<typename T>
    class alignas(4 * sizeof(T)) Mat4
    {
    public:
        explicit Mat4(const std::array<Vec4<T>, 4>& v)
        {
            values_ = v;
        }

        const T& operator()(size_t x, size_t y) const
        {
            return values_[x][y];
        }

        T& operator()(size_t x, size_t y)
        {
            return values_[x][y];
        }

        const Vec4<T>& operator[](size_t x) const
        {
            return values_[x];
        }

        Vec4<T>& operator[](size_t x)
        {
            return values_[x];
        }

        Mat4<T> Transpose() const
        {
            std::array<Vec4<T>, 4> v;
            for (int i = 0; i < 4; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    v[j][i] = values_[i][j];
                }
            }
            return Mat4<T>(v);
        }

        Mat4<T> operator+(const Mat4<T>& rhs) const
        {
            std::array<Vec4<T>, 4> v;
            for (int i = 0; i < 4; i++)
            {
                v[i] = values_[i] + rhs.values_[i];
            }
            return Mat4<T>(v);
        }

        Mat4<T> operator-(const Mat4<T>& rhs) const
        {
            std::array<Vec4<T>, 4> v;
            for (int i = 0; i < 4; i++)
            {
                v[i] = values_[i] - rhs.values_[i];
            }
            return Mat4<T>(v);
        }

        Vec4<T> operator*(const Vec4<T>& rhs) const
        {
            Vec4<T> v;
            for(int x = 0; x < 4; x++)
            {
                v[x] = 0;
                for(int i = 0; i < 4; i++)
                {
                    v[x] += values_[x][i]*rhs[i];
                }
            }
            return Mat4<T>(v);
        }

        inline Mat4<T> MultiplyNaive(const Mat4<T>& rhs) const noexcept
        {
            std::array<Vec4<T>, 4> v;
            for (int x = 0; x < 4; x++)
            {
                for (int y = 0; y < 4; y++)
                {
                    v[x][y] = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        v[x][y] += values_[x][i] * rhs[i][y];
                    }
                }
            }
            return Mat4<T>(v);
        }

        inline Mat4<T> MultiplyTranpose(const Mat4<T>& rhs) const noexcept
        {
            const auto rhsT = rhs.Transpose();
            std::array<Vec4<T>, 4> v;
            for (int x = 0; x < 4; x++)
            {
                for (int y = 0; y < 4; y++)
                {
                    v[x][y] = Vec4<T>::Dot(Vec4<T>(values_[x]), rhsT[y]);
                }
            }
            return Mat4<T>(v);
        }

        Mat4<T> MultiplyAoSoA(const Mat4<T>& rhs) const noexcept ;

        Mat4<T> MultiplyIntrinsincs(const Mat4<T>& rhs) const noexcept ;

        const static Mat4<T> Identity;
    private:
        std::array<Vec4<T>, 4> values_; //row vector
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
            for (int i = 0; i < N; i++)
            {
                xs[i] = m[0][i];
                ys[i] = m[1][i];
                zs[i] = m[2][i];
                ws[i] = m[3][i];
            }
        }

        explicit NVec4(const std::array<Vec4<T>, N>& soaV)
        {
            for (int i = 0; i < N; i++)
            {
                xs[i] = soaV[i][0];
                ys[i] = soaV[i][1];
                zs[i] = soaV[i][2];
                ws[i] = soaV[i][3];
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
                xs[i] = soaV[i][0];
                ys[i] = soaV[i][1];
                zs[i] = soaV[i][2];
                ws[i] = soaV[i][3];
            }
        }

        explicit NVec4(const Vec4<T>& v)
        {
            for (int i = 0; i < N; i++)
            {
                xs[i] = v.x;
                ys[i] = v.y;
                zs[i] = v.z;
                ws[i] = v.w;
            }
        }

        std::array<T,N> Multiply(const std::array<T,N>& v)
        {

        }
        static std::array<T, N> Dot(const NVec4<T, N>& v1, const Vec4<T>& v)
        {
            std::array<T, N> result;
            for (int i = 0; i < N; i++)
            {
                result[i] = v1.xs[i] * v.x + v1.ys[i] * v.y + v1.zs[i] * v.z + v1.ws[i] * v.z;
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
        static std::array<T, N> DotIntrinsics(const NVec4<T, N>& v1, const Vec4<T>& v);



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
    inline std::array<float, 8> EightVec4f::DotIntrinsics(const EightVec4f& v1, const EightVec4f& v2) {
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
    inline std::array<float, 8> EightVec4f::GetMagnitudeIntrinsincs() const {
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

    using Mat4f = Mat4<float>;

    template<>
    Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept ;


    template<typename T>
    inline Mat4<T> Mat4<T>::MultiplyAoSoA(const Mat4<T>& rhs) const noexcept
    {
        const FourVec4f rhsM(rhs.values_);
        std::array<Vec4f, 4> v;
        for (int x = 0; x < 4; x++)
        {
            //NVec4<T, 4> lhsM(values_[x]);

            v[x] = Vec4<T>(NVec4<T, 4>::Dot(rhsM, values_[x]));
        }
        return Mat4<T>(v);
    }
    template<>
    inline Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f &rhs) const noexcept
    {
        const FourVec4f rhsM(rhs.values_);
        std::array<Vec4f, 4> v;
        for(int x = 0; x < 4; x++)
        {
            //FourVec4f lhsM(values_[x]);

            v[x] = Vec4f(FourVec4f::DotIntrinsics(rhsM, values_[x]));
        }
        return Mat4f(v);
    }
}