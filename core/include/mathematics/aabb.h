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

namespace neko
{
    struct Aabb2
	{
		/// Get the center of the AABB.
		Vec2f GetCenter() const
		{
			return 0.5f * (lowerBound + upperBound);
		}

		/// Get the extents of the AABB (half-widths).
		Vec2f GetExtents() const
		{
			return 0.5f * (upperBound - lowerBound);
		}

		/// Get the center of the AABB.
		void SetCenterExtend(Vec2f center, Vec2f extends) {
			lowerBound = center - extends;
			upperBound = center + extends;
		}

		bool ContainsPoint(Vec2f point)
		{
			return  (point <= upperBound && point >= lowerBound);
		}

		bool ContainsAabb(Aabb2 aabb)
		{
			return (ContainsPoint(aabb.upperBound) && ContainsPoint(aabb.lowerBound));
		}

		bool IntersectAabb(Aabb2 aabb)
		{
			return (ContainsPoint(aabb.upperBound) || ContainsPoint(aabb.lowerBound));
		}

		Vec2f lowerBound;	///< the lower vertex
		Vec2f upperBound;	///< the upper vertex
	};

	struct Aabb3
	{
		/// Get the center of the AABB.
		Vec3f GetCenter() const
		{
			return (lowerBound + upperBound) * 0.5f;
		}

		/// Get the extents of the AABB (half-widths).
		Vec3f GetExtents() const
		{
			return (upperBound - lowerBound) * 0.5f;
		}

		/// Get the center of the AABB.
		void SetCenterExtend(Vec3f center, Vec3f extends) {
			lowerBound = center - extends;
			upperBound = center + extends;
		}

		bool ContainsPoint(Vec3f point)
		{
			return  (point <= upperBound && point >= lowerBound);
		}

		bool ContainsAabb(Aabb3 aabb)
		{
			return (ContainsPoint(aabb.upperBound) && ContainsPoint(aabb.lowerBound));
		}

		bool IntersectAabb(Aabb3 aabb)
		{
			return (ContainsPoint(aabb.upperBound) || ContainsPoint(aabb.lowerBound));
		}

		Vec3f lowerBound;	///< the lower vertex
		Vec3f upperBound;	///< the upper vertex
	};

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
			direction = dir.Normalized;
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
}