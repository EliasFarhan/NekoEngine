#include "mathematics/matrix.h"
#include "mathematics/multiple_vector.h"

namespace neko {
    template<>
    const Mat4f Mat4f::Identity = Mat4f(
            std::array<Vec4f, 4>{
                Vec4f(1, 0, 0, 0),
                Vec4f(0, 1, 0, 0),
                Vec4f(0, 0, 1, 0),
                Vec4f(0, 0, 0, 1)});


}