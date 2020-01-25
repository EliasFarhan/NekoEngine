#include "graphics/shape.h"

namespace neko
{

void NullShapeManager::AddTriangle(const Triangle& triangle)
{

}

void ShapeManager::AddTriangle(const Triangle& triangle)
{
    triangles_.push_back(triangle);
}
}