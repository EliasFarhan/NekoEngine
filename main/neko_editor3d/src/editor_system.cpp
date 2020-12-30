//
// Created by efarhan on 30.12.20.
//

#include "editor_system.h"

namespace neko::editor
{

bool EditorSystem::IsActive() const
{
    return isActive_;
}

void EditorSystem::SetActive(bool isActive)
{
    isActive_ = isActive;
}
}