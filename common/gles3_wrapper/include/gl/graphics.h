#pragma once

#include <thread>
#include "graphics/graphics.h"
namespace neko::gl
{

class Gles3Renderer : public Renderer
{
public:

private:
    std::thread renderThread_;
};

}