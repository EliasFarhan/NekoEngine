#pragma once

#include <engine/engine.h>
#include "entity_viewer.h"
#include "scene_viewer.h"

namespace editor
{

class NekoEditor : public neko::BasicEngine
{
public:
    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

private:
    neko::EntityManager entityManager_;
    EntityViewer entityViewer_;

    neko::BasicGraphicsManager graphicsManager_;
    sf::RenderTexture sceneRenderTexture_;
    SceneViewer sceneViewer_;


};

}