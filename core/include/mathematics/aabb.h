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
        return 0.5f * (localLowerLeftBound + localUpperRightBound);
    }

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetRight() {
		return Vec2f(CalculateDirection().y,-CalculateDirection().x);
    }

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetUp() {
        return Vec2f(CalculateDirection());
    }

    /// Set the center, the extends and the rotation of the OBB.
    void FromCenterExtendsRotation(Vec2f center, Vec2f localExtends, degree_t rot) {
        localLowerLeftBound = center - localExtends;
        localUpperRightBound = center + localExtends;
		rotation = rot;
    }

    bool IntersectObb(Obb2d obb) {
        Vec2f axis1 = CalculateDirection();
        Vec2f axis2 = GetRight();
        Vec2f axis3 = obb.CalculateDirection();
        Vec2f axis4 = obb.GetRight();

		float centersDistance = (GetCenter() - obb.GetCenter()).Magnitude();

		if(centersDistance <= GetExtendOnAxis(axis1) + obb.GetExtendOnAxis(axis1))
		{
			return true;
		}
		if (centersDistance <= GetExtendOnAxis(axis2) + obb.GetExtendOnAxis(axis2))
		{
			return true;
		}
		if (centersDistance <= GetExtendOnAxis(axis3) + obb.GetExtendOnAxis(axis3))
		{
			return true;
		}
		if (centersDistance <= GetExtendOnAxis(axis4) + obb.GetExtendOnAxis(axis4))
		{
			return true;
		}

        return false;
    }

    float GetExtendOnAxis(Vec2f axis) {
        float extend;

        if (axis == GetUp()) {
            extend = 0.5f * (localUpperRightBound.y - localLowerLeftBound.y);
            return extend;
        }

        if (axis == GetRight()) {
            extend = 0.5f * (localUpperRightBound.x - localLowerLeftBound.x);
            return extend;
        }
		
		float rotationToAxis = Vec2f::AngleBetween(axis, CalculateDirection()).
            value();
		rotationToAxis = std::fmod(rotationToAxis, PI);

		if((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
		{
			Vec2f lowerLeftToTopRight = lowerLeftBound - upperRightBound;

            extend = (lowerLeftToTopRight.Magnitude() * Vec2f::AngleBetween(lowerLeftToTopRight, axis)).value();
		}
		else
		{
			Vec2f upperLeftBound = GetOppositeBound(upperRightBound, true);
			Vec2 lowerRightToUpperLeft = (upperLeftBound - GetCenter()) * 2;

            extend = (lowerRightToUpperLeft.Magnitude() * Vec2f::AngleBetween(lowerRightToUpperLeft, axis)).value();
		}

        return extend;
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
            oppositeBound = localUpperRightBound + boundToOppositeBound +
                            boundToOppositeBound;
        } else 
		{
            centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection())) * -1;
            boundToOppositeBound =
                GetCenter() + centerToProjectionOnDir - bound;
            oppositeBound = localLowerLeftBound + boundToOppositeBound +
                            boundToOppositeBound;
        }

        return oppositeBound;
    }

    Vec2f ProjectOnAxis(const Vec2f& axis)
    {
        float min = std::numeric_limits<float>::infinity();
        float max = -std::numeric_limits<float>::infinity();
        std::array<Vec2f, 4> corners;
        corners[0] = localLowerLeftBound.Rotate(rotation);
        corners[1] = Vec2f(localUpperRightBound.x, localLowerLeftBound.y).Rotate(rotation);
        corners[2] = Vec2f(localLowerLeftBound.x, localUpperRightBound.y).Rotate(rotation);
        corners[3] = localUpperRightBound.Rotate(rotation);
        for (auto& corner : corners) {
            float projection = Vec2f::Dot(corner, axis);
            if (projection < min) { min = projection; }
            if (projection > max) { max = projection; }
        }
        return Vec2f(min, max);
    }

    std::array<Vec2f, 4> GetPerpendicularAxes(const std::array<Vec2f, 4>& vertices1, const std::array<Vec2f, 4>& vertices2)
    {
        std::array<Vec2f, 4> axes;

        return axes;
    }

    bool IsOverlapping(Obb2d& obb1)
    {
        std::array<Vec2f, 4 > perpendicularAxis = { GetUp(), GetRight(), obb1.GetUp(), obb1.GetRight(), };
        // we need to find the minimal overlap and axis on which it happens
        float minOverlap = std::numeric_limits<float>::infinity();

        for (auto& axis : perpendicularAxis) {
            Vec2f proj1 = ProjectOnAxis(axis);
            Vec2f proj2 = obb1.ProjectOnAxis(axis);

            float overlap = std::min(proj1.y, proj2.y) - std::max(proj1.x, proj2.x);
            if (overlap == 0.f) { // shapes are not overlapping
                return false;
            }
            else {
                if (overlap < minOverlap) {
                    minOverlap = overlap;
                }
            }
        }

        return true;
    }

	Vec2f CalculateDirection()	///< return the normal of the upper side
	{
		Vec2f direction;
		direction.x = Cos(rotation);
		direction.y = Sin(rotation);
		return direction;
	}

    Vec2f localLowerLeftBound;	///< the lower vertex
    Vec2f  localUpperRightBound;	///< the upper vertex
    radian_t rotation;       ///< the angle of rotation in rd
};

