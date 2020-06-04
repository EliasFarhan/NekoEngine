#pragma once

#include <mathematics/vector.h>
#include <mathematics/matrix.h>
#include "mathematics/transform.h"

namespace neko
{

struct Camera
{
    Vec3f position;
    Vec3f reverseDirection;

	void LookAt(Vec3f target)
	{
		const Vec3f direction = position - target;
		reverseDirection = direction.Normalized();
		
	}
	
	[[nodiscard]] Vec3f GetRight() const
	{
		return Vec3f::Cross(Vec3f::up, reverseDirection).Normalized();
	}

	[[nodiscard]] Vec3f GetUp() const
	{
		const Vec3f right = GetRight();
		return Vec3f::Cross(reverseDirection, right).Normalized();
	}
	[[nodiscard]] Mat4f GenerateViewMatrix() const
	{
		const Vec3f right = GetRight();
		const Vec3f up = GetUp();
		const Mat4f rotation(std::array<Vec4f, 4>{
			Vec4f(right.x, up.x, reverseDirection.x, 0.0f),
				Vec4f(right.y, up.y, reverseDirection.y, 0.0f),
				Vec4f(right.z, up.z, reverseDirection.z, 0.0f),
				Vec4f(0.0f, 0.0f, 0.0f, 1.0f)
		});
		const Mat4f translation(std::array<Vec4f, 4>{
			Vec4f(1,0,0,0),
			Vec4f(0,1,0,0),
			Vec4f(0,0,1,0),
			Vec4f(-position.x,-position.y,-position.z,1),
		});
		return rotation * translation;
	}

	void SetDirectionFromEuler(const EulerAngles& angles)
	{
		const Quaternion q = Quaternion::FromEuler(angles);
		reverseDirection = Vec3f(Transform3d::RotationMatrixFrom(q)*Vec4f(0,0,1,0));
	}
	void Rotate(const EulerAngles& angles)
	{
		const auto pitch = Quaternion::AngleAxis(angles.x, GetRight());

		const auto yaw = Quaternion::AngleAxis(angles.y, GetUp());

		const auto roll = Quaternion::AngleAxis(angles.z, reverseDirection);
		reverseDirection = Vec3f(Transform3d::RotationMatrixFrom(pitch*yaw*roll) * Vec4f(reverseDirection));
	}
};

struct Camera3D : Camera
{
	float aspect = 1.0f;
	degree_t fovY = degree_t(45.0f);
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	[[nodiscard]] Mat4f GenerateProjectionMatrix() const
	{
		return Transform3d::Perspective(
			fovY,
			aspect,
			nearPlane,
			farPlane);
	};

	void SetAspect(int width, int height)
	{
		aspect = static_cast<float>(width) / static_cast<float>(height);
	}

	radian_t GetFovX() const
	{
		return 2.0f*Atan(Tan(fovY*0.5f) * aspect);
	}

	
};
}