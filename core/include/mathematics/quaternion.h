#include <engine/entity.h>
#include <mathematics/matrix.h>
#include "engine/component.h"
#include "mathematics/vector.h"
//TEST
namespace neko
{
	class Quaternion
	{
	public:
		/*
		The dot product between two rotations.
		*/
		float Dot(Quaternion a, Quaternion b)
		{
			return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
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

		/*
		Returns a rotation that rotates z degrees around the z axis,
		x degrees around the x axis, and y degrees around the y axis; 
		applied in that order
		*/
		Quaternion Euler(Quaternion quaternion, Vec3f angle) {

		}

	private:
		float x;
		float y;
		float z;
		float w;
	};
}