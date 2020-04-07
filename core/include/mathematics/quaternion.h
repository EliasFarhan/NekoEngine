#pragma once
#include <engine/component.h>
#include <mathematics/vector.h>
#include "mathematics/trigo.h"
#include <mathematics/vector_nvec.h>


//DATE : 19.02.2020

namespace neko
{
struct Quaternion
{
	union
	{
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float coord[4];
	};

	Quaternion()
	{
		x = y = z = w = 0;
	}

	explicit Quaternion(float same)
		: x(same), y(same), z(same), w(same)
	{
	}

	Quaternion(float X, float Y, float Z, float W) noexcept
		: x(X), y(Y), z(Z), w(W)
	{
	}

	Quaternion(neko::Vec4f& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		w = v.w;
	}

	const float& operator[](size_t p_axis) const
	{
		return coord[p_axis];
	}

	float& operator[](size_t p_axis)
	{

		return coord[p_axis];
	}

	//The dot product between two rotations.
	static float Dot(Quaternion a, Quaternion b)
	{
		return	a.x * b.x +
				a.y * b.y +
				a.z * b.z + 
				a.w * b.w;
	}

	//Converts this quaternion to one with the same orientation but with a magnitude of 1.
	static Quaternion Normalized(Quaternion quaternion)
	{
		return quaternion / Magnitude(quaternion);
	}

	static float Magnitude(Quaternion quaternion)
	{
		return std::sqrt(quaternion.x * quaternion.x +
					quaternion.y * quaternion.y +
					quaternion.z * quaternion.z +
					quaternion.w * quaternion.w);
	}

	static float SquareMagnitude(Quaternion quaternion)
	{
		return (quaternion.x * quaternion.x +
			quaternion.y * quaternion.y +
			quaternion.z * quaternion.z +
			quaternion.w * quaternion.w);
	}

	//Rotates the Quaternion of angle degrees around axis.
	static Quaternion AngleAxis(radian_t rad, neko::Vec3f axis)
	{
		if (axis.SquareMagnitude() == 0.0f)
			return Quaternion::Identity();

		Quaternion result = Quaternion::Identity();
		axis = axis.Normalized();
		axis *= neko::Sin(rad);
		result.x = axis.x;
		result.y = axis.y;
		result.z = axis.z;
		result.w = neko::Cos(rad);

		return Normalized(result);
	}


	//Returns the angle in degrees between two rotations a and b.
	static degree_t Angle(const Quaternion& a, const Quaternion& b)
	{
		
		return 2.0f * neko::Acos(std::abs(Dot(a, b)));
	}

	Quaternion Conjugate() const
	{
		return Quaternion(-x, -y, -z, w);
	}

	//Returns the Inverse of rotation.
	Quaternion Inverse() const
	{
		const Quaternion conj = Conjugate();
		const float sMag = SquareMagnitude(*this);

		return conj / sMag;
	}

	/*
	Returns a rotation that rotates z degrees around the z axis,
	x degrees around the x axis, and y degrees around the y axis; 
	applied in that order
	*/
	static Quaternion FromEuler(EulerAngles angle)
	{
		const auto cy = neko::Cos(angle.x * 0.5f);
		const auto sy = neko::Sin(angle.x * 0.5f);
		const auto cp = neko::Cos(angle.y * 0.5f);
		const auto sp = neko::Sin(angle.y * 0.5f);
		const auto cr = neko::Cos(angle.z * 0.5f);
		const auto sr = neko::Sin(angle.z * 0.5f);

		return Quaternion(
			cy * cp * cr + sy * sp * sr,
			cy * cp * sr - sy * sp * cr,
			sy * cp * sr + cy * sp * cr,
			sy * cp * cr - cy * sp * sr
		);
	}

	static Quaternion Identity()
	{
		return Quaternion(0, 0, 0, 1);
	}
	
