#pragma once

#include "graphics/graphics.h"
namespace neko::gl
{
class GlGraphicsManager : public GraphicsManager
{
protected:
    void Render(RenderCommand* command) override;

};
}