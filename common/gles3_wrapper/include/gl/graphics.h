#pragma once

#include <thread>

#include "texture.h"
#include "graphics/graphics.h"
namespace neko::gl
{

class Gles3Renderer : public Renderer
{
public:
    Gles3Renderer();
    void ClearScreen() override;

    void BeforeRenderLoop() override;

    void AfterRenderLoop() override;

};

}