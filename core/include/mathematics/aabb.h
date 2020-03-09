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
    void SetCenterExtentRot(Vec2f center, Vec2f extent, degree_t rot) {
        lowerLeftBound = center - extent;
        upperRightBound = center + extent;
		rotation = rot;
    }


    bool IntersectObb(Obb2d obb) {
        Vec2f axis1 = CalculateDirection();
        Vec2f axis2 = GetRight();
        Vec2f axis3 = obb.CalculateDirection();
        Vec2f axis4 = obb.GetRight();

		float centersDistance = (GetCenter() - obb.GetCenter()).Magnitude();

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

        if (axis == CalculateDirection()) {
            extent = upperRightBound.y - lowerLeftBound.y;
        }

        if (axis == GetRight()) {
            extent = upperRightBound.x - lowerLeftBound.x;
        }
		
		float rotationToAxis = Vec2f::AngleBetween(axis, CalculateDirection()).
            value();
		rotationToAxis = std::fmod(rotationToAxis, PI);

		if((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
		{
			Vec2f lowerLeftToTopRight = lowerLeftBound - upperRightBound;

			extent = (lowerLeftToTopRight.Magnitude() * Vec2f::AngleBetween(lowerLeftToTopRight, axis)).value();
		}
		else
		{
			Vec2f upperLeftBound = GetOppositeBound(upperRightBound, true);
			Vec2 lowerRightToUpperLeft = (upperLeftBound - GetCenter()) * 2;

			extent = (lowerRightToUpperLeft.Magnitude() * Vec2f::AngleBetween(lowerRightToUpperLeft, axis)).value();
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
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection()));
            boundToOppositeBound =
                GetCenter() + centerToProjectionOnDir - bound;
            oppositeBound = upperRightBound + boundToOppositeBound +
                            boundToOppositeBound;
        } else 
		{
            centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection())) * -1;
            boundToOppositeBound =
                GetCenter() + centerToProjectionOnDir - bound;
            oppositeBound = upperRightBound + boundToOppositeBound +
                            boundToOppositeBound;
        }

        return oppositeBound;
    }

	Vec2f CalculateDirection()	///< return the normal of the upper side
	{
		Vec2f direction;
		direction.x = Cos(rotation);
		direction.y = Sin(rotation);
		return direction;
	}

    Vec2f lowerLeftBound;	///< the lower vertex
    Vec2f upperRightBound;	///< the upper vertex
    degree_t rotation;       ///< the angle of rotation in rd
};

struct Obb3d {

	/// Set the center, the extent and the rotations of the OBB.
	void SetCenterExtentRotZRotX(Vec3f center, Vec3f extent, degree_t rotz, degree_t rotx) {
		lowerLeftBound = center - extent;
		upperRightBound = center + extent;
		rotationZ = rotz;
		rotationX = rotx;
	}

	Vec3f CalculateDirection(){
		return Vec3f(Cos(rotationZ), Sin(rotationX) + Sin(rotationZ), Cos(rotationX)).Normalized();		// TODO normalize vec3f
	}

	bool IntersectObb(Obb3d obb)
	{
		Vec3f axis1 = GetDirection();
		Vec3f axis2 = GetRight();
		Vec3f axis3 = GetUp();
		Vec3f axis4 = obb.GetDirection();
		Vec3f axis5 = obb.GetRight();
		Vec3f axis6 = obb.GetUp();

		return false;
	}

	Vec3f GetDirection()
	{
		float x = Cos(rotationZ);
		float y = Sin(rotationZ)+Sin(rotationX);
		float z = Cos(rotationX);
		// return Vec3f(x, y, z).Normalized;
	}

	Vec3f GetRight()
	{

	}

	Vec3f GetUp()
	{

	}

	Vec3f lowerLeftBound;	///< the lower vertex
	Vec3f upperRightBound;	///< the upper vertex
	degree_t rotationZ;       ///< the angle of rotation on the Z axis in rd
	degree_t rotationX;       ///< the angle of rotation on the X axis in rd
};



struct Aabb2d {
    ///\brief Get the center of the AABB.
    Vec2f CalculateCenter() const { return 0.5f * (lowerLeftBound + upperRightBound); }