struct Obb3d {
    Vec2f GetCenter() const {
        return 0.5f * (localLowerLeftBound + localUpperRightBound);
    }

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetRight() {
        return Vec2f(CalculateDirection().y, -CalculateDirection().x);
    }

    /// Get the direction of the OBB rotated 90 degrees clockwise.
    Vec2f GetUp() {
        return Vec2f(CalculateDirection());
    }

    /// Set the center, the extends and the rotation of the OBB.
    void FromCenterExtendsRotation(Vec2f center, Vec2f localExtends, degree_t rot) {
        localLowerLeftBound = center - localExtends;
        localUpperRightBound = center + localExtends;
        rotation = rot;
    }

    bool IntersectObb(Obb2d obb) {
        Vec2f axis1 = CalculateDirection();
        Vec2f axis2 = GetRight();
        Vec2f axis3 = obb.CalculateDirection();
        Vec2f axis4 = obb.GetRight();

        float centersDistance = (GetCenter() - obb.GetCenter()).Magnitude();

        if (centersDistance <= GetExtendOnAxis(axis1) + obb.GetExtendOnAxis(axis1))
        {
            return true;
        }
        if (centersDistance <= GetExtendOnAxis(axis2) + obb.GetExtendOnAxis(axis2))
        {
            return true;
        }
        if (centersDistance <= GetExtendOnAxis(axis3) + obb.GetExtendOnAxis(axis3))
        {
            return true;
        }
        if (centersDistance <= GetExtendOnAxis(axis4) + obb.GetExtendOnAxis(axis4))
        {
            return true;
        }

        return false;
    }

    float GetExtendOnAxis(Vec2f axis) {
        float extend;

        if (axis == GetUp()) {
            extend = 0.5f * (localUpperRightBound.y - localLowerLeftBound.y);
            return extend;
        }

        if (axis == GetRight()) {
            extend = 0.5f * (localUpperRightBound.x - localLowerLeftBound.x);
            return extend;
        }

        float rotationToAxis = Vec2f::AngleBetween(axis, CalculateDirection()).
            value();
        rotationToAxis = std::fmod(rotationToAxis, PI);

        if ((rotationToAxis >= 0 && rotationToAxis <= PI / 2) || (rotationToAxis >= -PI && rotationToAxis <= -PI / 2))
        {
            Vec2f lowerLeftToTopRight = lowerLeftBound - upperRightBound;

            extend = (lowerLeftToTopRight.Magnitude() * Vec2f::AngleBetween(lowerLeftToTopRight, axis)).value();
        }
        else
        {
            Vec2f upperLeftBound = GetOppositeBound(upperRightBound, true);
            Vec2 lowerRightToUpperLeft = (upperLeftBound - GetCenter()) * 2;

            extend = (lowerRightToUpperLeft.Magnitude() * Vec2f::AngleBetween(lowerRightToUpperLeft, axis)).value();
        }

        return extend;
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
            oppositeBound = localUpperRightBound + boundToOppositeBound +
                boundToOppositeBound;
        }
        else
        {
            centerToProjectionOnDir =
                CalculateDirection() * (centerToBound).Magnitude() *
                Cos(Vec2f::AngleBetween(centerToBound, CalculateDirection())) * -1;
            boundToOppositeBound =
                GetCenter() + centerToProjectionOnDir - bound;
            oppositeBound = localLowerLeftBound + boundToOppositeBound +
                boundToOppositeBound;
        }

        return oppositeBound;
    }

    Vec2f ProjectOnAxis(const Vec2f& axis)
    {
        float min = std::numeric_limits<float>::infinity();
        float max = -std::numeric_limits<float>::infinity();
        std::array<Vec2f, 4> corners;
        corners[0] = localLowerLeftBound.Rotate(rotation);
        corners[1] = Vec2f(localUpperRightBound.x, localLowerLeftBound.y).Rotate(rotation);
        corners[2] = Vec2f(localLowerLeftBound.x, localUpperRightBound.y).Rotate(rotation);
        corners[3] = localUpperRightBound.Rotate(rotation);
        for (auto& corner : corners) {
            float projection = Vec2f::Dot(corner, axis);
            if (projection < min) { min = projection; }
            if (projection > max) { max = projection; }
        }
        return Vec2f(min, max);
    }

    std::array<Vec2f, 4> GetPerpendicularAxes(const std::array<Vec2f, 4>& vertices1, const std::array<Vec2f, 4>& vertices2)
    {
        std::array<Vec2f, 4> axes;

        return axes;
    }

    bool IsOverlapping(Obb2d& obb1)
    {
        std::array<Vec2f, 4 > perpendicularAxis = { GetUp(), GetRight(), obb1.GetUp(), obb1.GetRight(), };
        // we need to find the minimal overlap and axis on which it happens
        float minOverlap = std::numeric_limits<float>::infinity();

        for (auto& axis : perpendicularAxis) {
            Vec2f proj1 = ProjectOnAxis(axis);
            Vec2f proj2 = obb1.ProjectOnAxis(axis);

            float overlap = std::min(proj1.y, proj2.y) - std::max(proj1.x, proj2.x);
            if (overlap == 0.f) { // shapes are not overlapping
                return false;
            }
            else {
                if (overlap < minOverlap) {
                    minOverlap = overlap;
                }
            }
        }

        return true;
    }

    Vec3f GetUp()	///< return the normal of the upper side
    {
        Vec3f direction(1.0f, 0.0f, 0.0f);
        Vec3f point(1.0f, 0.0f, 0.0f);
        Mat3f rotateXMatrix = { Vec3f{1, 0, 0}, 0, Cos(rotation.x), -Sin(rotation.x), 0, Sin(rotation.x), Cos(rotation.x) }
        
        return direction;
    }

	Vec3f localLowerLeftBound;	///< the lower vertex
	Vec3f localUpperRightBound;	///< the upper vertex
    EulerAngles rotation;
};



