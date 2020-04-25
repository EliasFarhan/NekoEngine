#pragma once

#include <thread>
#include "graphics/graphics.h"
namespace neko::gl
{

class Gles3Renderer : public Renderer
{
public:
    void ClearScreen() override;

    void BeforeRenderLoop() override;

    void AfterRenderLoop() override;

};

}