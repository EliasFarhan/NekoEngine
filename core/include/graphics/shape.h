#include <vector>
#include <array>
#include "mathematics/vector.h"
#include "utilities/service_locator.h"
namespace neko
{
struct Shape
{
};

struct Triangle : Shape
{
    std::array<Vec4f,3> vertices;
};

class ShapeManagerInterface
{
public:
    virtual void AddTriangle(const Triangle& triangle) = 0;
};

class NullShapeManager : public ShapeManagerInterface
{
public:
    void AddTriangle(const Triangle& triangle) override;
};

class ShapeManager : public ShapeManagerInterface
{
public:
    void AddTriangle(const Triangle& triangle);

private:
    std::vector<Triangle> triangles_;
};

using ShapeManagerLocator = Locator<ShapeManagerInterface, NullShapeManager>;
}