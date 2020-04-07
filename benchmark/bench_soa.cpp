/*
MIT License

Copyright (c) 2017 SAE Institute Switzerland AG

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
#include <vector>
#include <list>
#include <array>
#include <iostream>
#include <benchmark/benchmark.h>
#include <memory>
#include <cmath>
#include "mathematics/intrinsincs.h"

const int fromRange = 8;
const int toRange = 1 << 20;

#ifdef __AVX2__
#define SIMD_REGISTER_SIZE 8
#else
#ifdef __SSE__
#define SIMD_REGISTER_SIZE 4
#else
#define SIMD_REGISTER_SIZE 4
#endif
#endif

float floatRand()
{
	return float(rand()) / (float(RAND_MAX) + 1.0f);
}

namespace neko
{
struct Vec4f
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;
	float w = 0.0f;

	Vec4f() = default;
	Vec4f(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{

	}

	float GetMagnitude() const
	{
		return sqrtf(x * x + y * y + w * w + z * z);
	}

	Vec4f Normalized() const
	{
		return (*this) / (*this).GetMagnitude();
	}

	bool operator==(const Vec4f& rhs) const
	{
		return x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w;
	}

	bool operator!=(const Vec4f& rhs) const
	{
		return !(rhs == *this);
	}

	Vec4f operator+(const Vec4f& rhs) const
	{
		return Vec4f(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vec4f operator-(const Vec4f& rhs) const
	{
		return Vec4f(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	Vec4f operator*(float rhs) const
	{
		return Vec4f(x * rhs, y * rhs, z * rhs, w * rhs);
	}

	Vec4f operator/(float rhs) const
	{
		return (*this) * (1.0f / rhs);
	}

	Vec4f& operator+=(const Vec4f& rhs)
	{
		*this = *this + rhs;
		return *this;
	}
	Vec4f& operator*=(const float& rhs)
	{
		*this = *this * rhs;
		return *this;
	}

	Vec4f Lerp(const Vec4f& v1, const Vec4f& v2, float t)
	{
		return v1 + (v2 - v1) * t;
	}
	float Dot(const Vec4f& v1, const Vec4f& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}
	Vec4f& operator-=(const Vec4f& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		this->z -= rhs.z;
		this->w -= rhs.w;
		return *this;
	}
};
}

namespace AOS
{
struct Transform
{
    neko::Vec4f position;
};

class TransformSystem
{
public:
    TransformSystem(size_t length)
    {
        m_Transforms.resize(length);
        for (auto& transform : m_Transforms)
        {
            transform.position = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
        }
    }

    void Translate(const neko::Vec4f moveValue)
    {
        for (auto& transform : m_Transforms)
        {
            transform.position += moveValue;
        }
    }
private:
    std::vector <Transform> m_Transforms;
};
}


namespace SOA
{
class TransformSystem
{
public:
	TransformSystem(size_t length)
	{
		m_PositionsX.resize(length);
		m_PositionsY.resize(length);
		m_PositionsZ.resize(length);
		m_PositionsW.resize(length);
		for (size_t i = 0; i < length; i++)
		{
			m_PositionsX[i] = floatRand();
			m_PositionsY[i] = floatRand();
			m_PositionsZ[i] = floatRand();
			m_PositionsW[i] = floatRand();
		}
	}
	void Translate(const neko::Vec4f moveValue)
	{
		for (auto& posX : m_PositionsX)
		{
			posX += moveValue.x;
		}
		for (auto& posY : m_PositionsY)
		{
			posY += moveValue.y;
		}
		for (auto& posZ : m_PositionsZ)
		{
			posZ += moveValue.z;
		}
		for (auto& posW : m_PositionsW)
		{
			posW += moveValue.w;
		}
	}

private:
	std::vector<float> m_PositionsX;
	std::vector<float> m_PositionsY;
	std::vector<float> m_PositionsZ;
	std::vector<float> m_PositionsW;

    std::vector<float> m_ScalesX;
    std::vector<float> m_ScaleY;
    std::vector<float> m_ScaleZ;
    std::vector<float> m_ScaleW;

    std::vector<float> m_Angles;
};
}


namespace AOSOA
{
template<size_t N>
struct PackedVec4f
{

    std::array<float, N> positionsX;
    std::array<float, N> positionsY;
    std::array<float, N> positionsZ;
    std::array<float, N> positionsW;

};

template<size_t N>
class TransformSystem
{
public:
    TransformSystem(size_t length)
    {
        transforms_.resize(length / N);
        for (auto& transform : transforms_)
        {
            for (int i = 0; i < N; i++)
            {
                transform.positionsX[i] = floatRand();
                transform.positionsY[i] = floatRand();
                transform.positionsZ[i] = floatRand();
                transform.positionsW[i] = floatRand();
            }
        }
    }

    void Translate(const neko::Vec4f moveValue)
    {
        for (auto& transform : transforms_)
        {
            for (auto& position : transform.positionsX)
            {
                position += moveValue.x;
            }
            for (auto& position : transform.positionsY)
            {
                position += moveValue.y;
            }
            for (auto& position : transform.positionsZ)
            {
                position += moveValue.z;
            }
            for (auto& position : transform.positionsW)
            {
                position += moveValue.w;
            }
        }
    }

private:
    std::vector<PackedVec4f<N>> transforms_;
};
}
#ifndef EMSCRIPTEN
#ifdef __AVX2__
namespace AVX2
{
using float8 = __m256;
struct PackedVec4f
{
    float8 positionsX;
    float8 positionsY;
    float8 positionsZ;
    float8 positionsW;

};
class TransformSystem
{
public:
    TransformSystem(size_t length)
    {
        transforms_.resize(length / 8);
        for (auto& transform : transforms_)
        {
            std::array<float,8> values;

            std::generate(values.begin(), values.End(),floatRand);
            transform.positionsX = _mm256_load_ps(&values[0]);
            std::generate(values.begin(), values.End(),floatRand);
            transform.positionsY = _mm256_load_ps(&values[0]);
            std::generate(values.begin(), values.End(),floatRand);
            transform.positionsZ = _mm256_load_ps(&values[0]);
            std::generate(values.begin(), values.End(),floatRand);
            transform.positionsW = _mm256_load_ps(&values[0]);

        }
    }
    void Translate(const neko::Vec4f moveValue)
    {

        const float8 moveX = _mm256_broadcast_ss(&moveValue.x);
        const float8 moveY = _mm256_broadcast_ss(&moveValue.y);
        const float8 moveZ = _mm256_broadcast_ss(&moveValue.z);
        const float8 moveW = _mm256_broadcast_ss(&moveValue.w);

        for (auto& transform : transforms_)
        {
            transform.positionsX = _mm256_add_ps(transform.positionsX, moveX);
            transform.positionsY = _mm256_add_ps(transform.positionsY, moveY);
            transform.positionsZ = _mm256_add_ps(transform.positionsZ, moveZ);
            transform.positionsW = _mm256_add_ps(transform.positionsW, moveW);
        }
    }
private:
    std::vector<PackedVec4f> transforms_;
};
}
#endif
#ifdef __SSE__
namespace SSE
{
using float4 = __m128;
struct PackedVec4f
{
	float4 positionsX;
	float4 positionsY;
	float4 positionsZ;
	float4 positionsW;
};
class TransformSystem
{
public:
	TransformSystem(size_t length)
	{
		transforms_.resize(length / 4);
		for (auto& transform : transforms_)
		{
			std::array<float, 8> values;

			std::generate(values.begin(), values.End(), floatRand);
			transform.positionsX = _mm_load_ps(&values[0]);
			std::generate(values.begin(), values.End(), floatRand);
			transform.positionsY = _mm_load_ps(&values[0]);
			std::generate(values.begin(), values.End(), floatRand);
			transform.positionsZ = _mm_load_ps(&values[0]);
			std::generate(values.begin(), values.End(), floatRand);
			transform.positionsW = _mm_load_ps(&values[0]);

		}
	}
	void Translate(const neko::Vec4f moveValue)
	{
		const float4 moveX = _mm_load1_ps(&moveValue.x);
		const float4 moveY = _mm_load1_ps(&moveValue.y);
		const float4 moveZ = _mm_load1_ps(&moveValue.z);
		const float4 moveW = _mm_load1_ps(&moveValue.w);
		for (auto& transform : transforms_)
		{
			transform.positionsX = _mm_add_ps(transform.positionsX, moveX);
			transform.positionsY = _mm_add_ps(transform.positionsY, moveY);
			transform.positionsZ = _mm_add_ps(transform.positionsZ, moveZ);
			transform.positionsW = _mm_add_ps(transform.positionsW, moveW);
		}
	}
private:
	std::vector<PackedVec4f> transforms_;
};
}
#endif
#endif

static void BM_AOS(benchmark::State& state)
{
	auto transformSystem = std::make_unique<AOS::TransformSystem>(state.range(0));
	const auto move = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
	for (auto _ : state)
	{
		transformSystem->Translate(move);
	}
}
// Register the function as a benchmark
BENCHMARK(BM_AOS)->Range(fromRange, toRange);


static void BM_SOA(benchmark::State& state) {
	auto transformSystem = std::make_unique<SOA::TransformSystem>(state.range(0));
	const auto move = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
	for (auto _ : state)
	{
		transformSystem->Translate(move);
	}
}
BENCHMARK(BM_SOA)->Range(fromRange, toRange);

static void BM_AOSOA(benchmark::State& state) {
	auto transformSystem = std::make_unique<AOSOA::TransformSystem<SIMD_REGISTER_SIZE>>(state.range(0));
	const auto move = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
	for (auto _ : state)
	{
		transformSystem->Translate(move);
	}
}
BENCHMARK(BM_AOSOA)->Range(fromRange, toRange);
#ifndef EMSCRIPTEN
#ifdef __SSE__
static void BM_AOSOA_SSE(benchmark::State& state) {
    auto transformSystem = std::make_unique<SSE::TransformSystem>(state.range(0));
    const auto move = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
    for (auto _ : state)
    {
        transformSystem->Translate(move);
    }
}
BENCHMARK(BM_AOSOA_SSE)->Range(fromRange, toRange);
#endif
#ifdef __AVX2__
static void BM_AOSOA_AVX2(benchmark::State& state) {
    auto transformSystem = std::make_unique<AVX2::TransformSystem>(state.range(0));
    const auto move = neko::Vec4f(floatRand(), floatRand(), floatRand(), floatRand());
    for (auto _ : state)
    {
        transformSystem->Translate(move);
    }
}
BENCHMARK(BM_AOSOA_AVX2)->Range(fromRange, toRange);
#endif
#endif


BENCHMARK_MAIN();