    ///\brief Get the extends of the AABB (half-widths).
    Vec2f CalculateExtends() const { return 0.5f * (upperRightBound - lowerLeftBound); }

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(const Vec2f center, const Vec2f extend) {
        lowerLeftBound = center - extend;
        upperRightBound = center + extend;
    }

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
        const Vec2f center,
        const Vec2f extend,
        const degree_t rotation) {
        radian_t newAngle = rotation;
        std::array<Vec2f, 4> corners;
        corners[0] = lowerLeftBound;
        corners[1] = Vec2f(lowerLeftBound.x, upperRightBound.y);
        corners[2] = Vec2f(upperRightBound.x, lowerLeftBound.y);
        corners[3] = upperRightBound;
        const float cosAngle = Cos(newAngle);
        const float sinAngle = Sin(newAngle);
        for (const Vec2f corner : corners) {
            const float x = corner.x * cosAngle - corner.y * sinAngle;
            const float y = corner.x * sinAngle + corner.y * cosAngle;
            if (x > upperRightBound.x) { upperRightBound.x = x; }
            if (x < lowerLeftBound.x) { lowerLeftBound.x = x; }
            if (y > upperRightBound.y) { upperRightBound.y = y; }
            if (y < lowerLeftBound.y) { lowerLeftBound.y = y; }
        }
    }

    ///\brief Set the AABB from OBB.
    void FromObb(Obb2d obb) {
        FromCenterExtendsRotation(
            obb.GetCenter(),
            Vec2f(obb.GetExtentOnAxis(obb.GetRight()), obb.GetExtentOnAxis(obb.CalculateDirection())),
            static_cast<degree_t>(obb.rotation));
    }

    bool ContainsPoint(const Vec2f point) const {
        bool contains = point.x <= upperRightBound.x && point.x >= lowerLeftBound.x;
        contains = point.y <= upperRightBound.y && point.y >= lowerLeftBound.y &&
                   contains;
        return contains;
    }

    bool ContainsAabb(const Aabb2d& aabb) const {
        return (ContainsPoint(aabb.upperRightBound) && ContainsPoint(aabb.lowerLeftBound)
        );
    }

    bool IntersectAabb(const Aabb2d& aabb) const {
        return (ContainsPoint(aabb.upperRightBound) || ContainsPoint(aabb.lowerLeftBound)
        );
    }


    bool IntersectRay(const Vec2f& dirRay, const Vec2f& origin) const {
        if (ContainsPoint(origin))
        {
            return true;
        }
        std::array<float, 4> touch;
        touch[0] = (lowerLeftBound.x - origin.x) / dirRay.x;
        touch[1] = (upperRightBound.x - origin.x) / dirRay.x;
        touch[2] = (lowerLeftBound.y - origin.y) / dirRay.y;
        touch[3] = (upperRightBound.y - origin.y) / dirRay.y;
        float touchMin = std::max(std::min(touch[0], touch[1]), std::min(touch[2], touch[3]));
        float touchMax = std::min(std::max(touch[0], touch[1]), std::max(touch[2], touch[3]));

        // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
        if (touchMax < 0)
        {
            return false;
        }

        // if tmin > tmax, ray doesn't intersect AABB
        if (touchMin > touchMax)
        {
            return false;
        }
        return true;

    }


    Vec2f lowerLeftBound = Vec2f::zero; // the lower vertex
    Vec2f upperRightBound = Vec2f::zero; // the upper vertex
};

struct Aabb3d {
    ///\brief Get the center of the AABB.
    Vec3f CalculateCenter() const { return (lowerLeftBound + upperRightBound) * 0.5f; }

