#pragma once

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

	void SetResourcePath(const std::string& resourcePath) override;

    EditorSystemId GetEditorSystemId() const override;

};
}