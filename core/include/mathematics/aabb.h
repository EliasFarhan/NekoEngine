#pragma once
/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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

#include "mathematics/vector.h"
#include "matrix.h"
#include "const.h"

namespace neko
{
	struct Obb2
	{
		/// Get the center of the OBB.
		Vec2f GetCenter() const
		{
			return 0.5f * (lowerLeftBound + upperRightBound);
		}

		/// Get the direction of the OBB rotated 90 degrees clockwise.
		Vec2f GetRight()
		{
			return Vec2f(-(direction.y - GetCenter().y) + GetCenter().x, direction.x - GetCenter().x + GetCenter().y);
		}

		/// Set the center, the extend and the direction of the OBB.
		void SetCenterExtendDir(Vec2f center, Vec2f extends, Vec2f dir) {
			lowerLeftBound = center - extends;
			upperRightBound = center + extends;
			direction = dir.Normalized();
		}


		bool IntersectObb(Obb2 obb)
		{
			Vec2f axis1 = direction;
			Vec2f axis2 = GetRight();
			Vec2f axis3 = obb.direction;
			Vec2f axis4 = obb.GetRight();

			/// TODO ExtendsComparaison

			return true;
		}

		float GetExtendOnAxis(Vec2f axis)
		{
			float extend;

			if (axis == direction)
			{
				extend = upperRightBound.y - lowerLeftBound.y;
			}

			if (axis == GetRight())
			{
				extend = upperRightBound.x - lowerLeftBound.x;
			}

			/// TODO General case

			return extend;
		}

		Vec2f GetOppositeBound(Vec2f bound, bool isUpper)
		{
			Vec2f centerToProjectionOnDir;
			Vec2f oppositeBound;
			Vec2f boundToOppositeBound;
			Vec2f centerToBound = bound - GetCenter();

			if (isUpper)
			{
				centerToProjectionOnDir = direction * (centerToBound).GetMagnitude() * cos(Vec2<float>::AngleBetween(centerToBound, direction));
				boundToOppositeBound = GetCenter() = centerToProjectionOnDir - bound;
				oppositeBound = upperRightBound + boundToOppositeBound + boundToOppositeBound;
			}
			else
			{
				centerToProjectionOnDir = direction * (centerToBound).GetMagnitude() * cos(Vec2<float>::AngleBetween(centerToBound, direction)) * -1;
				boundToOppositeBound = GetCenter() = centerToProjectionOnDir - bound;
				oppositeBound = upperRightBound + boundToOppositeBound + boundToOppositeBound;
			}

			return oppositeBound;
		}

		Vec2f lowerLeftBound;	///< the lower vertex
		Vec2f upperRightBound;	///< the upper vertex
		Vec2f direction;	///< the normal of the upper side
	};


	struct Aabb2d
	{
		///\brief Get the center of the AABB.
		Vec2f CalculateCenter() const
		{
			return 0.5f * (lowerBound + upperBound);
		}

		///\brief Get the extends of the AABB (half-widths).
		Vec2f CalculateExtends() const
		{
			return 0.5f * (upperBound - lowerBound);
		}

		///\brief Set the AABB from the center, extends.
		void FromCenterExtends(const Vec2f center, const Vec2f extend) {
			lowerBound = center - extend;
			upperBound = center + extend;
		}

		///\brief Enlarge AABB after a rotation
		void FromRotation(const float rotation) {
			const float newAngle = rotation / 180 * PI;
			std::array<Vec2f, 4> corners;
			corners[0] = lowerBound;
			corners[1] = Vec2f(lowerBound.x, upperBound.y);
			corners[2] = Vec2f(upperBound.x, lowerBound.y);
			corners[3] = upperBound;
			const float cosAngle = cosf(newAngle);
			const float sinAngle = sinf(newAngle);
			for (const Vec2f corner : corners)
			{
				const float x = corner.x * cosAngle - corner.y * sinAngle;
				const float y = corner.x * sinAngle + corner.y * cosAngle;
				if (x > upperBound.x)
				{
					upperBound.x = x;
				}
				if (x < lowerBound.x)
				{
					lowerBound.x = x;
				}
				if (y > upperBound.y)
				{
					upperBound.y = y;
				}
				if (y < lowerBound.y)
				{
					lowerBound.y = y;
				}
			}
		}

		///\brief Set the AABB from OBB.
		void FromObb(Obb2 obb) {
			FromCenterExtends(0.5f * (obb.upperRightBound - obb.lowerLeftBound), obb.GetCenter());
			FromRotation(Vec2f::AngleBetween<>(Vec2f(0, 1), obb.direction));
		}

