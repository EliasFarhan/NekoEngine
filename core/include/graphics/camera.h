#pragma once
/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

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


#include <mathematics/vector.h>
#include <mathematics/matrix.h>
#include "mathematics/transform.h"
#include "utils/service_locator.h"

namespace neko
{

struct Camera
{
    virtual ~Camera() = default;
    Vec3f position;
    Vec3f reverseDir = Vec3f::back, rightDir = Vec3f::right, upDir = Vec3f::up;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    void WorldLookAt(Vec3f target, Vec3f lookUp = Vec3f::down);

    [[nodiscard]] Mat4f GenerateViewMatrix() const;

    void SetDirectionFromEuler(const EulerAngles& angles);
    void Rotate(const EulerAngles& angles);

    [[nodiscard]] virtual Mat4f GenerateProjectionMatrix() const = 0;
};

struct Camera2D : Camera
{
	float right = 0.0f, left = 0.0f, top = 0.0f, bottom =0.0f;
	[[nodiscard]] Mat4f GenerateProjectionMatrix() const override;
	void SetExtends(Vec2f size);
};

struct Camera3D : Camera
{
	float aspect = 1.0f;
	degree_t fovY = degree_t(45.0f);

	[[nodiscard]] Mat4f GenerateProjectionMatrix() const override;;

	void SetAspect(int width, int height);

	[[nodiscard]] radian_t GetFovX() const;

	
};
/**
 * \brief Exclusiviley used for the CameraLocator, please do not use!
 */
struct NullCamera : Camera
{
    [[nodiscard]] Mat4f GenerateProjectionMatrix() const override;

};

using CameraLocator = Locator<Camera, NullCamera>;

}