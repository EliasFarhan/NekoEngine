#pragma once
#include "graphics/camera.h"
#include "voxel/chunk.h"
#include "voxel/region.h"

namespace neko::voxel
{
class Frustum
{
public:
    void SetCamera(const neko::Camera3D& camera);
    [[nodiscard]] bool Contains(Vec3f chunk, RegionId regionId) const;
    [[nodiscard]] bool Contains(const Cube& cube, Vec3f chunkPos, RegionId regionId) const;
private:
    float near_ = 0.0f, far_ = 0.0f;
    Vec3f dir_, rightDir_, upDir_, position_;
    Vec3f rightNormal_, leftNormal_, topNormal_, bottomNormal_;

};
}