    ///\brief Get the extends of the AABB (half-widths).
    Vec3f CalculateExtends() const { return (upperRightBound - lowerLeftBound) * 0.5f; }

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(
        const Vec3f center,
        const Vec3f extends,
        const Vec3f rotation = Vec3f::zero) {
        lowerLeftBound = center - extends;
        upperRightBound = center + extends;
    }

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
        const Vec3f center,
        const Vec3f extends,
        const EulerAngles rotation) {
        lowerLeftBound = center - extends;
        upperRightBound = center + extends;
        Vec3<radian_t> newAngle = Vec3<radian_t>{
        rotation.x, rotation.y, rotation.z
        };
        std::array<Vec3f, 6> corners;
        corners[0] = lowerLeftBound;
        corners[1] = upperRightBound;
        corners[2] = Vec3f(lowerLeftBound.x, upperRightBound.y, upperRightBound.z);
        corners[3] = Vec3f(lowerLeftBound.x, lowerLeftBound.y, upperRightBound.z);
        corners[4] = Vec3f(upperRightBound.x, lowerLeftBound.y, lowerLeftBound.z);
        corners[5] = Vec3f(upperRightBound.x, upperRightBound.y, lowerLeftBound.z);
        const float cosAngleX = Cos(newAngle.x);
        const float sinAngleX = Sin(newAngle.x);
        const float cosAngleY = Cos(newAngle.y);
        const float sinAngleY = Sin(newAngle.y);
        const float cosAngleZ = Cos(newAngle.z);
        const float sinAngleZ = Sin(newAngle.z);
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
            if (x > upperRightBound.x) { upperRightBound.x = x; }
            if (x < lowerLeftBound.x) { lowerLeftBound.x = x; }
            if (y > upperRightBound.y) { upperRightBound.y = y; }
            if (y < lowerLeftBound.y) { lowerLeftBound.y = y; }
            if (z > upperRightBound.z) { upperRightBound.z = z; }
            if (z < lowerLeftBound.z) { lowerLeftBound.z = z; }
        }
    }

    ////\brief Set the AABB3d from OBB3d.
    //void FromObb(Obb3d obb) {
    //	FromCenterExtendsRotation(obb.GetCenter(), obb.GetExtends(), Vec3f::AngleBetween<>(Vec3f(0, 1)));
    //}

    bool ContainsPoint(const Vec3f point) const {
        bool contains = point.x <= upperRightBound.x && point.x >= lowerLeftBound.x;
        contains = point.y <= upperRightBound.y && point.y >= lowerLeftBound.y &&
                   contains;
        contains = point.z <= upperRightBound.z && point.z >= lowerLeftBound.z &&
                   contains;
        return contains;
    }

    bool ContainsAabb(const Aabb3d& aabb) const {
        return (ContainsPoint(aabb.upperRightBound) &&
                ContainsPoint(aabb.lowerLeftBound));
    }

    bool IntersectAabb(const Aabb3d& aabb) const {
        return (ContainsPoint(aabb.upperRightBound) ||
                ContainsPoint(aabb.lowerLeftBound));
    }

    bool IntersectRay(const Vec3f& dirRay, const Vec3f& origin) const {
        if (ContainsPoint(origin))
        {
            return true;
        }
        std::array<float, 6> touch;
        touch[0] = (lowerLeftBound.x - origin.x) / dirRay.x;
        touch[1] = (upperRightBound.x - origin.x) / dirRay.x;
        touch[2] = (lowerLeftBound.y - origin.y) / dirRay.y;
        touch[3] = (upperRightBound.y - origin.y) / dirRay.y;
        touch[4] = (lowerLeftBound.z - origin.z) / dirRay.z;
        touch[5] = (upperRightBound.z - origin.z) / dirRay.z;
        float touchMin = std::max(std::max(std::min(touch[0], touch[1]), std::min(touch[2], touch[3])), std::min(touch[4], touch[5]));
        float touchMax = std::min(std::min(std::max(touch[0], touch[1]), std::max(touch[2], touch[3])), std::max(touch[4], touch[5]));

        // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
        if (touchMax < 0)
        {
            return false;
        }

        // if tmin > tmax, ray doesn't intersect AABB
        if (touchMin > touchMax)
        {
            return false;
        }
        return true;

    }


    bool IntersectPlane(const Vec3f& normal, const Vec3f& point) const {
        if (ContainsPoint(point))
        {
            return true;
        }
        Vec3f extends = CalculateExtends();
        Vec3f center = CalculateCenter();
        float r = extends.x * std::abs(normal.x) + extends.y * std::abs(normal.y) + extends.z * std::abs(normal.z);
        float d = Vec3f::Dot(normal, point);
        float s = Vec3f::Dot(normal, center)-d;
        return std::abs(s) <= r;
    }

    Vec3f lowerLeftBound = Vec3f::zero; // the lower vertex
    Vec3f upperRightBound = Vec3f::zero; // the upper vertex
};

}
