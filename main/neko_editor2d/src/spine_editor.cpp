#include <utilities/file_utility.h>
#include "tools/spine_editor.h"

namespace neko::editor
{


SpineEditorSystem::SpineEditorSystem(Configuration& config) :
        BasicEditorSystem(config),
        spineManager_(entityManager_)
{

}

void SpineEditorSystem::Init()
{
}

void SpineEditorSystem::Update(float dt)
{
}

void SpineEditorSystem::Destroy()
{
}

void SpineEditorSystem::OnListingView()
{
}

void SpineEditorSystem::OnMainView()
{
}

void SpineEditorSystem::OnInspectorView()
{
}

void SpineEditorSystem::OnSave()
{
}

EditorSystemId SpineEditorSystem::GetEditorSystemId() const
{
    return INVALID_EDITOR_SYSTEM_ID;
}

void SpineEditorSystem::SetCurrentSpine(std::string_view spineFilename)
{

}

void SpineEditorSystem::ReloadSpineSet()
{
    logDebug("Reload textures set in Texture Editor System");
    neko::IterateDirectory(config_.dataRootPath,
                           [this](std::string_view path)
                           {
                               if (!neko::IsRegularFile(path))
                                   return;
                               if (sfml::SpineManager::HasValidExtension(path))
                               {
                                   spineSet_.emplace(path);
                               }
                           }, true);
}

}
