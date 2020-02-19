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

namespace neko {
struct Obb2d {
    /// Get the center of the OBB.
    Vec2f GetCenter() const {
        return 0.5f * (lowerLeftBound + upperRightBound);
    }

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetRight() {
		return Vec2f(CalculateDirection().y,-CalculateDirection().x);
    }

    /// Set the center, the extent and the rotation of the OBB.
    void SetCenterExtentRot(Vec2f center, Vec2f extent, float rot) {
        lowerLeftBound = center - extent;
        upperRightBound = center + extent;
		rotation = rot;
    }


    bool IntersectObb(Obb2d obb) {
        Vec2f axis1 = CalculateDirection();
        Vec2f axis2 = GetRight();
        Vec2f axis3 = obb.CalculateDirection();
        Vec2f axis4 = obb.GetRight();

		float centersDistance = (GetCenter() - obb.GetCenter()).GetMagnitude();

		if(2*centersDistance <= GetExtentOnAxis(axis1) + obb.GetExtentOnAxis(axis1))
		{
			return false;
		}
		if (2 * centersDistance <= GetExtentOnAxis(axis2) + obb.GetExtentOnAxis(axis2))
		{
			return false;
		}
		if (2 * centersDistance <= GetExtentOnAxis(axis3) + obb.GetExtentOnAxis(axis3))
		{
			return false;
		}
		if (2 * centersDistance <= GetExtentOnAxis(axis4) + obb.GetExtentOnAxis(axis4))
		{
			return false;
		}

        return true;
    }

    float GetExtentOnAxis(Vec2f axis) {
        float extent;
		float rotationToAxis;

        if (axis == CalculateDirection()) {
            extent = upperRightBound.y - lowerLeftBound.y;
        }

        if (axis == GetRight()) {
            extent = upperRightBound.x - lowerLeftBound.x;
        }

		rotationToAxis = Vec2<float>::AngleBetween(axis, CalculateDirection());
		if (rotationToAxis >= 0)
		{
			rotationToAxis = rotationToAxis - int(rotationToAxis / PI) * PI;
		}
		else
		{
			rotationToAxis = rotationToAxis + int(rotationToAxis / PI) * PI;
		}

		if((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
		{
			Vec2f lowerLeftToTopRight = lowerLeftBound - upperRightBound;

			extent = lowerLeftToTopRight.GetMagnitude() * Vec2f::AngleBetween(lowerLeftToTopRight, axis);
		}
		else
		{
			Vec2f upperLeftBound = GetOppositeBound(upperRightBound, true);
			Vec2 lowerRightToUpperLeft = (upperLeftBound - GetCenter()) * 2;

			extent = lowerRightToUpperLeft.GetMagnitude() * Vec2f::AngleBetween(lowerRightToUpperLeft, axis);
		}

        return extent;
    }

    Vec2f GetOppositeBound(Vec2f bound, bool isUpper) {
        Vec2f centerToProjectionOnDir;
        Vec2f oppositeBound;
        Vec2f boundToOppositeBound;
        Vec2f centerToBound = bound - GetCenter();

        if (isUpper) 
		{
            centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).GetMagnitude() *
                std::cos(Vec2<float>::AngleBetween(centerToBound, CalculateDirection()));
            boundToOppositeBound =
                GetCenter() = centerToProjectionOnDir - bound; //TODO FIX THIS LINE
            oppositeBound = upperRightBound + boundToOppositeBound +
                            boundToOppositeBound;
        } else 
		{
            centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).GetMagnitude() *
                std::cos(Vec2<float>::AngleBetween(centerToBound, CalculateDirection())) * -1;
            boundToOppositeBound =
                GetCenter() = centerToProjectionOnDir - bound;
            oppositeBound = upperRightBound + boundToOppositeBound +
                            boundToOppositeBound;
        }

        return oppositeBound;
    }

	Vec2f CalculateDirection()	///< return the normal of the upper side
	{
		Vec2f direction;
		direction.x = std::cosf(rotation);
		direction.y = std::sinf(rotation);
		return direction;
	}

    Vec2f lowerLeftBound;	///< the lower vertex
    Vec2f upperRightBound;	///< the upper vertex
	float rotation;       ///< the angle of rotation in rd
};

struct Obb3d {

	/// Set the center, the extent and the rotations of the OBB.
	void SetCenterExtentRotZRotX(Vec3f center, Vec3f extent, float rotz, float rotx) {
		lowerLeftBound = center - extent;
		upperRightBound = center + extent;
		rotationZ = rotz;
		rotationX = rotx;
	}

	Vec3f CalculateDirection(){
		return Vec3f(cosf(rotationZ), sinf(rotationX) + sinf(rotationZ), cos(rotationX));		// TODO normalize vec3f
	}



	Vec3f lowerLeftBound;	///< the lower vertex
	Vec3f upperRightBound;	///< the upper vertex
	float rotationZ;       ///< the angle of rotation on the Z axis in rd
	float rotationX;       ///< the angle of rotation on the X axis in rd
};



struct Aabb2d {
    ///\brief Get the center of the AABB.
    Vec2f CalculateCenter() const { return 0.5f * (lowerBound + upperBound); }

