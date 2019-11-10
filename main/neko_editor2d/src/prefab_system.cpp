
#include "tools/prefab_system.h"
#include <tools/neko_editor.h>

namespace neko::editor
{

void EditorPrefabSystem::Init()
{

}

void EditorPrefabSystem::Update(float dt)
{

}

void EditorPrefabSystem::Destroy()
{

}

void EditorPrefabSystem::OnListingView()
{

}

void EditorPrefabSystem::OnMainView()
{

}

void EditorPrefabSystem::OnInspectorView()
{

}

void EditorPrefabSystem::OnSave()
{

}

EditorSystemId EditorPrefabSystem::GetEditorSystemId() const
{
    xxh::hash_state64_t prefabIdHashStream(0);
    //TODO hash the prefabId
    return prefabIdHashStream.digest();
}
}