	//Operators
	Quaternion operator/(Quaternion rhs) const
	{
		return *this * rhs.Inverse();
	}

	Quaternion operator/(const float rhs) const {
		return Quaternion(
			x / rhs,
			y / rhs,
			z / rhs,
			w / rhs);
	}

	Quaternion& operator+=(const float rhs)
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}

	Quaternion operator-(const Quaternion& rhs) const
	{
		return Quaternion(
			x - rhs.x,
			y - rhs.y, 
			z - rhs.z, 
			w - rhs.w);
	}
	Quaternion& operator-=(const Quaternion& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}

	Quaternion operator+(const Quaternion& rhs) const
	{
		return Quaternion(
			x + rhs.x,
			y + rhs.y,
			z + rhs.z,
			w + rhs.w);
	}

	Quaternion& operator+=(const Quaternion& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}
	

	Quaternion operator*(const Quaternion& rhs) const
	{
		return Quaternion(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}

	Quaternion operator*(const float rhs) const {
		return Quaternion(
			x * rhs,
			y * rhs,
			z * rhs,
			w * rhs);
	}
	
	Quaternion& operator*=(const Quaternion& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}
	
	bool operator==(const Quaternion& right) const
	{
		return x == right.x && y == right.y && z == right.z && w == right.w;
	}
	
	bool operator!=(const Quaternion& right) const
	{
		return !(*this == right);
	}

    friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
    {
        os << "Quaternion(" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << ")";
        return os;
    }
};
	
#ifdef  __SSE__
	
struct FourQuaternion				//64 bytes
{

	explicit FourQuaternion(std::array<float, 4> qx, std::array<float, 4> qy, std::array<float, 4> qz, std::array<float, 4> qw)
	{
		x = qx;
		y = qy;
		z = qz;
		w = qw;
	}

	static inline std::array<float, 4> Dot(FourQuaternion q1, FourQuaternion q2)
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(q1.x.data());
		auto y1 = _mm_load_ps(q1.y.data());
		auto z1 = _mm_load_ps(q1.z.data());
		auto w1 = _mm_load_ps(q1.w.data());

		auto x2 = _mm_load_ps(q2.x.data());
		auto y2 = _mm_load_ps(q2.y.data());
		auto z2 = _mm_load_ps(q2.z.data());
		auto w2 = _mm_load_ps(q2.w.data());

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

	//Converts this quaternion to one with the same orientation but with a magnitude of 1.
	static FourQuaternion Normalized(FourQuaternion quaternion)
	{
		//TODO Test if it's working
		return quaternion / Magnitude(quaternion);
	}

	static inline std::array<float, 4> Magnitude(FourQuaternion quaternion)
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(quaternion.x.data());
		auto y1 = _mm_load_ps(quaternion.y.data());
		auto z1 = _mm_load_ps(quaternion.z.data());
		auto w1 = _mm_load_ps(quaternion.w.data());

		x1 = _mm_mul_ps(x1, x1);
		y1 = _mm_mul_ps(y1, y1);
		z1 = _mm_mul_ps(z1, z1);
		w1 = _mm_mul_ps(w1, w1);

		x1 = _mm_add_ps(x1, y1);
		z1 = _mm_add_ps(z1, w1);
		x1 = _mm_add_ps(x1, z1);

		x1 = _mm_sqrt_ps(x1);

		_mm_store_ps(result.data(), x1);
		return result;
		//TODO: Test if it is working
	}

	static inline std::array<float, 4> SquareMagnitude(FourQuaternion quaternion)
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(quaternion.x.data());
		auto y1 = _mm_load_ps(quaternion.y.data());
		auto z1 = _mm_load_ps(quaternion.z.data());
		auto w1 = _mm_load_ps(quaternion.w.data());
		
		x1 = _mm_mul_ps(x1, x1);
		y1 = _mm_mul_ps(y1, y1);
		z1 = _mm_mul_ps(z1, z1);
		w1 = _mm_mul_ps(w1, w1);

