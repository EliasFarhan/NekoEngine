#include "gl/graphics.h"

namespace neko::gl
{
void GlGraphicsManager::Render(RenderCommand* command)
{
    if(command == nullptr)
        return;
    auto* glRenderProgram = dynamic_cast<RenderProgram*>(command);
    if(glRenderProgram)
    {
        glRenderProgram->Render();
    }
}
}