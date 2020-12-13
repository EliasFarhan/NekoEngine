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

#include "voxel/frustum.h"

namespace neko::voxel
{

void Frustum::SetCamera(const Camera3D& camera)
{
    near_ = camera.nearPlane;
    far_ = camera.farPlane;

    position_ = camera.position;
    dir_ = -camera.reverseDir;
    rightDir_ = camera.rightDir;
    upDir_ = camera.upDir;

    const auto fovX = camera.GetFovX();
    const auto rightQuaternion = Quaternion::AngleAxis(fovX / 2.0f, upDir_);
    rightNormal_ = Vec3f(Transform3d::RotationMatrixFrom(rightQuaternion) * Vec4f(rightDir_));

    const auto leftQuaternion = Quaternion::AngleAxis(-fovX / 2.0f, upDir_);
    leftNormal_ = Vec3f(Transform3d::RotationMatrixFrom(leftQuaternion) * Vec4f(-rightDir_));

    const auto topQuaternion = Quaternion::AngleAxis(camera.fovY / 2.0f, rightDir_);
    topNormal_ = Vec3f(Transform3d::RotationMatrixFrom(topQuaternion) * Vec4f(-upDir_));

    const auto bottomQuaternion = Quaternion::AngleAxis(-camera.fovY / 2.0f, rightDir_);
    bottomNormal_ = Vec3f(Transform3d::RotationMatrixFrom(bottomQuaternion) * Vec4f(upDir_));

}

bool Frustum::Contains(const Cube& cube, Vec3f chunkPos, RegionId regionId) const
{
    constexpr float radius = Sqrt(3.0f*(0.5f*0.5f));
    const auto cubeY = cube.cubeId & 31u;
    const auto cubeX = (cube.cubeId >> 10u) & 31u;
    const auto cubeZ = (cube.cubeId >> 5u) & 31u;
    const auto cubePos = Vec3f(
            float(cubeX),
            float(cubeY),
            float(cubeZ)) + chunkPos;
    //Near
    {
        const auto planePos = position_ + dir_ * near_;
        const auto asterPos = cubePos - planePos;
        const auto v = Vec3f::Dot(dir_, asterPos);
        if( v < -radius)
            return false;
    }
    //Far
    {
        const auto planePos = position_ + dir_ * far_;
        const auto asterPos = cubePos - planePos;
        const auto v = Vec3f::Dot(-dir_, asterPos);
        if (v < -radius)
            return false;
    }
    const auto deltaCubePos = cubePos - position_;
    //Right
    {
        const auto v = Vec3f::Dot(rightNormal_, deltaCubePos);
        if(v < -radius)
        {
            return false;
        }
    }
    //Left
    {
        const auto v = Vec3f::Dot(leftNormal_, deltaCubePos);
        if (v < -radius)
        {
            return false;
        }
    }
    //Top
    {


        const auto v = Vec3f::Dot(topNormal_, deltaCubePos);
        if (v < -radius)
        {
            return false;
        }
    }
    //Bottom
    {
        const auto v = Vec3f::Dot(bottomNormal_, deltaCubePos);
        if (v < -radius)
        {
            return false;
        }
    }
    return true;
}

bool Frustum::Contains(Vec3f chunkPos, RegionId regionId) const
{
    constexpr float radius = Sqrt(3.0f * static_cast<float>(chunkSize / 2u * chunkSize / 2u));
    const auto chunkHalfSize = Vec3f::one * static_cast<float>(chunkSize / 2u);
    const auto center = chunkPos + chunkHalfSize;
    //Near
    {
        const auto planePos = position_ + dir_ * near_;
        const auto chunkDeltaPos = chunkPos - planePos;
        const auto v = Vec3f::Dot(dir_, chunkDeltaPos);
        if( v < -radius)
            return false;
    }
    //Far
    {
        const auto planePos = position_ + dir_ * far_;
        const auto chunkDeltaPos = chunkPos - planePos;
        const auto v = Vec3f::Dot(-dir_, chunkDeltaPos);
        if (v < -radius)
            return false;
    }
    const auto chunkDeltaPos = center - position_;
    //Right
    {
        const auto v = Vec3f::Dot(rightNormal_, chunkDeltaPos);
        if(v < -radius)
        {
            return false;
        }
    }
    //Left
    {
        const auto v = Vec3f::Dot(leftNormal_, chunkDeltaPos);
        if (v < -radius)
        {
            return false;
        }
    }
    //Top
    {
        const auto v = Vec3f::Dot(topNormal_, chunkDeltaPos);
        if (v < -radius)
        {
            return false;
        }
    }
    //Bottom
    {
        const auto v = Vec3f::Dot(bottomNormal_, chunkDeltaPos);
        if (v < -radius)
        {
            return false;
        }
    }
    return true;
}
}