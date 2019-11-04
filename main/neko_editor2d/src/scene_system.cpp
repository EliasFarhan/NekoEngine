#include <tools/neko_editor.h>
#include "tools/scene_system.h"

namespace neko::editor
{
void EditorSceneSystem::Init()
{

}

void EditorSceneSystem::Update(float dt)
{

}

void EditorSceneSystem::Destroy()
{

}

EditorSceneSystem::EditorSceneSystem(NekoEditor& editor) :
        System(),
        editorExport_{
                entityManager_,
                position2dManager_,
                scale2dManager_,
                rotation2dManager_,
                transform2dManager_,
                sceneManager_,
                bodyDef2DManager_,
                spriteManager_,
                textureManager_,
                spineManager_,
                boxColliderDefManager_,
                circleColliderDefManager_,
                polygonColldierDefManager_,
                colliderManagerDefManager_,
                prefabManager_,
                editor
        },
        transform2dManager_(position2dManager_, scale2dManager_,
                            rotation2dManager_),
        prefabManager_(editorExport_),
        sceneManager_(editorExport_),
        colliderManagerDefManager_(editorExport_),
        spriteManager_(textureManager_)
{

}
}