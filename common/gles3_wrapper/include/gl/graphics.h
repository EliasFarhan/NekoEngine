#pragma once

#include <thread>
#include "graphics/graphics.h"
namespace neko::gl
{

class Gles3Renderer : public Renderer
{
protected:
    void ClearScreen() override;
    void Update() override;

    void BeforeRenderLoop() override;

    void AfterRenderLoop() override;

};

}