    ///\brief Get the extends of the AABB (half-widths).
    Vec2f CalculateExtends() const { return 0.5f * (upperBound - lowerBound); }

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(const Vec2f center, const Vec2f extend) {
        lowerBound = center - extend;
        upperBound = center + extend;
    }

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
        const Vec2f center,
        const Vec2f extend,
        const float rotation) {
        const float newAngle = rotation / 180 * PI;
        std::array<Vec2f, 4> corners;
        corners[0] = lowerBound;
        corners[1] = Vec2f(lowerBound.x, upperBound.y);
        corners[2] = Vec2f(upperBound.x, lowerBound.y);
        corners[3] = upperBound;
        const float cosAngle = cosf(newAngle);
        const float sinAngle = sinf(newAngle);
        for (const Vec2f corner : corners) {
            const float x = corner.x * cosAngle - corner.y * sinAngle;
            const float y = corner.x * sinAngle + corner.y * cosAngle;
            if (x > upperBound.x) { upperBound.x = x; }
            if (x < lowerBound.x) { lowerBound.x = x; }
            if (y > upperBound.y) { upperBound.y = y; }
            if (y < lowerBound.y) { lowerBound.y = y; }
        }
    }

    ///\brief Set the AABB from OBB.
    void FromObb(Obb2d obb) {
        FromCenterExtendsRotation(0.5f * (
                                      obb.upperRightBound - obb.lowerLeftBound),
                                  obb.GetCenter(),
                                  Vec2f::AngleBetween<>(Vec2f(0, 1),
                                                        obb.CalculateDirection()));
    }

    bool ContainsPoint(const Vec2f point) const {
        bool contains = point.x <= upperBound.x && point.x >= lowerBound.x;
        contains = point.y <= upperBound.y && point.y >= lowerBound.y &&
                   contains;
        return contains;
    }

    bool ContainsAabb(const Aabb2d& aabb) const {
        return (ContainsPoint(aabb.upperBound) && ContainsPoint(aabb.lowerBound)
        );
    }

    bool IntersectAabb(const Aabb2d& aabb) const {
        return (ContainsPoint(aabb.upperBound) || ContainsPoint(aabb.lowerBound)
        );
    }

    Vec2f lowerBound = Vec2f::Zero; // the lower vertex
    Vec2f upperBound = Vec2f::Zero; // the upper vertex
};

struct Aabb3d {
    ///\brief Get the center of the AABB.
    Vec3f CalculateCenter() const { return (lowerBound + upperBound) * 0.5f; }

    ///\brief Get the extends of the AABB (half-widths).
    Vec3f CalculateExtends() const { return (upperBound - lowerBound) * 0.5f; }

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(
        const Vec3f center,
        const Vec3f extends,
        const Vec3f rotation = Vec3f::Zero) {
        lowerBound = center - extends;
        upperBound = center + extends;
    }

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
        const Vec3f center,
        const Vec3f extends,
        const Vec3f rotation) {
        lowerBound = center - extends;
        upperBound = center + extends;
        Vec3f newAngle = Vec3f(rotation.x / 180 * M_PI,
                               rotation.y / 180 * M_PI,
                               rotation.z / 180 * M_PI);
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
        for (Vec3f corner : corners) {
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
            if (x > upperBound.x) { upperBound.x = x; }
            if (x < lowerBound.x) { lowerBound.x = x; }
            if (y > upperBound.y) { upperBound.y = y; }
            if (y < lowerBound.y) { lowerBound.y = y; }
            if (z > upperBound.z) { upperBound.z = z; }
            if (z < lowerBound.z) { lowerBound.z = z; }
        }
    }

    /////\brief Set the AABB from OBB.
    //void FromObb(Obb3 obb) {
    //	FromCenterExtendsRotation(0.5f * (obb.upperRightBound - obb.lowerLeftBound), obb.GetCenter(), Vec3f::AngleBetween<>(Vec3f(0, 1), obb.direction));
    //}

    bool ContainsPoint(const Vec3f point) const {
        bool contains = point.x <= upperBound.x && point.x >= lowerBound.x;
        contains = point.y <= upperBound.y && point.y >= lowerBound.y &&
                   contains;
        contains = point.z <= upperBound.z && point.z >= lowerBound.z &&
                   contains;
        return contains;
    }

    bool ContainsAabb(const Aabb3d& aabb) const {
        return (ContainsPoint(aabb.upperBound) &&
                ContainsPoint(aabb.lowerBound));
    }

    bool IntersectAabb(const Aabb3d& aabb) const {
        return (ContainsPoint(aabb.upperBound) ||
                ContainsPoint(aabb.lowerBound));
    }

    Vec3f lowerBound = Vec3f::Zero; // the lower vertex
    Vec3f upperBound = Vec3f::Zero; // the upper vertex
};

}
