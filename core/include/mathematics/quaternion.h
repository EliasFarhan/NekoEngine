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
		axis *= Sin(rad);
		result.x = axis.x;
		result.y = axis.y;
		result.z = axis.z;
		result.w = Cos(rad);

		return Normalized(result);
	}


	//Returns the angle in degrees between two rotations a and b.
	static degree_t Angle(const Quaternion& a, const Quaternion& b)
	{
		
		return 2.0f * Acos(std::abs(Dot(a, b)));		//Todo change with neko::Acos
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
		const auto cy = Cos(angle.x * 0.5f);
		const auto sy = Sin(angle.x * 0.5f);
		const auto cp = Cos(angle.y * 0.5f);
		const auto sp = Sin(angle.y * 0.5f);
		const auto cr = Cos(angle.z * 0.5f);
		const auto sr = Sin(angle.z * 0.5f);

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
	
struct IntrinsicsQuaternion
{
	IntrinsicsQuaternion()
	{
		//fourVec4_ = FourVec4f::Zero;		//TODO
	}

	/*IntrinsicsQuaternion(float X, float Y, float Z, float W)
	{
		vec4_.x = X;
		vec4_.y = Y;
		vec4_.z = Z;
		vec4_.w = W;
	}*/

	IntrinsicsQuaternion(FourVec4f v)
	{
		fourVec4_ = v;
	}

	/*const float& operator[](size_t p_axis) const
	{
		return coord[p_axis];
	}

	float& operator[](size_t p_axis)
	{

		return coord[p_axis];
	}*/

	//The dot product between two rotations.
	static std::array<float, 4> Dot(IntrinsicsQuaternion a, IntrinsicsQuaternion b)
	{
		return	FourVec4f::DotIntrinsics(a.fourVec4_, b.fourVec4_);
	}

	//Converts this quaternion to one with the same orientation but with a magnitude of 1.
	static IntrinsicsQuaternion Normalized(IntrinsicsQuaternion quaternion)
	{
		//return quaternion / Magnitude(quaternion);
		//TODO
	}

	static std::array<float, 4> Magnitude(IntrinsicsQuaternion quaternion)
	{
		return quaternion.fourVec4_.MagnitudeIntrinsics();
	}

	static std::array<float, 4> SquareMagnitude(IntrinsicsQuaternion quaternion)
	{
		return quaternion.fourVec4_.SquareMagnitudeIntrinsics();
	}

	//Rotates the Quaternion of angle degrees around axis.
	static IntrinsicsQuaternion AngleAxis(radian_t rad, neko::Vec3f axis)
	{
		/*if (axis.SquareMagnitude() == 0.0f)
			return IntrinsicsQuaternion::Identity();

		IntrinsicsQuaternion result = IntrinsicsQuaternion::Identity();
		axis = axis.Normalized();
		axis *= Sin(rad);
		result.vec4_.x = axis.x;
		result.vec4_.y = axis.y;
		result.vec4_.z = axis.z;
		result.vec4_.w = Cos(rad);

		return Normalized(result);*/
		//TODO Intrinsics this
	}


	//Returns the angle in degrees between two rotations a and b.
	static degree_t Angle(const IntrinsicsQuaternion& a, const IntrinsicsQuaternion& b)
	{

		//return 2.0f * Acos(std::abs(Dot(a, b)));		//Todo change with neko::Acos
		//TODO Intrinsics this
	}

	IntrinsicsQuaternion Conjugate() const
	{
		//return IntrinsicsQuaternion(-vec4_.x, -vec4_.y, -vec4_.z, vec4_.w);
		//TODO Intrinsics this
	}

	//Returns the Inverse of rotation.
	IntrinsicsQuaternion Inverse() const
	{
		/*const IntrinsicsQuaternion conj = Conjugate();
		const float sMag = SquareMagnitude(*this);

		return conj / sMag;*/
		//TODO Intrinsics this
	}

	/*
	Returns a rotation that rotates z degrees around the z axis,
	x degrees around the x axis, and y degrees around the y axis;
	applied in that order
	*/
	static IntrinsicsQuaternion FromEuler(EulerAngles angle)
	{
		/*const auto cy = Cos(angle.x * 0.5f);
		const auto sy = Sin(angle.x * 0.5f);
		const auto cp = Cos(angle.y * 0.5f);
		const auto sp = Sin(angle.y * 0.5f);
		const auto cr = Cos(angle.z * 0.5f);
		const auto sr = Sin(angle.z * 0.5f);

		return IntrinsicsQuaternion(
			cy * cp * cr + sy * sp * sr,
			cy * cp * sr - sy * sp * cr,
			sy * cp * sr + cy * sp * cr,
			sy * cp * cr - cy * sp * sr
		);*/
		//TODO Intrinsics this
	}

	static IntrinsicsQuaternion Identity()
	{
		//return IntrinsicsQuaternion(0, 0, 0, 1);
		//TODO Intrinsics this
	}

	//Operators
	IntrinsicsQuaternion operator/(IntrinsicsQuaternion rhs) const
	{
		return *this * rhs.Inverse();
	}

	IntrinsicsQuaternion operator/(const float rhs) const {
		/*return IntrinsicsQuaternion(
			vec4_ / rhs);*/ //TODO
	}

	/*IntrinsicsQuaternion& operator+=(const float rhs)
	{
		vec4_ += rhs;
		return *this;
	}*/

	IntrinsicsQuaternion operator-(const IntrinsicsQuaternion& rhs) const
	{
		/*return IntrinsicsQuaternion(
			vec4_ - rhs.vec4_);*/ //TODO
	}
	IntrinsicsQuaternion& operator-=(const IntrinsicsQuaternion& rhs)
	{
		/*vec4_ -= rhs.vec4_;
		return *this;*/ //TODO
	}

	IntrinsicsQuaternion operator+(const IntrinsicsQuaternion& rhs) const
	{
		/*return IntrinsicsQuaternion(
			vec4_ - rhs.vec4_);*/ //TODO
	}

	IntrinsicsQuaternion& operator+=(const IntrinsicsQuaternion& rhs)
	{
		/*vec4_ += rhs.vec4_;
		return *this;*/ //TODO
	}


	IntrinsicsQuaternion operator*(const IntrinsicsQuaternion& rhs) const
	{
		//return IntrinsicsQuaternion(vec4_ * rhs.vec4_);
		//TODO
	}

	IntrinsicsQuaternion operator*(const float rhs) const {
		//return IntrinsicsQuaternion(vec4_ * rhs);
		//TODO
	}

	/*IntrinsicsQuaternion& operator*=(const IntrinsicsQuaternion& rhs)
	{
		vec4_ *= rhs.vec4_; 
		return *this;
	}*/

	bool operator==(const IntrinsicsQuaternion& right) const
	{
		//return vec4_ == right.vec4_;
		//TODO
	}

	bool operator!=(const IntrinsicsQuaternion& right) const
	{
		return !(*this == right);
	}

	/*friend std::ostream& operator<<(std::ostream& os, const IntrinsicsQuaternion& quat)
	{
		os << "Quaternion(" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << ")";
		return os;
	}*/
	
private:
	neko::FourVec4f fourVec4_;
};
}