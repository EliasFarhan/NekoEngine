#pragma once

#include <mathematics/vector.h>
#include <mathematics/matrix.h>
#include "mathematics/transform.h"

namespace neko
{

struct Camera
{
	virtual ~Camera() = default;
	Vec3f position;
    Vec3f reverseDirection = Vec3f::back, right = Vec3f::right, up = Vec3f::up;
	float nearPlane = 0.1f;
	float farPlane = 100.0f;
	void LookAt(Vec3f target, Vec3f lookUp = Vec3f::down)
	{
		reverseDirection = (position - target).Normalized();
		right = Vec3f::Cross(reverseDirection, lookUp).Normalized();
		up = Vec3f::Cross(reverseDirection, right).Normalized();
		
	}

	[[nodiscard]] Mat4f GenerateViewMatrix() const
	{

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
		const auto pitch = Quaternion::AngleAxis(angles.x, right);

		const auto yaw = Quaternion::AngleAxis(angles.y, up);

		const auto roll = Quaternion::AngleAxis(angles.z, reverseDirection);
		reverseDirection = Vec3f(Transform3d::RotationMatrixFrom(pitch*yaw*roll) * Vec4f(reverseDirection));
		LookAt(-reverseDirection+position);
	}

	[[nodiscard]] virtual Mat4f GenerateProjectionMatrix() const = 0;
};

struct Camera2D : Camera
{
	float right = 0.0f, left = 0.0f, top = 0.0f, bottom =0.0f;
	[[nodiscard]] Mat4f GenerateProjectionMatrix() const override
	{
		return Mat4f(std::array<Vec4f, 4>{
			Vec4f(2.0f / (right - left), 0, 0, 0),
			Vec4f(0, 2.0f / (top - bottom), 0, 0),
			Vec4f(0, 0, -2.0f / (farPlane - nearPlane), 0),
			Vec4f(-(right + left) / (right - left), 
					-(top + bottom) / (top - bottom), 
					-(farPlane + nearPlane) / (farPlane - nearPlane), 1.0f)
		});
	}
	void SetSize(Vec2f size)
	{
		left = -size.x;
		right = size.x;
		top = size.y;
		bottom = -size.y;
	}
};

struct Camera3D : Camera
{
	float aspect = 1.0f;
	degree_t fovY = degree_t(45.0f);

	[[nodiscard]] Mat4f GenerateProjectionMatrix() const override
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