		x1 = _mm_add_ps(x1, y1);
		z1 = _mm_add_ps(z1, w1);
		x1 = _mm_add_ps(x1, z1);

		_mm_store_ps(result.data(), x1);
		return result;
		//TODO: Test if it is working
	}

	//Rotates the Quaternion of angle radians around axis.
	static FourQuaternion AngleAxis(std::array<radian_t, 4> rad, neko::FourVec3f axis)
	{
		FourQuaternion result = FourQuaternion::Identity();
		axis = axis.Normalized();
		result.x = axis.xs;
		result.y = axis.ys;
		result.z = axis.zs;

		std::array<float, 4> radf { rad[0].value(), rad[1].value(), rad[2].value(), rad[3].value() };
		
		auto x1 = _mm_load_ps(radf.data());

		x1 = _mm_cos_ps(x1);

		_mm_store_ps(result.w.data(), x1);

		return Normalized(result);
		//TODO: Test if working
	}
	
	static std::array<degree_t, 4> Angle(const FourQuaternion a, const FourQuaternion b)
	{
		std::array<float, 4> dot = Dot(a, b);
		dot = std::array<float, 4>{std::abs(dot[0]), std::abs(dot[1]), std::abs(dot[2]), std::abs(dot[3])};
		const std::array<float, 4> f{ 2.0f, 2.0f, 2.0f, 2.0f };
		alignas(4 * sizeof(float)) std::array<float, 4> resultf;

		auto x1 = _mm_load_ps(dot.data());
		auto x2 = _mm_load_ps(f.data());

		x1 = _mm_acos_ps(x1);

		x1 = _mm_mul_ps(x1, x2);
		_mm_store_ps(resultf.data(), x1);

		std::array<degree_t, 4> result{ static_cast<degree_t>(resultf[0]), static_cast<degree_t>(resultf[1]), static_cast<degree_t>(resultf[2]), static_cast<degree_t>(resultf[3])};
		return result;
		//TODO: Test if working
	}

	FourQuaternion Conjugate() const
	{
		return FourQuaternion(
			std::array<float, 4> {-x[0], -x[1], -x[2], -x[3]},
			std::array<float, 4> {-y[0], -y[1], -y[2], -y[3]},
			std::array<float, 4> {-z[0], -z[1], -z[2], -z[3]},
			w);
		//TODO Test if working
		//TODO Try to find a better optimisation
	}

	//Returns the Inverse of rotation.
	FourQuaternion Inverse() const
	{
		const FourQuaternion conj = Conjugate();
		const std::array<float, 4> sMag = SquareMagnitude(*this);

		return conj / sMag;
		//TODO Test if working
	}

	/*
	Returns a rotation that rotates z degrees around the z axis,
	x degrees around the x axis, and y degrees around the y axis;
	applied in that order
	*/
	static FourQuaternion FromEuler(std::array<EulerAngles, 4> angle)
	{
		std::array<float, 4> eulerAngleX { angle[0].x.value(), angle[1].x.value(), angle[2].x.value(), angle[3].x.value() };
		std::array<float, 4> eulerAngleY { angle[0].y.value(), angle[1].y.value(), angle[2].y.value(), angle[3].y.value() };
		std::array<float, 4> eulerAngleZ { angle[0].z.value(), angle[1].z.value(), angle[2].z.value(), angle[3].z.value() };
		std::array<float, 4> const0p5{ 0.5f, 0.5f, 0.5f, 0.5f };

		alignas(4 * sizeof(float)) std::array<float, 4> resultX;
		alignas(4 * sizeof(float)) std::array<float, 4> resultY;
		alignas(4 * sizeof(float)) std::array<float, 4> resultZ;
		alignas(4 * sizeof(float)) std::array<float, 4> resultW;

		auto x = _mm_load_ps(eulerAngleX.data());
		auto y = _mm_load_ps(eulerAngleY.data());
		auto z = _mm_load_ps(eulerAngleZ.data());
		auto w = _mm_load_ps(const0p5.data());

		auto cy = _mm_mul_ps(x, w);	//Todo: See if possible to use only one mul
		cy = _mm_cos_ps(cy);
		auto sy = _mm_mul_ps(x, w);
		sy = _mm_sin_ps(sy);
		
		auto cp = _mm_mul_ps(y, w);	//Todo: See if possible to use only one mul
		cp = _mm_cos_ps(cp);
		auto sp = _mm_mul_ps(y, w);
		sp = _mm_sin_ps(sp);
		
		auto cr = _mm_mul_ps(z, w);	//Todo: See if possible to use only one mul
		cr = _mm_cos_ps(cr);
		auto sr = _mm_mul_ps(z, w);
		sr = _mm_sin_ps(sr);

		x = _mm_mul_ps(cy, cp);
		x = _mm_mul_ps(x, cr);
		y = _mm_mul_ps(sy, sp);
		y = _mm_mul_ps(y, sr);
		x = _mm_add_ps(x, y);

		_mm_store_ps(resultX.data(), x);
		
		x = _mm_mul_ps(cy, cp);
		x = _mm_mul_ps(x, sr);
		y = _mm_mul_ps(sy, sp);
		y = _mm_mul_ps(y, cr);
		x = _mm_min_ps(x, y);

		_mm_store_ps(resultY.data(), x);

		x = _mm_mul_ps(sy, cp);
		x = _mm_mul_ps(x, sr);
		y = _mm_mul_ps(cy, sp);
		y = _mm_mul_ps(y, cr);
		x = _mm_add_ps(x, y);

		_mm_store_ps(resultZ.data(), x);

		x = _mm_mul_ps(sy, cp);
		x = _mm_mul_ps(x, cr);
		y = _mm_mul_ps(cy, sp);
		y = _mm_mul_ps(y, sr);
		x = _mm_min_ps(x, y);

		_mm_store_ps(resultW.data(), x);

		return FourQuaternion(resultX, resultY, resultZ, resultW);
		//TODO Test if working
	}

	static FourQuaternion Identity()
	{
		return FourQuaternion(
			std::array<float, 4> {0,0,0,0},
			std::array<float, 4> {0,0,0,0},
			std::array<float, 4> {0,0,0,0},
			std::array<float, 4> {1,1,1,1});
		//TODO Test if working
	}

	//Operators
	FourQuaternion operator/(FourQuaternion rhs) const
	{
		return *this * rhs.Inverse();
		//TODO Test if working
	}

	inline std::array<float, 4> DivideFourFloat(const std::array<float, 4> dividend, const std::array<float, 4> divisor) const
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(dividend.data());
		auto x2 = _mm_load_ps(divisor.data());

		x1 = _mm_div_ps(x1, x2);
		
		_mm_store_ps(result.data(), x1);
		return result;
	}

	inline std::array<float, 4> MultiplyFourFloat(const std::array<float, 4> dividend, const std::array<float, 4> divisor) const
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(dividend.data());
		auto x2 = _mm_load_ps(divisor.data());

		x1 = _mm_mul_ps(x1, x2);

		_mm_store_ps(result.data(), x1);
		return result;
	}

	inline std::array<float, 4> AddFourFloat(const std::array<float, 4> dividend, const std::array<float, 4> divisor) const
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(dividend.data());
		auto x2 = _mm_load_ps(divisor.data());

		x1 = _mm_add_ps(x1, x2);

		_mm_store_ps(result.data(), x1);
		return result;
	}

	inline std::array<float, 4> SubFourFloat(const std::array<float, 4> dividend, const std::array<float, 4> divisor) const
	{
		alignas(4 * sizeof(float)) std::array<float, 4> result;
		auto x1 = _mm_load_ps(dividend.data());
		auto x2 = _mm_load_ps(divisor.data());

		x1 = _mm_sub_ps(x1, x2);

		_mm_store_ps(result.data(), x1);
		return result;
	}
	
	inline FourQuaternion operator/(const std::array<float, 4> rhs) const {
		return FourQuaternion(
			DivideFourFloat(x, rhs),
			DivideFourFloat(y, rhs),
			DivideFourFloat(z, rhs),
			DivideFourFloat(w, rhs));
		
	}

	//TODO
	/*Quaternion& operator+=(const float rhs)	
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		w /= rhs;
		return *this;
	}*/

	FourQuaternion operator-(const FourQuaternion& rhs) const
	{
		return FourQuaternion(
			SubFourFloat(x, rhs.x),
			SubFourFloat(y, rhs.y),
			SubFourFloat(z, rhs.z),
			SubFourFloat(w, rhs.w));
	}

	//TODO
	/*Quaternion& operator-=(const Quaternion& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;
		return *this;
	}*/

	FourQuaternion operator+(const FourQuaternion& rhs) const
	{
		return FourQuaternion(
			AddFourFloat(x, rhs.x),
			AddFourFloat(y, rhs.y),
			AddFourFloat(z, rhs.z),
			AddFourFloat(w, rhs.w));
	}

	//TODO
	/*Quaternion& operator+=(const Quaternion& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return *this;
	}*/


	FourQuaternion operator*(const FourQuaternion& rhs) const
	{
		//TODO Polish Code
		return FourQuaternion(
			AddFourFloat(
				AddFourFloat(
					MultiplyFourFloat(w, rhs.x), MultiplyFourFloat(x, rhs.w)),
				AddFourFloat(
					MultiplyFourFloat(y, rhs.z), MultiplyFourFloat(z, rhs.y))),
			AddFourFloat(
				AddFourFloat(
					MultiplyFourFloat(w, rhs.y), MultiplyFourFloat(y, rhs.w)),
				AddFourFloat(
					MultiplyFourFloat(z, rhs.x), MultiplyFourFloat(x, rhs.z))),
			AddFourFloat(
				AddFourFloat(
					MultiplyFourFloat(w, rhs.z), MultiplyFourFloat(z, rhs.w)),
				AddFourFloat(
					MultiplyFourFloat(x, rhs.y), MultiplyFourFloat(y, rhs.x))),
			AddFourFloat(
				AddFourFloat(
					MultiplyFourFloat(w, rhs.w), MultiplyFourFloat(x, rhs.x)),
				AddFourFloat(
					MultiplyFourFloat(y, rhs.y), MultiplyFourFloat(z, rhs.z)))
		);
		
		/*return Quaternion(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);*/
	}

	FourQuaternion operator*(const std::array<float, 4> rhs) const {
		return FourQuaternion(
			MultiplyFourFloat(x, rhs),
			MultiplyFourFloat(y, rhs),
			MultiplyFourFloat(z, rhs),
			MultiplyFourFloat(w, rhs));
	}

	//TODO Finish the other functions
	//TODO
	/*Quaternion& operator*=(const Quaternion& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
		z *= rhs.z;
		w *= rhs.w;
		return *this;
	}*/

	/*bool operator==(const Quaternion& right) const
	{
		//TODO
		return x == right.x && y == right.y && z == right.z && w == right.w;
	}

	bool operator!=(const Quaternion& right) const
	{
		//TODO
		return !(*this == right);
	}*/

	/*friend std::ostream& operator<<(std::ostream& os, const Quaternion& quat)
	{
		//TODO
		os << "Quaternion(" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << ")";
		return os;
	}*/

	std::array<float, 4> x;		//16 bytes
	std::array<float, 4> y;		//16 bytes
	std::array<float, 4> z;		//16 bytes
	std::array<float, 4> w;		//16 bytes
};
#endif
}
