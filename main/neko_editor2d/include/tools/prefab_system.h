#pragma once

#include <engine/system.h>
#include <SFML/Graphics/RenderTexture.hpp>
#include "engine_export.h"
#include <tools/neko_editor.h>

namespace neko::editor
{
class NekoEditor;
class EditorPrefabSystem : public NekoEditorSystem
{
public:
	using NekoEditorSystem::NekoEditorSystem;

    void Init() override;

    void Update(float dt) override;

    void Destroy() override;

    void OnListingView() override;

    void OnMainView() override;

    void OnInspectorView() override;

    void OnSave() override;

    EditorSystemId GetEditorSystemId() const override;

protected:
    Prefab currentPrefab_;
};
}