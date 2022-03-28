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

#include "mathematics/matrix4.h"

namespace neko
{

#ifdef __SSE__
template<>
Mat4f Mat4f::TransposeIntrinsics() const
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

#if defined(__SSE__) && !defined(__ANDROID__)
template<>
Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
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

        x = _mm_add_ps(x, y);
        z = _mm_add_ps(z, w);
        x = _mm_add_ps(x, z);
        _mm_store_ps(&v[column][0], x);
    }
    return Mat4f(v);
}
#endif

#if false
template<>
constexpr  Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
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

		x = vmulq_f32(c1, x);
		y = vmulq_f32(c2, y);
		z = vmulq_f32(c3, z);
		w = vmulq_f32(c4, w);

		x = vaddq_f32(x, y);
		z = vaddq_f32(z, w);
		x = vaddq_f32(x, z);
		vst1q_f32(&v[column][0], x);
}
	return Mat4f(v);
}
#endif

#if defined(EMSCRIPTEN) || defined(__arm__) || defined(__ANDROID__) || defined(__aarch64__)
template<>
constexpr Mat4f Mat4f::MultiplyIntrinsincs(const Mat4f& rhs) const noexcept
{
	std::array<Vec4f, 4> v;
	v4f c1 = *(v4f*)(&this->columns_[0][0]);
	v4f c2 = *(v4f*)(&this->columns_[1][0]);
	v4f c3 = *(v4f*)(&this->columns_[2][0]);
	v4f c4 = *(v4f*)(&this->columns_[3][0]);
	for (int column = 0; column < 4; column++)
	{
		float tmp = rhs.columns_[column][0];
		v4f rhsX = { tmp, tmp, tmp, tmp };
		tmp = rhs.columns_[column][1];
		v4f rhsY = { tmp, tmp, tmp, tmp };
		tmp = rhs.columns_[column][2];
		v4f rhsZ = { tmp, tmp, tmp, tmp };
		tmp = rhs.columns_[column][3];
		v4f rhsW = { tmp, tmp, tmp, tmp };
		v4f x = (c1 * rhsX);
		v4f y = (c2 * rhsY);
		v4f z = (c3 * rhsZ);
		v4f w = (c4 * rhsW);

		x = (x + y);
		z = (z + w);
		x = (x + z);
		*(v4f*)(&v[column][0]) = x;
}
	return Mat4f(v);
}
#endif

}