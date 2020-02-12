#include <engine/entity.h>
#include <mathematics/matrix.h>
#include "engine/component.h"
#include "mathematics/vector.h"
#include <cmath>

namespace neko
{
template<typename T>
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
		T coord[4];
	};

	Quaternion()
	{
		x = y = z = w = 0;
	}

	explicit Quaternion(T same)
		: x(same), y(same), z(same), w(same)
	{

	}

	Quaternion(T X, T Y, T Z, T W) noexcept
		: x(X), y(Y), z(Z), w(W)
	{
	}

	template<typename U>
	explicit
		Quaternion(const Quaternion<U>& quaternion): 
			x(static_cast<T>(quaternion.x)),
			y(static_cast<T>(quaternion.y)),
			z(static_cast<T>(quaternion.z)),
			w(static_cast<T>(quaternion.w))
	{
	}

	/*
	The dot product between two rotations.
	*/
	float Dot(Quaternion a, Quaternion b)
	{
		return	a.x * b.x +
				a.y * b.y +
				a.z * b.z + 
				a.w * b.w;
	}

	/*
	Returns the Inverse of rotation.
	*/
	Quaternion Inverse(Quaternion quaternion)
	{

	}

	/*
	Converts this quaternion to one with the same orientation but with a magnitude of 1.
	*/
	Quaternion Normalize(Quaternion quaternion)
	{
		return quaternion / Magnitude(quaternion);
	}

	float Magnitude(Quaternion quaternion) {
		return sqrtf(quaternion.x * quaternion.x +
					quaternion.y * quaternion.y +
					quaternion.z * quaternion.z +
					quaternion.w * quaternion.w);
	}

	/*
	Creates a rotation which rotates angle degrees around axis.
	*/
	Vec3f AngleAxis(Quaternion quaternion, float angle, Vec3f axis) {

	}

	/*
	Returns the angle in degrees between two rotations a and b.
	*/
	float Angle(Quaternion a, Quaternion b) {

	}

	Quaternion GetConjugate()
	{
		return Quaternion<T>(-quaternion.x, -quaternion.y, -quaternion.z, quaternion.w);)
	}


	Quaternion GetInverse()
	{
		const Quaternion<T> conj = GetConjugate();
		const float mag = Magnitude();

		return conj / (mag * mag);
	}
	/*
	Returns a rotation that rotates z degrees around the z axis,
	x degrees around the x axis, and y degrees around the y axis; 
	applied in that order
	*/
	Quaternion Euler(Vec3f vector3)
	{
		double cy = cos(vector3.x * 0.5);
		double sy = sin(vector3.x * 0.5);
		double cp = cos(vector3.y * 0.5);
		double sp = sin(vector3.y * 0.5);
		double cr = cos(vector3.z * 0.5);
		double sr = sin(vector3.z * 0.5);

		Quaternion<T> q;
		q.w = cy * cp * cr + sy * sp * sr;
		q.x = cy * cp * sr - sy * sp * cr;
		q.y = sy * cp * sr + cy * sp * cr;
		q.z = sy * cp * cr - cy * sp * sr;

		return q;
	}

	Quaternion<T> operator/(Quaternion rhs) const
	{
		return *this * rhs.GetInverse();

	}

	Quaternion<T> operator-(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(
			x - rhs.x,
			y - rhs.y, 
			z - rhs.z, 
			w - rhs.w);
	}

	Quaternion<T> operator+(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(
			x + rhs.x,
			y + rhs.y,
			z + rhs.z,
			w + rhs.w);
	}

	Quaternion<T> operator*(const Quaternion<T>& rhs) const
	{
		return Quaternion<T>(
			w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y,
			w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z,
			w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x,
			w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z);
	}
};
}