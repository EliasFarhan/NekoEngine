//
// Created by efarhan on 7/31/19.
//
#include <tools/neko_editor.h>

namespace editor
{
void NekoEditor::Init()
{
    BasicEngine::Init();
    sceneRenderTexture_.create(config.gameWindowSize.x, config.gameWindowSize.y);

}

void NekoEditor::Update(float dt)
{
    BasicEngine::Update(dt);

    sceneRenderTexture_.clear(config.bgColor);

    //Draw things into the graphics manager

    graphicsManager_.Render(&sceneRenderTexture_);
    sceneRenderTexture_.display();
    sceneViewer_.Update(sceneRenderTexture_);

    entityViewer_.Update(entityManager_);
}

void NekoEditor::Destroy()
{
    BasicEngine::Destroy();
}
}