		bool ContainsPoint(const Vec2f point) const
		{
			bool contains = point.x <= upperBound.x && point.x >= lowerBound.x;
			contains = point.y <= upperBound.y && point.y >= lowerBound.y && contains;
			return  contains;
		}

		bool ContainsAabb(const Aabb2d& aabb) const
		{
			return (ContainsPoint(aabb.upperBound) && ContainsPoint(aabb.lowerBound));
		}

		bool IntersectAabb(const Aabb2d& aabb) const
		{
			return (ContainsPoint(aabb.upperBound) || ContainsPoint(aabb.lowerBound));
		}

		Vec2f lowerBound = Vec2f::Zero;	// the lower vertex
		Vec2f upperBound = Vec2f::Zero;	// the upper vertex
	};

	struct Aabb3d
	{
		///\brief Get the center of the AABB.
		Vec3f CalculateCenter() const
		{
			return (lowerBound + upperBound) * 0.5f;
		}

		///\brief Get the extends of the AABB (half-widths).
		Vec3f CalculateExtends() const
		{
			return (upperBound - lowerBound) * 0.5f;
		}

		///\brief Set the AABB from the center, extends.
		void FromCenterExtends(const Vec3f center, const Vec3f extends, const Vec3f rotation = Vec3f::Zero) {
			lowerBound = center - extends;
			upperBound = center + extends;
		}

		///\brief Enlarge AABB after a rotation
		void FromRotation(const Vec3f rotation) {
			Vec3f newAngle = Vec3f(rotation.x / 180 * M_PI, rotation.y / 180 * M_PI, rotation.z / 180 * M_PI);
			std::array<Vec3f, 6> corners;
			corners[0] = lowerBound;
			corners[1] = upperBound;
			corners[2] = Vec3f(lowerBound.x, upperBound.y, upperBound.z);
			corners[3] = Vec3f(lowerBound.x, lowerBound.y, upperBound.z);
			corners[4] = Vec3f(upperBound.x, lowerBound.y, lowerBound.z);
			corners[5] = Vec3f(upperBound.x, upperBound.y, lowerBound.z);
			const float cosAngleX = cosf(newAngle.x);
			const float sinAngleX = sinf(newAngle.x);
			const float cosAngleY = cosf(newAngle.y);
			const float sinAngleY = sinf(newAngle.y);
			const float cosAngleZ = cosf(newAngle.z);
			const float sinAngleZ = sinf(newAngle.z);
			for (Vec3f corner : corners)
			{
				//Rotate around X
				float x = corner.x * cosAngleX - corner.y * sinAngleX;
				float y = corner.x * sinAngleX + corner.y * cosAngleX;
				float z = corner.z;
				corner = Vec3f(x, y, z);
				//Rotate around Y
				x = corner.x * cosAngleY + corner.z * sinAngleY;
				y = corner.y;
				z = -corner.x * sinAngleY + corner.z * cosAngleY;
				corner = Vec3f(x, y, z);
				//Rotate around Z
				x = corner.x;
				y = corner.y * cosAngleZ - corner.z * sinAngleZ;
				z = corner.y * sinAngleZ + corner.z * cosAngleZ;
				if (x > upperBound.x)
				{
					upperBound.x = x;
				}
				if (x < lowerBound.x)
				{
					lowerBound.x = x;
				}
				if (y > upperBound.y)
				{
					upperBound.y = y;
				}
				if (y < lowerBound.y)
				{
					lowerBound.y = y;
				}
				if (z > upperBound.z)
				{
					upperBound.z = z;
				}
				if (z < lowerBound.z)
				{
					lowerBound.z = z;
				}
			}
		}

		/////\brief Set the AABB from OBB.
		//void FromObb(Obb3 obb) {
		//	FromCenterExtends(0.5f * (obb.upperRightBound - obb.lowerLeftBound), obb.GetCenter());
		//	FromRotation(Vec3f::AngleBetween<>(Vec3f(0, 1), obb.direction));
		//}


		bool ContainsPoint(const Vec3f point) const
		{
			bool contains = point.x <= upperBound.x && point.x >= lowerBound.x;
			contains = point.y <= upperBound.y && point.y >= lowerBound.y && contains;
			contains = point.z <= upperBound.z && point.z >= lowerBound.z && contains;
			return  contains;
		}

		bool ContainsAabb(const Aabb3d& aabb) const
		{
			return (ContainsPoint(aabb.upperBound) && ContainsPoint(aabb.lowerBound));
		}

		bool IntersectAabb(const Aabb3d& aabb) const
		{
			return (ContainsPoint(aabb.upperBound) || ContainsPoint(aabb.lowerBound));
		}

		Vec3f lowerBound = Vec3f::Zero;	// the lower vertex
		Vec3f upperBound = Vec3f::Zero;	// the upper vertex
	};

}

