
#include "tools/prefab_system.h"
#include <tools/neko_editor.h>

namespace neko::editor
{

EditorPrefabSystem::EditorPrefabSystem(NekoEditor& editor) :
        System(),
        editorExport_{
                entityManager_,
                position2dManager_,
                scale2dManager_,
                rotation2dManager_,
                transform2dManager_,
                sceneManager_,
                bodyDef2dManager_,
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
        transform2dManager_(position2dManager_, scale2dManager_, rotation2dManager_),
        sceneManager_(editorExport_),
        spriteManager_(textureManager_),
        colliderManagerDefManager_(editorExport_),
        prefabManager_(editorExport_)
{

}

void EditorPrefabSystem::Init()
{

}

void EditorPrefabSystem::Update(float dt)
{

}

void EditorPrefabSystem::Destroy()
{

}
}