struct Aabb2d {
    ///\brief Get the center of the AABB.
    Vec2f CalculateCenter() const { return 0.5f * (lowerLeftBound + upperRightBound); }

    ///\brief Get the extends of the AABB (half-widths).
    Vec2f CalculateExtends() const { return 0.5f * (upperRightBound - lowerLeftBound); }

    ///\brief Set the AABB from the center, extends.
    void FromCenterExtends(const Vec2f center, const Vec2f extends) {
        lowerLeftBound = center - extends;
        upperRightBound = center + extends;
    }

    ///\brief Enlarge AABB after a rotation
    void FromCenterExtendsRotation(
        const Vec2f center,
        const Vec2f extends,
        const degree_t rotation) {
        Vec2f relativeLowerLeftBound = extends *-1.0f;
        Vec2f relativeUpperRightBound = extends;
        radian_t newAngle = rotation;
        std::array<Vec2f, 4> corners;
        corners[0] = relativeLowerLeftBound.Rotate(newAngle);
        corners[1] = Vec2f(relativeLowerLeftBound.x, relativeUpperRightBound.y).Rotate(newAngle);
        corners[2] = Vec2f(relativeUpperRightBound.x, relativeLowerLeftBound.y).Rotate(newAngle);
        corners[3] = relativeUpperRightBound.Rotate(newAngle);
        const float cosAngle = Cos(newAngle);
        const float sinAngle = Sin(newAngle);
        for (const Vec2f corner : corners) {
            if (corner.x > relativeUpperRightBound.x) { relativeUpperRightBound.x = corner.x; }
            if (corner.x < relativeLowerLeftBound.x) { relativeLowerLeftBound.x = corner.x; }
            if (corner.y > relativeUpperRightBound.y) { relativeUpperRightBound.y = corner.y; }
            if (corner.y < relativeLowerLeftBound.y) { relativeLowerLeftBound.y = corner.y; }
        }
        upperRightBound = relativeUpperRightBound + center;
        lowerLeftBound = relativeLowerLeftBound + center;
    }

    ///\brief Set the AABB from OBB.
    void FromObb(Obb2d obb) {
        FromCenterExtendsRotation(
            obb.GetCenter(),
            Vec2f(obb.GetExtendOnAxis(obb.GetRight()), obb.GetExtendOnAxis(obb.CalculateDirection())),
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
        Vec3f relativeLowerLeftBound = extends * -1.0f;
        Vec3f relativeUpperRightBound = extends;
        Vec3<radian_t> newAngle = Vec3<radian_t>{
        rotation.x, rotation.y, rotation.z
        };
        std::array<Vec3f, 6> corners;
        corners[0] = relativeLowerLeftBound;
        corners[1] = upperRightBound;
        corners[2] = Vec3f(relativeLowerLeftBound.x, relativeUpperRightBound.y, relativeUpperRightBound.z);
        corners[3] = Vec3f(relativeLowerLeftBound.x, relativeLowerLeftBound.y, relativeUpperRightBound.z);
        corners[4] = Vec3f(relativeUpperRightBound.x, relativeLowerLeftBound.y, relativeLowerLeftBound.z);
        corners[5] = Vec3f(relativeUpperRightBound.x, relativeUpperRightBound.y, relativeLowerLeftBound.z);
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
            if (x > relativeUpperRightBound.x) { relativeUpperRightBound.x = x; }
            if (x < relativeLowerLeftBound.x) { relativeLowerLeftBound.x = x; }
            if (y > relativeUpperRightBound.y) { relativeUpperRightBound.y = y; }
            if (y < relativeLowerLeftBound.y) { relativeLowerLeftBound.y = y; }
            if (z > relativeUpperRightBound.z) { relativeUpperRightBound.z = z; }
            if (z < relativeLowerLeftBound.z) { relativeLowerLeftBound.z = z; }
        }
        lowerLeftBound = relativeLowerLeftBound + center;
        upperRightBound = relativeUpperRightBound + center;
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
